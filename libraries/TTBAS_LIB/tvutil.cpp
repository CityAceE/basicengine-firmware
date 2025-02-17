//
// NTSC TV表示デバイスの管理
// 作成日 2017/04/11 by たま吉さん
// 修正日 2017/04/13, 横文字数算出ミス修正(48MHz対応)
// 修正日 2017/04/15, 行挿入の追加
// 修正日 2017/04/17, bitmap表示処理の追加
// 修正日 2017/04/18, cscroll,gscroll表示処理の追加
// 修正日 2017/04/25, gscrollの機能修正, gpeek,ginpの追加
// 修正日 2017/04/28, tv_NTSC_adjustの追加
// 修正日 2017/05/19, tv_getFontAdr()の追加
// 修正日 2017/05/30, SPI2(PA7 => PB15)に変更
// 修正日 2017/06/14, SPI2時のPB13ピンのHIGH設定対策
// 修正日 2017/07/25, tv_init()の追加
// 修正日 2017/07/29, スクリーンモード変更対応

#include "../../ttbasic/ttconfig.h"

#include <stdint.h>
#include <string.h>
#include "../../ttbasic/video.h"
#include "../../ttbasic/graphics.h"
#include "tTVscreen.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <fonts.h>

struct ttf_font_t {
  const uint8_t *font;
  const char *name;
};

struct font_t {
  struct ttf_font_t ttf_font;
  int w, h;
  stbtt_fontinfo ttf;
};

static std::vector<struct ttf_font_t> ttf_fonts;
static std::vector<struct font_t> fonts;

struct font_t *tvfont;

uint16_t c_width;   // Screen horizontal characters
uint16_t c_height;  // Screen vertical characters
uint16_t f_width;   // Font width (pixels)
uint16_t f_height;  // Font height (pixels)
uint16_t g_width;   // Screen horizontal pixels
uint16_t g_height;  // Screen vertical pixels
uint16_t win_x, win_y, win_width, win_height;
uint16_t win_c_width, win_c_height;

pixel_t fg_color = (pixel_t)-1;
pixel_t bg_color = (pixel_t)0xff000000;
pixel_t cursor_color = (pixel_t)0x92;  // XXX: wrong on 32-bit colorspaces
static pixel_t gradient[256];  // gradient from background to foreground color, for TTF rendering

uint16_t gcurs_x = 0;
uint16_t gcurs_y = 0;

#define UNIMAP_SIZE 0x30000

struct unimap {
  uint8_t *bitmap;
  uint8_t w, h;
  int off_x, off_y;
};
struct unimap *unimap = NULL;

int tv_font_count(void) {
  return fonts.size();
}

const char *tv_font_info(int idx, int *w, int *h) {
  if (w)
    *w = fonts[idx].w;
  if (h)
    *h = fonts[idx].h;

  return fonts[idx].ttf_font.name;
}

bool tv_have_font(const char *name) {
  for (auto &i : ttf_fonts) {
    if (!strcmp(i.name, name))
      return true;
  }
  return false;
}

// フォント利用設定
bool tv_fontInit(const char *name, int w, int h) {
  // allocate/reset conversion map
  if (!unimap)
    unimap = (struct unimap *)calloc(1, UNIMAP_SIZE * sizeof(*unimap));
  else {
    for (int i = 0; i < UNIMAP_SIZE; ++i) {
      free(unimap[i].bitmap);
    }
    memset(unimap, 0, UNIMAP_SIZE * sizeof(*unimap));
  }

  // do we have this font already?
  bool must_init = true;
  for (auto &i : fonts) {
    if (!strcmp(i.ttf_font.name, name) && i.w == w && i.h == h) {
      must_init = false;
      tvfont = &i;
      break;
    }
  }

  if (must_init) {
    ttf_font_t *ttf_font = NULL;
    for (auto &i : ttf_fonts) {
      if (!strcmp(i.name, name)) {
        ttf_font = &i;
        break;
      }
    }

    if (!ttf_font)
      return false;

    font_t new_font = { *ttf_font, w, h, };
    stbtt_InitFont(&new_font.ttf, ttf_font->font, stbtt_GetFontOffsetForIndex(ttf_font->font, 0));
    fonts.push_back(new_font);
    tvfont = &fonts.back();
  }

  f_height = h;
  f_width = w;

  // Screen dimensions, characters
  c_width = g_width / f_width;
  c_height = g_height / f_height;
  win_c_width = win_width / f_width;
  win_c_height = win_height / f_height;

  return true;
}

