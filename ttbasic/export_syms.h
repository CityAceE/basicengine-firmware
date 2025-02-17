// unistd time
R(usleep, eb_udelay)

// stdio string formatting
R(printf, c_printf)
S(asprintf)
S(snprintf)
S(sprintf)
S(vasprintf)
S(vsnprintf)
S(vsprintf)
S(sscanf)

// wrapper that jumps back to BASIC prompt
R(_exit, be__exit)
R(exit, be_exit)
R(atexit, be_atexit)

// stdlib memory allocation
S(malloc)
S(free)
S(realloc)
S(calloc)

// eb bg
S(eb_bg_set_size)
S(eb_bg_set_pattern)
S(eb_bg_set_tile_size)
S(eb_bg_set_window)
S(eb_bg_set_priority)
S(eb_bg_enable)
S(eb_bg_disable)
S(eb_bg_off)
S(eb_bg_load)
S(eb_bg_save)
S(eb_bg_move)

S(eb_sprite_off)
S(eb_sprite_set_pattern)
S(eb_sprite_set_size)
S(eb_sprite_enable)
S(eb_sprite_disable)
S(eb_sprite_set_key)
S(eb_sprite_set_priority)
S(eb_sprite_set_frame)
S(eb_sprite_set_opacity)
S(eb_sprite_set_angle)
S(eb_sprite_set_scale_x)
S(eb_sprite_set_scale_y)
S(eb_sprite_set_alpha)
S(eb_sprite_reload)
S(eb_sprite_move)

S(eb_bg_get_tiles)
S(eb_bg_map_tile)
S(eb_bg_set_tiles)
S(eb_bg_set_tile)

S(eb_frameskip)

S(eb_sprite_tile_collision)
S(eb_sprite_collision)
S(eb_sprite_enabled)
S(eb_sprite_x)
S(eb_sprite_y)
S(eb_sprite_w)
S(eb_sprite_h)

S(eb_bg_x)
S(eb_bg_y)
S(eb_bg_win_x)
S(eb_bg_win_y)
S(eb_bg_win_width)
S(eb_bg_win_height)
S(eb_bg_enabled)

S(eb_sprite_frame_x)
S(eb_sprite_frame_y)
S(eb_sprite_flip_x)
S(eb_sprite_flip_y)
S(eb_sprite_opaque)

S(eb_add_bg_layer)
S(eb_remove_bg_layer)

// eb conio
S(eb_add_input_filter)
S(eb_add_output_filter)
S(eb_char_get)
S(eb_char_set)
S(eb_clrtoeol)
S(eb_cls)
S(eb_cscroll)
S(eb_csize_height)
S(eb_csize_width)
S(eb_enable_escape_codes)
S(eb_enable_scrolling)
S(eb_font)
S(eb_font_by_name)
S(eb_font_count)
S(eb_font_info)
S(eb_getch)
S(eb_kbhit)
S(eb_last_key_event)
S(eb_load_font)
S(eb_locate)
S(eb_mode_from_size)
S(eb_mode_size)
S(eb_num_modes)
S(eb_pos_x)
S(eb_pos_y)
S(eb_putch)
S(eb_puts)
S(eb_screened_get_line)
S(eb_show_cursor)
S(eb_show_cursor)
S(eb_term_getch)
S(eb_term_putch)
S(eb_window)
S(eb_window_off)

// eb_img
S(eb_load_image)
S(eb_load_image_to)

// eb_input
S(eb_inkey)
S(eb_key_state)
S(eb_mouse_abs_x)
S(eb_mouse_abs_y)
S(eb_mouse_button)
S(eb_mouse_buttons)
S(eb_mouse_rel_x)
S(eb_mouse_rel_y)
S(eb_mouse_warp)
S(eb_mouse_wheel)
S(eb_pad_state)

// eb_io
S(eb_gpio_get_pin)
S(eb_gpio_set_pin)
S(eb_gpio_set_pin_mode)
S(eb_i2c_read)
S(eb_i2c_write)
S(eb_spi_set_bit_order)
S(eb_spi_set_freq)
S(eb_spi_set_mode)
S(eb_spi_transfer)
S(eb_spi_write)

