// SPDX-License-Identifier: MIT
// Copyright (c) 2017-2019 Ulrich Hecht

#include <vector>

#include "basic.h"
#include "basic_native.h"
#include "eb_sys.h"

#include <dyncall.h>
#include <dlfcn.h>

std::vector<struct module> modules;

/***bc sys TCC
Compiles and, optionally, loads or runs C programs.
\usage
TCC [<c_file$>[, <c_file$> ...]] [TO <executable$>] [MOD] [ARG <argument$>[, <argument$> ...]]
\args
@c_file$	a C source code file to compile
@executable$	name of executable file to generate
@argument$	argument to pass to compiled file when run
\desc
Compiles the given C source code files to an executable file, or to a
loadable module if the `MOD` keyword is present.

The compiled program or module is written to `executable$`.

If `executable$` is not specified, the code is compiled to a temporary file
and immediately executed or loaded. For regular programs, any `argument$`
parameters will be passed to the program.
***/
void Basic::init_tcc() {
  callvm = dcNewCallVM(4096);
  dcMode(callvm, DC_CALL_C_DEFAULT);

  // add empty module to make default symbols visible to BASIC
  if (modules.size() == 0) {
    struct module system = { NULL, "system" };
    modules.push_back(system);
  }
}

void Basic::itcc() {
  bool run_tmp = false;
  bool is_mod = false;
  BString tmpfile;
  char tmpdir[17] = "/tmp/ebtccXXXXXX";

  std::list<BString> args;
  args.push_back("tcc");
  args.push_back("-I");
  args.push_back(BString(getenv("ENGINEBASIC_ROOT")) + BString("/sys/include"));

  // file(s) to compile
  for (;;) {
    BString file = getParamFname();
    if (err)
      return;
    args.push_back(file);
    if (*cip == I_COMMA)
      ++cip;
    else
      break;
  }

  // output file (tmp file if not specified)
  if (*cip == I_TO) {
    ++cip;
    BString outfile = getParamFname();
    args.push_back("-o");
    args.push_back(outfile);
  } else {
    run_tmp = true;
    args.push_back("-o");
    if (!mkdtemp(tmpdir)) {
      err = ERR_OS;
      err_expected = _("unable to create temporary directory");
      return;
    }
    tmpfile = BString(tmpdir) + BString(is_mod ? "/a.so" : "/a.out");
    args.push_back(tmpfile);
  }

  // module?
  if (*cip == I_MOD) {
    ++cip;
    is_mod = true;
    args.push_back("-shared");
  }

  // compile
  if (run_list(args)) {
    err = ERR_COMPILE;
    goto cleanup;
  }

  // if it's a temp file, we want to load/run it immediately
  if (run_tmp) {
    if (is_mod) {
      eb_load_module(tmpfile.c_str());
    } else {
      std::list<BString> run_args;
      run_args.push_back(tmpfile);

      if (*cip == I_ARG) {
        ++cip;

        while (!end_of_statement()) {
          BString arg = istrexp();
          if (err)
            goto cleanup;

          run_args.push_back(arg);

          if (*cip == I_COMMA)
            ++cip;
          else
            break;
        }
      }

      // XXX: I_OFF

      shell_list(run_args);
    }

  }

cleanup:
  if (run_tmp) {
    unlink(tmpfile.c_str());
    rmdir(tmpdir);
  }

  if (!end_of_statement())
    SYNTAX_T(_("expected end of statement"));
}

void *Basic::get_symbol(const char *sym_name) {
  dlerror();

  void *sym = dlsym(RTLD_DEFAULT, sym_name);

  if (!dlerror())
    return sym;
  else
    return NULL;
}

const char *Basic::get_name(void *addr) {
  static Dl_info info;
  if (!dladdr(addr, &info))
    return NULL;

  // XXX: returns symbol even if addr is not the start address; do we want that?
  return info.dli_sname;
}