int tv_addFont(const uint8_t *font, const char *name) {
  if (tv_have_font(name))
    return -1;
  if (stbtt_GetNumberOfFonts(font) < 1)
    return -2;

  ttf_font_t ttf_font = { font, strdup(name) };
  ttf_fonts.push_back(ttf_font);

  return 0;
}

void tv_setFontByIndex(int idx) {
  if (idx < fonts.size()) {
    f_width = fonts[idx].w;
    f_height = fonts[idx].h;
    tv_fontInit(fonts[idx].ttf_font.name, f_width, f_height);
  }
}

bool tv_setFontByName(const char *name, int w, int h) {
  return tv_fontInit(name, w, h);
}

int tv_current_font_index(void) {
  for (int i = 0; i < fonts.size(); ++i) {
    if (tvfont == &fonts[i])
      return i;
  }
  return -1;
}

int colmem_fg_x, colmem_fg_y;
int colmem_bg_x, colmem_bg_y;

//
// Default setting for NTSC display
//
void tv_init(int16_t ajst, uint8_t vmode) {
  g_width = vs23.width();    // Horizontal pixel number
  g_height = vs23.height();  // Number of vertical pixels

  win_x = 0;
  win_y = 0;
  win_width = g_width;
  win_height = g_height;

  if (fonts.size() == 0) {
    // populate with built-in fonts to make sure we always have the fallbacks
    for (int i = 0; i < NUM_FONTS; ++i) {
      f_width = builtin_fonts[i].w;
      f_height = builtin_fonts[i].h;
      tv_addFont(builtin_fonts[i].data, builtin_fonts[i].name);
      tv_fontInit(builtin_fonts[i].name, f_width, f_height);
    }
  }
}

void tv_reinit() {
  vs23.reset();
  tv_window_reset();
}

void tv_window_set(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  win_x = x * f_width;
  win_y = y * f_height;
  win_width = w * f_width;
  win_height = h * f_height;
  win_c_width = win_width / f_width;
  win_c_height = win_height / f_height;
}

void tv_window_reset() {
  win_x = 0;
  win_y = 0;
  win_width = g_width;
  win_height = g_height;
  win_c_width = win_width / f_width;
  win_c_height = win_height / f_height;
}

void tv_window_get(int &x, int &y, int &w, int &h) {
  x = win_x / f_width;
  y = win_y / f_height;
  w = win_width / f_width;
  h = win_height / f_height;
}

//
// End of NTSC display
//
void tv_end() {
}

const uint8_t *tv_getFontAdr() {
  return tvfont->ttf_font.font;
}

// Screen characters sideways
uint16_t tv_get_cwidth() {
  return c_width;
}

// Screen characters downwards
uint16_t tv_get_cheight() {
  return c_height;
}

uint16_t tv_get_win_cwidth() {
  return win_c_width;
}

uint16_t tv_get_win_cheight() {
  return win_c_height;
}

// Screen graphic horizontal pixels
uint16_t tv_get_gwidth() {
  return g_width;
}

// Screen graphic vertical pixels
uint16_t tv_get_gheight() {
  return g_height;
}

//
// Cursor display
//
void tv_drawCurs(uint16_t x, uint16_t y) {
  pixel_t pix[f_width];
  for (int i = 0; i < f_width; ++i)
    pix[i] = cursor_color;

  for (int i = 0; i < f_height; ++i) {
    pixel_t *address =
            vs23.pixelAddr(win_x + x * f_width, win_y + y * f_height + i);
    vs23.setPixels(address, pix, f_width);
  }
}

void tv_setcolor(pixel_t fc, pixel_t bc) {
  fg_color = fc;
  bg_color = bc;

  uint8_t fr, fg, fb, fa;
  vs23.rgbaFromColor(fc, fr, fg, fb, fa);
  uint8_t br, bg, bb, ba;
  vs23.rgbaFromColor(bc, br, bg, bb, ba);

  double scale_r = (br - fr) / 255.0;
  double scale_g = (bg - fg) / 255.0;
  double scale_b = (bb - fb) / 255.0;

  for (int i = 0; i < 256; ++i) {
    gradient[i] = csp.colorFromRgb(br - i * scale_r, bg - i * scale_g, bb - i * scale_b);
  }
}

void tv_flipcolors() {
  tv_setcolor(bg_color, fg_color);
}