// eb_video
S(eb_blit)
S(eb_blit_alpha)
S(eb_border)
S(eb_circle)
S(eb_color)
S(eb_cursor_color)
S(eb_frame)
S(eb_get_bg_color)
S(eb_get_fg_color)
S(eb_gscroll)
S(eb_line)
S(eb_palette)
S(eb_point)
S(eb_pset)
S(eb_psize_height)
S(eb_psize_lastline)
S(eb_psize_width)
S(eb_rect)
S(eb_rgb)
S(eb_rgb_from_indexed)
S(eb_rgb_indexed)
S(eb_screen)
S(eb_vsync)

// eb_sys
S(eb_process_events)
S(eb_process_events_check)
S(eb_process_events_wait)
S(eb_set_cpu_speed)
S(eb_tick)
S(eb_utick)
S(eb_wait)

// eb_basic
S(eb_add_command)
S(eb_add_numfun)
S(eb_add_strfun)
S(eb_kwtbl)
S(eb_kwtbl_size)
S(eb_new_basic_context)
S(eb_delete_basic_context)
S(eb_exec_basic)
S(eb_set_error)
S(eb_get_error)

// eb_file
S(eb_file_exists)
S(eb_file_size)
S(eb_is_directory)
S(eb_is_file)
S(eb_unzip)

S(eb_theme_color)

// eb_native
S(eb_tcc_new)
S(eb_tcc_initialize_symbols)
S(eb_tcc_link)

// libtcc
S(tcc_new)
S(tcc_delete)
S(tcc_set_lib_path)
S(tcc_set_error_func)
S(tcc_set_options)
S(tcc_add_include_path)
S(tcc_add_sysinclude_path)
S(tcc_define_symbol)
S(tcc_undefine_symbol)
S(tcc_add_file)
S(tcc_compile_string)
S(tcc_set_output_type)
S(tcc_add_library_path)
S(tcc_add_library)
S(tcc_add_symbol)
S(tcc_output_file)
S(tcc_run)
S(tcc_relocate)
S(tcc_get_symbol)
S(tcc_have_symbol)
S(tcc_get_name)

// stdlib type conversion
S(atof)
S(atoi)
S(atol)

// string functions
#ifndef _WIN32
S(bcopy)
S(bzero)
#endif
S(memchr)
S(memcmp)
S(memcpy)
S(memmove)
#if !defined(_WIN32) && !defined(__APPLE__)
S(memrchr)
#endif
S(memset)
S(strcasecmp)
#ifdef _WIN32
// XXX: Is that a good idea?
R(strcasestr, utf8casestr)
#else
S(strcasestr)
#endif
S(strcat)
S(strcmp)
S(strcoll)
S(strcpy)
S(strcspn)
S(strdup)
S(strerror)
S(strlen)
S(strncasecmp)
S(strncat)
S(strncmp)
S(strncpy)
#ifndef _WIN32
S(strndup)
#endif
S(strpbrk)
S(strspn)
S(strtod)
S(strtol)
S(strtoll)
S(strtoul)
#ifdef __clang__
S(strchr)
S(strrchr)
S(strstr)
#else
R(strchr, __builtin_strchr)
R(strrchr, __builtin_strrchr)
R(strstr, __builtin_strstr)
#endif

// unistd/stdio file ops
S(chdir)
S(mkdir)
S(mkstemp)
S(remove)
S(rename)
S(rmdir)
S(unlink)

// dirent directory functions
#ifdef ALLWINNER_BARE_METAL
S(readdir)
#else
R(readdir, be_readdir)
#endif
S(closedir)
S(opendir)
S(getcwd)

//S(fnmatch)	in newlib, but only built for posix targets

// newlib-style errno wrapper
S(__errno)

//S(realpath)	not in newlib

// stdio stream interface
R(getc, fgetc)
S(clearerr)
S(fclose)
S(fdopen)
S(feof)
S(ferror)
S(fflush)
S(fgetc)
S(fgets)
S(fileno)
S(fopen)
S(fprintf)
S(fputc)
S(fputs)
S(fread)
S(freopen)
S(fscanf)
S(fseek)
S(ftell)
S(fwrite)
S(setvbuf)
S(ungetc)
S(vfprintf)

#ifndef ALLWINNER_BARE_METAL	// XXX: should probably be "if not newlib"
S(putchar)
S(puts)
#endif

// stdio streams
S(__getreent)

// unistd file I/O
S(close)
//S(creat)	not in newlib
S(read)
S(write)
S(lseek)	// assumes "whence" defines to be identical across platforms
#ifdef ALLWINNER_BARE_METAL
S(open)
S(stat)
S(fstat)
//S(lstat)	not in newlib
#else
R(stat, _native_stat)
R(fstat, _native_fstat)
//R(lstat, _native_lstat)
#endif

