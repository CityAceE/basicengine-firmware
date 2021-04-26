R(printf, c_printf)

R(usleep, delayMicroseconds)

S(sprintf)
S(strncpy)
S(vsnprintf)
S(snprintf)
S(vasprintf)
S(asprintf)

R(exit, be_exit)

S(malloc)
S(free)
S(realloc)
S(calloc)

S(eb_locate)
S(eb_putch)
S(eb_getch)
S(eb_clrtoeol)
S(eb_cls)
S(eb_puts)
S(eb_show_cursor)
S(eb_csize_height)
S(eb_csize_width)
S(eb_enable_scrolling)
S(eb_show_cursor)
S(eb_kbhit)

S(eb_wait)

S(atoi)

S(strcpy)
S(strlen)
S(strcat)
S(strcmp)
S(strncmp)
S(strtol)
S(strtoll)
R(strchr, __builtin_strchr)
R(strstr, __builtin_strstr)
R(strrchr, __builtin_strrchr)
S(strdup)
S(strerror)
S(strspn)
S(strcspn)
S(strndup)

S(write)

S(unlink)
S(mkstemp)
S(rename)
S(mkdir)
S(rmdir)

S(readdir)
S(closedir)
S(opendir)
S(getcwd)
S(chdir)
S(fopen)
S(fclose)
S(fputc)
S(ferror)
S(fgets)
S(fgetc)
S(fputs)
S(fprintf)
S(fread)
S(fseek)
S(ftell)
S(fwrite)
S(fdopen)
S(fileno)

S(open)
S(close)
S(read)
S(write)

R(stdout, &stdout)
R(stdin, &stdin)
R(stderr, &stderr)

S(memcmp)
S(memcpy)
S(memset)
S(memmove)
S(bcopy)
S(bzero)
S(memrchr)

S(putenv)
S(getenv)

S(qsort)
R(abs, __builtin_abs)

S(getopt)
R(optarg, &optarg)
R(optind, &optind)

S(isdigit)
S(isprint)
S(isspace)
S(isalnum)

#ifdef __x86_64__
S(__va_start)
S(__va_arg)
#endif
