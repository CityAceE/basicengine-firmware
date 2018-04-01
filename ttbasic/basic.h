#ifndef __BASIC_H
#define __BASIC_H

#include "ttconfig.h"

#include <stdint.h>
#include <string.h>
#include <sdfiles.h>
#include "BString.h"

#include "variable.h"
#include "proc.h"

#define SIZE_LINE 256    // コマンドライン入力バッファサイズ + NULL
#define SIZE_IBUF 256    // 中間コード変換バッファサイズ

extern char lbuf[SIZE_LINE];
extern char tbuf[SIZE_LINE];
extern int32_t tbuf_pos;
extern unsigned char ibuf[SIZE_IBUF];

extern unsigned char *listbuf;

extern uint8_t err; // Error message index

#define MAX_RETVALS 4
extern num_t retval[MAX_RETVALS];

uint8_t toktoi(bool find_prg_text = true);
void putlist(unsigned char* ip, uint8_t devno=0);

// メモリ書き込みポインタのクリア
static inline void cleartbuf() {
  tbuf_pos=0;
  memset(tbuf,0,SIZE_LINE);
}

#define c_getch( ) sc0.get_ch()
#define c_kbhit( ) sc0.isKeyIn()

void c_putch(uint8_t c, uint8_t devno = 0);
void c_puts(const char *s, uint8_t devno=0);
void c_puts_P(const char *s, uint8_t devno=0);
void screen_putch(uint8_t c, bool lazy = false);

#define PRINT_P(msg) c_puts_P(PSTR((msg)))

#define dbg_printf(x, y...) printf_P(PSTR(x), y)

void putnum(num_t value, int8_t d, uint8_t devno=0);
void putint(int value, int8_t d, uint8_t devno=0);
void putHexnum(uint32_t value, uint8_t d, uint8_t devno=0);

void newline(uint8_t devno=0);

extern unsigned char *clp;
extern unsigned char *cip;

BString getParamFname();
num_t getparam();

void get_input(bool numeric = false);

uint32_t getPrevLineNo(uint32_t lineno);
uint32_t getNextLineNo(uint32_t lineno);
uint32_t getBottomLineNum();
uint32_t getTopLineNum();
char* getLineStr(uint32_t lineno, uint8_t devno = 3);

// '('チェック関数
inline uint8_t checkOpen() {
  if (*cip != I_OPEN) err = ERR_PAREN;
  else cip++;
  return err;
}

// ')'チェック関数
inline uint8_t checkClose() {
  if (*cip != I_CLOSE) err = ERR_PAREN;
  else cip++;
  return err;
}

int BASIC_FP token_size(uint8_t *code);
num_t BASIC_FP iexp();
BString istrexp(void);

// キーワードテーブル
#include "kwtbl.h"

// Keyword count
#define SIZE_KWTBL (sizeof(kwtbl) / sizeof(const char*))

// i-code(Intermediate code) assignment
#include "kwenum.h"

extern uint8_t err;
extern const char *err_expected;

#define E_SYNTAX(exp) do { err = ERR_SYNTAX; err_expected = kwtbl[exp]; } while(0)
#define SYNTAX_T(exp) do { static const char __msg[] PROGMEM = exp; \
                           err = ERR_SYNTAX; err_expected = __msg; \
                      } while(0)
extern void E_VALUE(int32_t from, int32_t to);

#define E_ERR(code, exp) do { \
  static const char __msg[] PROGMEM = exp; \
  err = ERR_ ## code; err_expected = __msg; \
  } while(0)


#ifdef FLOAT_NUMS
// コマンド引数取得(uint32_t,引数チェックなし)
static inline uint8_t BASIC_FP getParam(int32_t& prm, token_t next_token) {
  num_t p = iexp();
  prm = p;
  if (!err && next_token != I_NONE && *cip++ != next_token) {
    E_SYNTAX(next_token);
  }
  return err;
}
// コマンド引数取得(int32_t,引数チェックあり)
static inline uint8_t BASIC_FP getParam(int32_t& prm, int32_t v_min,  int32_t v_max, token_t next_token) {
  prm = iexp();
  if (!err && (prm < v_min || prm > v_max))
    E_VALUE(v_min, v_max);
  else if (next_token != I_NONE && *cip++ != next_token) {
    E_SYNTAX(next_token);
  }
  return err;
}
#endif

#ifdef ESP8266
extern "C" size_t umm_free_heap_size( void );
#endif

#include <tscreenBase.h>
#include <tTVscreen.h>

extern tTVscreen sc0;

#define NEW_ALL		0
#define NEW_PROG	1
#define NEW_VAR		2

void c_puts(const char *s, uint8_t devno);
void c_puts_P(const char *s, uint8_t devno);

#define COL_BG		0
#define COL_FG		1
#define COL_KEYWORD	2
#define COL_LINENUM	3
#define COL_NUM		4
#define COL_VAR		5
#define COL_LVAR	6
#define COL_OP		7
#define COL_STR		8
#define COL_PROC	9
#define COL_COMMENT	10
#define COL_BORDER	11

#define CONFIG_COLS	12

typedef struct {
  int16_t NTSC;        // NTSC設定 (0,1,2,3)
  int16_t KEYBOARD;    // キーボード設定 (0:JP, 1:US)
  uint8_t color_scheme[CONFIG_COLS][3];
  bool interlace;
  bool lowpass;
  uint8_t mode;
  uint8_t font;
  uint8_t cursor_color;
  uint8_t beep_volume;
} SystemConfig;
extern SystemConfig CONFIG;

#define COL(n)	(vs23.colorFromRgb(CONFIG.color_scheme[COL_ ## n]))

#endif