// stdlib environment
S(getenv)
S(putenv)
#ifndef _WIN32
S(setenv)
#endif

// stdlib miscellaneous
S(abort)  // XXX: does this do anything useful?
#ifdef __clang__
S(abs)
#else
R(abs, __builtin_abs)
#endif
S(labs)
S(qsort)
S(rand)
#ifndef _WIN32
S(random)
#endif
S(srand)

// unistd getopt
S(getopt)
R(optarg, &optarg)
R(optind, &optind)

// unistd misc
#if !defined(_WIN32) && !defined(__APPLE__)
R(environ, &environ)
#endif

// setjmp
#ifdef _WIN32
R(setjmp, _setjmp)
#else
S(setjmp)
#endif
S(longjmp)

// ctype
S(isdigit)
S(isalnum)
S(isalpha)
S(isblank)
S(iscntrl)
S(isgraph)
S(islower)
S(isprint)
S(ispunct)
S(isspace)
S(isupper)
S(isxdigit)
S(toupper)
S(tolower)

// stdlib multi-byte functions
S(mbtowc)
S(towlower)
#ifndef _WIN32
S(wcwidth)
#endif

// math
S(acos)
S(acosf)
S(acosh)
S(acoshf)
S(asin)
S(asinf)
S(asinh)
S(asinhf)
S(atan)
S(atan2)
S(atan2f)
S(atanf)
S(atanh)
S(atanhf)
S(cbrt)
S(cbrtf)
S(ceil)
S(ceilf)
S(cos)
S(cosf)
S(cosh)
S(coshf)
S(erf)
S(erfc)
S(erfcf)
S(erff)
S(exp)
S(expf)
S(fabs)
S(fabsf)
S(floor)
S(floorf)
S(fmod)
S(fmodf)
S(frexp)
S(frexpf)
S(j0)
#if !defined(__UCLIBC__) && !defined(_WIN32) && !defined(__APPLE__)
S(j0f)
#endif
S(j1)
#if !defined(__UCLIBC__) && !defined(_WIN32) && !defined(__APPLE__)
S(j1f)
#endif
S(ldexp)
S(ldexpf)
S(log)
S(log10)
S(log10f)
S(logf)
S(pow)
S(powf)
S(round)
S(roundf)
S(sin)
S(sinf)
S(sinh)
S(sinhf)
S(sqrt)
S(sqrtf)
S(tan)
S(tanf)
S(tanh)
S(tanhf)
S(y0)
#if !defined(__UCLIBC__) && !defined(_WIN32) && !defined(__APPLE__)
S(y0f)
#endif
S(y1)
#if !defined(__UCLIBC__) && !defined(_WIN32) && !defined(__APPLE__)
S(y1f)
#endif

// time
S(time)
S(gettimeofday)
S(localtime)

#ifdef __x86_64__
S(__va_start)
S(__va_arg)
S(__fixunsxfdi)
S(__fixunssfdi)
S(__fixunsdfdi)
S(__popcountdi2)
#endif

#ifdef __arm__
S(__aeabi_d2lz)
S(__aeabi_d2ulz)
S(__aeabi_dadd)
S(__aeabi_drsub)
S(__aeabi_dsub)
S(__aeabi_f2d)
S(__aeabi_f2lz)
S(__aeabi_f2ulz)
S(__aeabi_i2d)
S(__aeabi_idiv)
S(__aeabi_idiv0)
S(__aeabi_idivmod)
S(__aeabi_l2d)
S(__aeabi_l2f)
S(__aeabi_lasr)
S(__aeabi_ldiv0)
S(__aeabi_ldivmod)
S(__aeabi_llsl)
S(__aeabi_llsr)
S(__aeabi_memcpy)
S(__aeabi_memcpy4)
S(__aeabi_memcpy8)
S(__aeabi_memmove)
S(__aeabi_memmove4)
S(__aeabi_memmove8)
S(__aeabi_memset)
S(__aeabi_ui2d)
S(__aeabi_uidiv)
S(__aeabi_uidivmod)
S(__aeabi_ul2d)
S(__aeabi_ul2f)
S(__aeabi_uldivmod)
S(__aeabi_unwind_cpp_pr0)
S(__aeabi_unwind_cpp_pr1)
S(__aeabi_unwind_cpp_pr2)
#endif

