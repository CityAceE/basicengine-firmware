// SPDX-License-Identifier: MIT
// Copyright (c) 2018 Ulrich Hecht

#include "ttconfig.h"

#if defined(ESP8266) || defined(ESP8266_NOWIFI) || defined(ESP32)
#define ESC_F "\\f"
#define LICENSE_COLOR ESC_F "44"
#define TEXT_COLOR ESC_F "ff"
#define HEADLINE_COLOR ESC_F "e2"
#define AUML "\x84"
#else
#define ESC_F "\xf4\x80\x81\xa6"	// aka 0x100066, aka \f in BASIC
#define LICENSE_COLOR ESC_F "ff8c8d"
#define TEXT_COLOR ESC_F "ffffff"
#define HEADLINE_COLOR ESC_F "0007c7"
#define AUML "ä"
#endif

#define GPL LICENSE_COLOR "(GPL)" TEXT_COLOR "\n"
#define GPLv2 LICENSE_COLOR "(GPL v2)" TEXT_COLOR "\n"
#define ISC LICENSE_COLOR "(ISC)" TEXT_COLOR "\n"
#define LGPL LICENSE_COLOR "(LGPL)" TEXT_COLOR "\n"
#define MIT LICENSE_COLOR "(MIT)" TEXT_COLOR "\n"
#define PD LICENSE_COLOR "(PD)" TEXT_COLOR "\n"
#define ZLIB LICENSE_COLOR "(zlib)" TEXT_COLOR "\n"

static const char __credits[] PROGMEM =
HEADLINE_COLOR " \bEngine BASIC" TEXT_COLOR " is based on:\n"
"* Toyoshiki TinyBASIC by Tetsuya Suzuki\n"
"  and Tamakichi-san " GPL
"  BASIC Engine port by Ulrich Hecht\n"
"* Graphics drivers by Ulrich Hecht " MIT
"* Text screen subsystem by Tamakichi-san\n"
"* PCX image loader by David Reid " PD
"* Screen allocator by Jukka Jyl" AUML "nki " PD
"* TSF wavetable synthesizer by Steve Volta,\n"
"  Bernhard Schelling and Earle Philhower " MIT
"* MML parser by Shinichiro Nakamura " MIT

#if defined(ESP8266) || defined(ESP8266_NOWIFI)
// XXX: SAM is not a part of the core system on modern platforms, but it is
// still in the demos.
"* SAM speech synthesizer by Sebastian Macke\n"
"  and Earle Philhower, based on a program\n"
"  by Don't ask Software\n"

"* I2S driver by Charles Lohr\n"
"* I2S PWM sound driver by Ulrich Hecht " MIT
"* SD card driver and file system by\n"
"  Bill Greiman " MIT
"* Unifile file system abstraction by\n"
"  Ulrich Hecht " MIT
"* PS/2 keyboard driver by Tamakichi-san\n"
"* PSX controller driver by Kevin Ahrendt " GPL
#endif

#if defined(H3)
"* allwinner-bare-metal framework by\n"
"  Charlie Smurthwaite and Ulrich Hecht " GPL
#if defined(JAILHOUSE)
"* Jailhouse hypervisor by Jan Kiszka et al. " GPLv2
#endif
#endif

#if defined(SDL)
"* SDL by Sam Latinga et al. " ZLIB
#if defined(__linux__)
"* Linux kernel by Linus Torvalds et al. "  GPLv2
"* Buildroot Linux distribution by\n"
"  Peter Korsgaard et al. " GPL
#endif
#endif

#if defined(ESP8266) || defined(ESP8266_NOWIFI)
"* \"e\" text editor by \"Mtv Europe\"\n"
"  using mcurses by Frank Meyer " GPL
#endif

"* dyncall library by Daniel Adler,\n"
"  Tassilo Philipp et al. " ISC
"* tcc by Fabrice Bellard et al. " LGPL

"* System integration and optimizations\n"
"  by Ulrich Hecht\n"
"* Typographical design by Damian Vila\n"
;
