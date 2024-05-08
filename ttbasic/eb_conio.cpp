// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Ulrich Hecht

#include "eb_api.h"
#include "eb_conio.h"
#include "eb_file.h"

#include "basic.h"
#include <fonts.h>

EBAPI void eb_locate(int x, int y) {
  if (x >= sc0.getWidth())  // xの有効範囲チェック
    x = sc0.getWidth() - 1;
  else if (x < 0)
    x = 0;
  if (y >= sc0.getHeight())  // yの有効範囲チェック
    y = sc0.getHeight() - 1;
  else if (y < 0)
    y = 0;

  // カーソル移動
  sc0.locate((uint16_t)x, (uint16_t)y);
}

EBAPI void eb_window_off(void) {
  sc0.setWindow(0, 0, sc0.getScreenWidth(), sc0.getScreenHeight());
  sc0.setScroll(true);
}

EBAPI int eb_window(int x, int y, int w, int h) {
  if (check_param(x, 0, sc0.getScreenWidth() - 1) ||
      check_param(y, 0, sc0.getScreenHeight() - 1) ||
      check_param(w, 1, sc0.getScreenWidth() - x) ||
      check_param(h, 1, sc0.getScreenHeight() - y))
    return -1;

  sc0.setWindow(x, y, w, h);
  sc0.setScroll(h > 1 ? true : false);
  sc0.locate(0, 0);
  sc0.show_curs(false);

  return 0;
}

EBAPI int eb_font(int idx) {
  if (check_param(idx, 0, eb_font_count() - 1))
    return -1;
  sc0.setFontByIndex(idx);
  sc0.forget();
  return 0;
}

EBAPI const char *eb_font_info(int idx, int *w, int *h) {
  return sc0.fontInfo(idx, w, h);
}

EBAPI int eb_font_by_name(const char *name, int w, int h) {
  if (!sc0.setFontByName(name, w, h)) {
    err = ERR_FONT;
    err_expected = _("unknown font");
    return -1;
  }
  sc0.forget();
  return sc0.currentFontIndex();
}

EBAPI int eb_load_font(const char *file_name) {
  int ret = -1;
  int size = eb_file_size(file_name);
  if (size < 0) {
    err = ERR_FILE_OPEN;
    return ret;
  }

  BString filestr = BString(file_name);

  int base_index = filestr.lastIndexOf('/') + 1;

  int ext_index = filestr.lastIndexOf('.');
  if (ext_index <= base_index)
    ext_index = -1;

  BString font_name = filestr.substring(base_index, ext_index);

  if (sc0.haveFont(font_name.c_str())) {
    err = ERR_FONT;
    err_expected = _("font already exists");
    return -1;
  }

  FILE *ttf = fopen(file_name, "rb");
  if (!ttf) {
    err = ERR_FILE_OPEN;
    return ret;
  }

  uint8_t *font = new uint8_t[size];
  if (fread(font, size, 1, ttf) != 1)
    err = ERR_FILE_READ;
  else {
    int fret = sc0.addFont(font, font_name.c_str());
    if (fret < 0) {
      err = ERR_FONT;
      if (fret == -2)
        err_expected = _("not a valid TrueType font file");
    } else
      ret = 0;
  }

  fclose(ttf);
  return ret;
}

EBAPI int eb_font_count(void) {
  return sc0.fontCount();
}

EBAPI int eb_load_lang_resources(int lang) {
  BString font_root = BString(getenv("HOME")) + BString("/sys/fonts/");
  int ret = -1;
  if (lang == 4) {
    BString font = font_root + BString("misaki_gothic_w.ttf");
    int min_ret = eb_load_font(font.c_str());
    if (!min_ret)
      min_ret = eb_font_by_name("misaki_gothic_w", 8, 8);

    font = font_root + BString("k8x12w.ttf");
    int k_ret = eb_load_font(font.c_str());
    if (!k_ret)
      k_ret = eb_font_by_name("k8x12w", 8, 12);

    ret = k_ret != -1 ? k_ret : min_ret;
  } else
    ret = 0;

  return ret;
}

EBAPI int eb_pos_x(void) {
  return sc0.c_x();
}

EBAPI int eb_pos_y(void) {
  return sc0.c_y();
}

EBAPI unsigned int eb_char_get(int x, int y) {
  return (x < 0 || y < 0 || x >=sc0.getWidth() || y >=sc0.getHeight()) ? 0 : sc0.vpeek(x, y);
}

EBAPI void eb_char_set(int x, int y, unsigned int c) {
  if (check_param(x, 0, sc0.getWidth() - 1) ||
      check_param(y, 0, sc0.getHeight() - 1))
    return;
  sc0.write(x, y, c);
}

EBAPI int eb_cscroll(int x1, int y1, int x2, int y2, int d) {
  if (x1 < 0 || y1 < 0 || x2 < x1 || y2 < y1 ||
      x2 >= sc0.getWidth() ||
      y2 >= sc0.getHeight()) {
    err = ERR_VALUE;
    return -1;
  }
  if (d < 0 || d > 3)
    d = 0;
  sc0.cscroll(x1, y1, x2 - x1 + 1, y2 - y1 + 1, d);
  return 0;
}

EBAPI int eb_csize_height(void) {
  return sc0.getHeight();
}

EBAPI int eb_csize_width(void) {
  return sc0.getWidth();
}

EBAPI int eb_getch(void) {
  return c_getch();
}

EBAPI int eb_last_key_event(void) {
  keyinfo ki;
  ki.kevt = ps2last();
  return ki.value;
}

EBAPI void eb_putch(int c) {
    c_putch(c);
}

EBAPI void eb_clrtoeol(void) {
    sc0.clerLine(sc0.c_y(), sc0.c_x());
}

EBAPI void eb_cls(void) {
    sc0.cls();
}

EBAPI void eb_puts(const char *s) {
    c_puts(s);
}

EBAPI void eb_show_cursor(int enable) {
    sc0.show_curs(enable);
}

EBAPI void eb_enable_scrolling(int enable) {
    sc0.setScroll(enable);
}

EBAPI void eb_enable_escape_codes(int enable) {
}

EBAPI void eb_enable_ansi_mode(int enable) {
    screen_putch_enable_ansi_mode = !!enable;
}

EBAPI int eb_kbhit(void) {
    return c_kbhit();
}

EBAPI int eb_term_getch(void) {
  return sc0.term_getch();
}

EBAPI void eb_term_putch(char c) {
  sc0.term_putch(c);
}

EBAPI char *eb_screened_get_line(void) {
  uint8_t rc = sc0.edit();
  if (!rc)
    return NULL;

  char *textline = (char *)sc0.getText();
  if (strlen(textline) == 0) {
    free(textline);
    return NULL;
  } else {
    tlimR(textline);
    return textline;
  }
}

EBAPI void eb_add_output_filter(int (*filter)(int c, void *userdata), void *userdata) {
  sc0.add_output_filter(filter, userdata);
}

EBAPI void eb_add_input_filter(int (*filter)(int c, void *userdata), void *userdata) {
  sc0.add_input_filter(filter, userdata);
}