Basic::nfc_result Basic::do_nfc(void *sym, enum return_type rtype) {
  static float empty[16] = {};
  nfc_result ret;

  dcReset(callvm);

  std::vector<char *> string_list;

  if (!end_of_statement()) {
    do {
      if (*cip == I_SHARP) {
        ++cip;
        double arg = iexp();
        if (err)
          goto out;
        dcArgDouble(callvm, arg);
      } else if (*cip == I_BANG) {
        ++cip;
        float arg = iexp();
        if (err)
          goto out;
        dcArgFloat(callvm, arg);
      } else if (*cip == I_MUL) {
        ++cip;
        void *arg = (void *)(uintptr_t)iexp();
        if (err)
          goto out;
        dcArgPointer(callvm, arg);
      } else if (is_strexp()) {
        BString arg_str = istrexp();
        char *arg = strdup(arg_str.c_str());
        if (err)
          goto out;
        string_list.push_back(arg);
        dcArgPointer(callvm, arg);
      } else {
        int arg = (int)iexp();
        if (err)
          goto out;
        dcArgInt(callvm, arg);
      }
    } while (*cip++ == I_COMMA);
    --cip;
  }

  dcArgPointer(callvm, NULL);

  switch (rtype) {
    case RET_INT: ret.rint = dcCallInt(callvm, sym); break;
    case RET_UINT: ret.ruint = dcCallInt(callvm, sym); break;
    case RET_POINTER: ret.rptr = dcCallPointer(callvm, sym); break;
    case RET_FLOAT: ret.rflt = dcCallFloat(callvm, sym); break;
    case RET_DOUBLE: ret.rdbl = dcCallDouble(callvm, sym); break;
  }

out:
  for (auto s : string_list) {
    free(s);
  }

  return ret;
}

void Basic::infc() {
  void *sym = *((void **)cip);
  cip += icodes_per_ptr();

  do_nfc(sym);
}

num_t Basic::nnfc() {
  void *sym = *((void **)cip);
  cip += icodes_per_ptr();

  return_type rtype = RET_INT;

  if (*cip == I_SHARP) {
    ++cip;
    rtype = RET_DOUBLE;
  } else if (*cip == I_BANG) {
    ++cip;
    rtype = RET_FLOAT;
  } else if (*cip == I_MUL) {
    ++cip;
    rtype = RET_POINTER;
  };

  if (checkOpen())
    return 0;

  nfc_result ret = do_nfc(sym, rtype);

  if (checkClose())
    return 0;

  switch (rtype) {
    case RET_INT: return ret.rint;
    case RET_UINT: return ret.ruint;
    case RET_POINTER: return (uintptr_t)ret.rptr;
    case RET_FLOAT: return ret.rflt;
    case RET_DOUBLE: return ret.rdbl;
    default: err = ERR_SYS; return 0;
  }
}

BString Basic::snfc() {
  BString out;

  void *sym = *((void **)cip);
  cip += icodes_per_ptr();

  return_type rtype = RET_POINTER;
  if (*cip == I_MUL)
    ++cip;

  if (checkOpen())
    return out;

  nfc_result ret = do_nfc(sym, rtype);

  if (checkClose())
    return out;

  out = (char *)ret.rptr;
  return out;
}

/***bf sys GETSYM
Returns a pointer to a native symbol.
\usage ptr = GETSYM(name$)
\args
@name$	name of a native symbol
\ret Pointer to requested symbol, or 0 if not found.
***/
num_t Basic::ngetsym() {
  if (checkOpen())
    return 0;
  BString sym = istrexp();
  if (checkClose())
    return 0;

  return (uintptr_t)get_symbol(sym.c_str());
}

#include <sys/wait.h>

int exec_list(std::list<BString> &args) {
  std::vector<const char *> argsp;

  for (auto &s : args)
    argsp.push_back(s.c_str());

  argsp.push_back(NULL);

  return execvp(argsp[0], (char *const *)argsp.data());
}

int run_list(std::list<BString> &args) {
  pid_t pid = fork();
  if (pid < 0) {
    return -1;
  } else if (pid > 0) {
    int wstatus;
    waitpid(pid, &wstatus, 0);
    return WEXITSTATUS(wstatus);
  } else {
    exec_list(args);
    return -1;
  }
}