//
// Display character
//
static void ICACHE_RAM_ATTR tv_write_px(uint16_t x, uint16_t y, utf8_int32_t c) {
  int w = f_width, h = f_height;
  int off_x = 0, off_y = 0;

  if (c < 0 || c >= UNIMAP_SIZE)
    c = 0xfffd;

  if (!unimap[c].bitmap && c != ' ') {
    float scale = stbtt_ScaleForPixelHeight(&tvfont->ttf, f_height);

    int ascent_i, descent_i, lineGap;
    float ascent, descent;

    stbtt_GetFontVMetrics(&tvfont->ttf, &ascent_i, &descent_i, &lineGap);

    // Assigning the scaled ascent/descent values to the integer variables
    // and using them below causes an off-by-one on Win32 (and only Win32,
    // i686-w64-mingw32-gcc (GCC) 12-win32), observable in characters being
    // shifted one pixel down.
    // The issue magically disappears when printing the value as debug
    // output, without any other change to the code.
    // This is either a bug or undefined behavior I'm not aware of.
    ascent = (float)ascent_i * scale;
    descent = (float)descent_i * scale;

    int glyph_idx = stbtt_FindGlyphIndex(&tvfont->ttf, c);
    if (glyph_idx != 0) {
      unimap[c].bitmap = stbtt_GetCodepointBitmap(&tvfont->ttf, 0, scale, c, &w,
                                                  &h, &off_x, &off_y);
      unimap[c].w = w;
      unimap[c].h = h;
      unimap[c].off_x = off_x;
      unimap[c].off_y = off_y + f_height + descent;
    } else {
      for (auto i : fonts) {
        if (i.h == f_height && i.w == f_width &&
            stbtt_FindGlyphIndex(&i.ttf, c) != 0) {
          scale = stbtt_ScaleForPixelHeight(&i.ttf, f_height);
          stbtt_GetFontVMetrics(&i.ttf, &ascent_i, &descent_i, &lineGap);
          ascent = (float)ascent_i * scale;
          descent = (float)descent_i * scale;
          unimap[c].bitmap = stbtt_GetCodepointBitmap(&i.ttf, 0, scale, c, &w,
                                                      &h, &off_x, &off_y);
          unimap[c].w = w;
          unimap[c].h = h;
          unimap[c].off_x = off_x;
          unimap[c].off_y = off_y + f_height + descent;
          break;
        }
      }
      if (!unimap[c].bitmap) {
        unimap[c].w = unimap[c].h = 0;
      }
    }
  }

  const uint8_t *chp = unimap[c].bitmap;
  w = unimap[c].w;
  h = unimap[c].h;
  off_x = unimap[c].off_x;
  off_y = unimap[c].off_y;

  for (int i = 0; i < f_height; ++i) {
    pixel_t pix[f_width];
    for (int j = 0; j < f_width; ++j) {
      if (i < off_y || j < off_x || !w || !h)
        pix[j] = bg_color;
      else if (i >= off_y + h || j >= off_x + w)
        pix[j] = bg_color;
      else
        pix[j] = gradient[chp[(j - off_x) + (i - off_y) * w]];
    }
    pixel_t *address = vs23.pixelAddr(x, y + i);
    vs23.setPixels(address, pix, f_width);
  }
}

void ICACHE_RAM_ATTR tv_write(uint16_t x, uint16_t y, utf8_int32_t c) {
  tv_write_px(win_x + x * f_width, win_y + y * f_height, c);
}

void ICACHE_RAM_ATTR tv_write_color(uint16_t x, uint16_t y, utf8_int32_t c,
                                    pixel_t fg, pixel_t bg) {
  if (fg != fg_color || bg != bg_color) {
    pixel_t sfg = fg_color;
    pixel_t sbg = bg_color;
    tv_setcolor(fg, bg);
    tv_write(x, y, c);
    tv_setcolor(sfg, sbg);
  } else
    tv_write(x, y, c);
}

//
// Clear screen
//
void tv_cls() {
  // If the window covers the entire screen, erase it in one go. This helps
  // avoid leaving behind breadcrumbs on screens where the font dimensions
  // are not evenly divisible by the screen dimensions.
  if (win_x == 0 && win_y == 0 && win_width + f_width > g_width &&
      win_height + f_height > g_height) {
    vs23.fillRect(0, 0, g_width, g_height, bg_color);
  } else {
    for (int i = 0; i < win_c_height; ++i) {
      tv_clerLine(i);
    }
  }
}

//
// Clear one specified line
//
void tv_clerLine(uint16_t l, int from) {
  int left = win_x + from * f_width;
  int top = win_y + l * f_height;
  int width = win_width - from * f_width;
  vs23.fillRect(left, top, left + width, top + f_height, bg_color);
}

