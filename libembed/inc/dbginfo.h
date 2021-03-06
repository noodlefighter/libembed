/**
 * @file
 * @brief   Debug Info Interface
 * @details fork from  https://github.com/tmk/tmk_keyboard
 * @internal
 * @par Modification history
 * - 1.00	2016-11-27 noodlefigher
 * @endinternal
 */

/* Copyright 2012 Jun Wako <wakojun@gmail.com> */
/* Very basic dbginfo functions, intended to be used with usb_debug_only.c
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __DBGINFO_H__
#define __DBGINFO_H__

#include "libem_common.h"

#if(LIBEMBED_CFG_DEBUG_INFO_ENABLE)

#ifdef __GNUC__
#define dbginfo(...)                  printf(__VA_ARGS__)
#define dbginfoln(...)                printf("\r\n" __VA_ARGS__)
#else
#define dbginfo                       printf
#define dbginfoln                     printf("\r\n");  printf
#endif

/* decimal */
#define dbginfo_dec(i)                printf("%u", i)
#define dbginfo_decs(i)               printf("%d", i)
/* hex */
#define dbginfo_hex4(i)               printf("%X", i)
#define dbginfo_hex8(i)               printf("%02X", i)
#define dbginfo_hex16(i)              printf("%04X", i)
#define dbginfo_hex32(i)              printf("%08lX", i)
/* binary */
#define dbginfo_bin4(i)               printf("%04b", i)
#define dbginfo_bin8(i)               printf("%08b", i)
#define dbginfo_bin16(i)              printf("%016b", i)
#define dbginfo_bin32(i)              printf("%032lb", i)
#define dbginfo_bin_reverse8(i)       printf("%08b", bitrev(i))
#define dbginfo_bin_reverse16(i)      printf("%016b", bitrev16(i))
#define dbginfo_bin_reverse32(i)      printf("%032lb", bitrev32(i))
/* dbginfo value utility */
#define dbginfo_val_dec(v)            printf(#v ": %u\n", v)
#define dbginfo_val_decs(v)           printf(#v ": %d\n", v)
#define dbginfo_val_hex8(v)           printf(#v ": %X\n", v)
#define dbginfo_val_hex16(v)          printf(#v ": %02X\n", v)
#define dbginfo_val_hex32(v)          printf(#v ": %04lX\n", v)
#define dbginfo_val_bin8(v)           printf(#v ": %08b\n", v)
#define dbginfo_val_bin16(v)          printf(#v ": %016b\n", v)
#define dbginfo_val_bin32(v)          printf(#v ": %032lb\n", v)
#define dbginfo_val_bin_reverse8(v)   printf(#v ": %08b\n", bitrev(v))
#define dbginfo_val_bin_reverse16(v)  printf(#v ": %016b\n", bitrev16(v))
#define dbginfo_val_bin_reverse32(v)  printf(#v ": %032lb\n", bitrev32(v))

#else   /* LIBEMBED_CFG_DEBUG_INFO_ENABLE */

#ifdef __GNUC__
#define dbginfo(...)                  ((void)0)
#define dbginfoln(...)                ((void)0)
#else
#define dbginfo                       (void)
#define dbginfoln                     (void)
#endif

#define dbginfo_dec(data)             ((void)0)
#define dbginfo_decs(data)            ((void)0)
#define dbginfo_hex4(data)            ((void)0)
#define dbginfo_hex8(data)            ((void)0)
#define dbginfo_hex16(data)           ((void)0)
#define dbginfo_hex32(data)           ((void)0)
#define dbginfo_bin4(data)            ((void)0)
#define dbginfo_bin8(data)            ((void)0)
#define dbginfo_bin16(data)           ((void)0)
#define dbginfo_bin32(data)           ((void)0)
#define dbginfo_bin_reverse8(data)    ((void)0)
#define dbginfo_bin_reverse16(data)   ((void)0)
#define dbginfo_bin_reverse32(data)   ((void)0)
#define dbginfo_val_dec(v)            ((void)0)
#define dbginfo_val_decs(v)           ((void)0)
#define dbginfo_val_hex8(v)           ((void)0)
#define dbginfo_val_hex16(v)          ((void)0)
#define dbginfo_val_hex32(v)          ((void)0)
#define dbginfo_val_bin8(v)           ((void)0)
#define dbginfo_val_bin16(v)          ((void)0)
#define dbginfo_val_bin32(v)          ((void)0)
#define dbginfo_val_bin_reverse8(v)   ((void)0)
#define dbginfo_val_bin_reverse16(v)  ((void)0)
#define dbginfo_val_bin_reverse32(v)  ((void)0)

#endif  /* LIBEMBED_CFG_DEBUG_INFO_ENABLE */

#endif  /* __DBG_INFO_H__ */
