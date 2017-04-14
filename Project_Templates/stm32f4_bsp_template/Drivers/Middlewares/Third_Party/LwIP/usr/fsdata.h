/**
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/
#ifndef __FSDATA_H__
#define __FSDATA_H__

#include "fs.h"

#define file_NULL (struct fsdata_file *) NULL
#define FS_ROOT file__STM32F4x7LED_html
#define FS_NUMFILES 7

typedef struct  {
  const struct fsdata_file *next;
  const unsigned char *name;
  const unsigned char *data;
  int len;
  u8_t http_header_included;
#if HTTPD_PRECALCULATED_CHECKSUM
  u16_t chksum_count;
  const struct fsdata_chksum *chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
}fsdata_file;

extern const fsdata_file file__STM32F4x7_files_logo_jpg[];
extern const fsdata_file file__STM32F4x7_files_ST_gif[];
extern const fsdata_file file__STM32F4x7_files_stm32_jpg[];
extern const fsdata_file file__404_html[];
extern const fsdata_file file__index_html[];
extern const fsdata_file file__STM32F4x7ADC_shtml[];
extern const fsdata_file file__STM32F4x7LED_html[];

#endif /* __FSDATA_H__ */