//
// Insert one line at specified line (downward scroll)
//
void tv_insLine(uint16_t l) {
  if (l > win_c_height - 1) {
    return;
  } else if (l == win_c_height - 1) {
    tv_clerLine(l);
  } else {
#ifdef SINGLE_BLIT
    vs23.blitRect(win_x, win_y + f_height * l,
                  win_x, win_y + f_height * (l + 1),
                  win_width, (win_c_height - 1 - l) * f_height);
#else
    vs23.blitRect(win_x, win_y + f_height * l,
                  win_x, win_y + f_height * (l + 1),
                  win_width / 2, win_height - f_height - l * f_height);
    vs23.blitRect(win_x + win_width / 2, win_y + f_height * l,
                  win_x + win_width / 2, win_y + f_height * (l + 1),
                  win_width / 2, win_height - f_height - l * f_height);
#endif
    tv_clerLine(l);
  }
}

// Screen scroll up for one line
void tv_scroll_up() {
#ifdef SINGLE_BLIT
  vs23.blitRect(win_x, win_y + f_height,
                win_x, win_y,
                win_width, (win_c_height - 1) * f_height);
#else
  vs23.blitRect(win_x, win_y + f_height,
                win_x, win_y,
                win_width/2, win_height - f_height);
  vs23.blitRect(win_x + win_width / 2, win_y + f_height,
                win_x + win_width / 2, win_y,
                win_width/2, win_height-f_height);
#endif
  tv_clerLine(win_c_height - 1);
}

// Screen scroll down for one line
void tv_scroll_down() {
#ifdef SINGLE_BLIT
  vs23.blitRect(win_x, win_y,
                win_x, win_y + f_height,
                win_width, (win_c_height - 1) * f_height);
#else
  vs23.blitRect(win_x, win_y,
                win_x, win_y + f_height,
                win_width/2, win_height-f_height);
  vs23.blitRect(win_x + win_width / 2, win_y,
                win_x + win_width / 2, win_y + f_height,
                win_width/2, win_height-f_height);
#endif
  tv_clerLine(0);
}

// Point drawing
void tv_pset(int16_t x, int16_t y, pixel_t c) {
  vs23.setPixel(x, y, c);
}

// Draw a line
void tv_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, pixel_t c) {
  gfx.drawLine(x1, y1, x2, y2, c);
}

// Drawing a circle
void tv_circle(int16_t x, int16_t y, int16_t r, pixel_t c, int f) {
  gfx.drawCircle(x, y, r, c, f);
}

// Draw a square
void tv_rect(int16_t x, int16_t y, int16_t w, int16_t h, pixel_t c, int f) {
  gfx.drawRect(x, y, w, h, c, f);
}

void tv_set_gcursor(uint16_t x, uint16_t y) {
  gcurs_x = x;
  gcurs_y = y;
}

void tv_write(utf8_int32_t c) {
  if (gcurs_x < g_width - f_width)
    tv_write_px(gcurs_x, gcurs_y, c);
  gcurs_x += f_width;
}

// Graphic horizontal scroll
void tv_gscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode) {
  pixel_t col_black = (pixel_t)0;
  switch (mode) {
    case 0:	// up
      vs23.blitRect(x,	       y + 1, x,         y, w / 2, h - 1);
      vs23.blitRect(x + w / 2, y + 1, x + w / 2, y, w / 2, h - 1);
      gfx.drawLine(x, y + h - 1, x + w - 1, y + h - 1, col_black);
      break;
    case 1:	// down
      vs23.blitRect(x,         y, x,         y + 1, w / 2, h - 1);
      vs23.blitRect(x + w / 2, y, x + w / 2, y + 1, w / 2, h - 1);
      gfx.drawLine(x, y, x + w - 1, y, col_black);
      break;
    case 2:	// left
      vs23.blitRect(x + 1,     y, x,             y, w / 2 - 1, h);
      vs23.blitRect(x + w / 2, y, x + w / 2 - 1, y, w / 2,     h);
      gfx.drawLine(x + w - 1, y, x + w - 1, y + h - 1, col_black);
      break;
    case 3:	// right
      vs23.blitRect(x + w / 2, y, x + w / 2 + 1, y, w / 2 - 1, h);
      vs23.blitRect(x,         y, x + 1,         y, w / 2,     h);
      gfx.drawLine(x, y, x, y + h - 1, col_black);
      break;
  }
}