#ifdef __i386__
S(__fixsfdi)
#endif

// sts_mixer
S(sts_mixer_init)
S(sts_mixer_shutdown)
S(sts_mixer_get_active_voices)
S(sts_mixer_play_sample)
S(sts_mixer_play_stream)
S(sts_mixer_stop_voice)
S(sts_mixer_stop_sample)
S(sts_mixer_stop_stream)
S(sts_mixer_mix_audio)
S(eb_get_mixer)

// stb_image
S(stbi_load_from_memory)
S(stbi_load_from_callbacks)
S(stbi_load)
S(stbi_load_from_file)
S(stbi_load_gif_from_memory)
S(stbi_load_16_from_memory)
S(stbi_load_16_from_callbacks)
S(stbi_load_16)
S(stbi_load_from_file_16)
S(stbi_is_hdr_from_callbacks)
S(stbi_is_hdr_from_memory)
S(stbi_is_hdr)
S(stbi_is_hdr_from_file)
S(stbi_failure_reason)
S(stbi_image_free)
S(stbi_info_from_memory)
S(stbi_info_from_callbacks)
S(stbi_is_16_bit_from_memory)
S(stbi_is_16_bit_from_callbacks)
S(stbi_info)
S(stbi_info_from_file)
S(stbi_is_16_bit)
S(stbi_is_16_bit_from_file)
S(stbi_set_unpremultiply_on_load)
S(stbi_convert_iphone_png_to_rgb)
S(stbi_set_flip_vertically_on_load)
S(stbi_zlib_decode_malloc_guesssize)
S(stbi_zlib_decode_malloc_guesssize_headerflag)
S(stbi_zlib_decode_malloc)
S(stbi_zlib_decode_buffer)
S(stbi_zlib_decode_noheader_malloc)
S(stbi_zlib_decode_noheader_buffer)

// stb_image_resize
S(stbir_resize)
S(stbir_resize_float)
S(stbir_resize_float_generic)
S(stbir_resize_region)
S(stbir_resize_subpixel)
S(stbir_resize_uint16_generic)
S(stbir_resize_uint8)
S(stbir_resize_uint8_generic)
S(stbir_resize_uint8_srgb)
S(stbir_resize_uint8_srgb_edgemode)

// stb_image_write
S(stbi_flip_vertically_on_write)
S(stbi_write_bmp)
S(stbi_write_bmp_to_func)
S(stbi_write_hdr)
S(stbi_write_hdr_to_func)
S(stbi_write_jpg)
S(stbi_write_jpg_to_func)
S(stbi_write_png)
S(stbi_write_png_to_func)
S(stbi_write_tga)
S(stbi_write_tga_to_func)

// utf8.h
S(utf8casecmp)
S(utf8cat)
S(utf8chr)
S(utf8cmp)
S(utf8cpy)
S(utf8cspn)
S(utf8dup)
S(utf8len)
S(utf8nlen)
S(utf8ncasecmp)
S(utf8ncat)
S(utf8ncmp)
S(utf8ncpy)
S(utf8ndup)
S(utf8pbrk)
S(utf8rchr)
S(utf8size)
S(utf8size_lazy)
S(utf8nsize_lazy)
S(utf8spn)
S(utf8str)
S(utf8casestr)
S(utf8valid)
S(utf8nvalid)
S(utf8makevalid)
S(utf8codepoint)
S(utf8codepointcalcsize)
S(utf8codepointsize)
S(utf8catcodepoint)
S(utf8islower)
S(utf8isupper)
S(utf8lwr)
S(utf8upr)
S(utf8lwrcodepoint)
S(utf8uprcodepoint)
S(utf8rcodepoint)
S(utf8dup_ex)
S(utf8ndup_ex)

#ifdef ALLWINNER_BARE_METAL
// allwinner-bare-metal
S(mmu_flush_dcache_range)

S(smp_start_secondary_core)
S(smp_stop_secondary_core)
S(smp_get_core_id)

#endif

// C++ new/delete
S(_ZdlPv)
#ifdef __arm__
S(_ZdlPvj)
S(_Znaj)
S(_Znwj)
#endif
#ifdef __x86_64__
S(_ZdlPvm)
S(_Znam)
S(_Znwm)
#endif
#ifdef __i386__
S(_Znaj)
S(_Znwj)
#endif
