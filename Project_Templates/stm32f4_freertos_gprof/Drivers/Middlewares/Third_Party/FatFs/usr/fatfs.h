/**
  ******************************************************************************
  * File Name          : fatfs.h
  * Date               : 05/06/2015 12:55:50
  * Description        : Header for fatfs applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h" /* defines USBH_Driver as external */
#include "user_diskio.h" /* defines USER_Driver as external */

/* USER CODE BEGIN Includes */

#define STRLEN 255
#define N_ARGS    2
/* USER CODE END Includes */

extern uint8_t retUSBH; /* Return value for USBH */
extern char USBH_Path[4]; /* USBH logical drive path */
extern uint8_t retUSER; /* Return value for USER */
extern char USER_Path[4]; /* USER logical drive path */

extern FATFS USBH_fatfs;
extern char str[STRLEN];
extern char line[STRLEN];
extern char *args[N_ARGS];

void MX_FATFS_Init(void);

/* USER CODE BEGIN Prototypes */
void Print_Help(void);
void show_res (char *mesg, FRESULT rc);
void show_dir(char *dirname);
void write_log(char *fname);
void type_file(char *fname);
void delete_file(char *fname);
void scan_args(char *buf, char *args[]);
void FATFS_CMD(void);
/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
