/**
  ******************************************************************************
  * File Name          : fatfs.c
  * Date               : 05/06/2015 12:55:50
  * Description        : Code for fatfs applications
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

#include "fatfs.h"
#include "stm32f4_discovery_uart.h"

uint8_t retUSBH;    /* Return value for USBH */
char USBH_Path[4];  /* USBH logical drive path */
uint8_t retUSER;    /* Return value for USER */
char USER_Path[4];  /* USER logical drive path */

/* User */
FATFS USBH_fatfs;
char str[STRLEN];
char line[STRLEN];
char *args[N_ARGS];
long p2,w1;
uint8_t Buffer[512]; /* Buffer to store a complete sector */

/* USER CODE BEGIN Variables */
// Message Blocks
const char *help_msg[9]={"\r\nCommand List:\r\n",
                          "status         - disk status\r\n",
                          "dir<dir name>  - list directory/files\r\n",
                          "write<f_name>  - create file with f_name and dump some data\r\n",
                          "read<f_name>   - read file with f_name and compare dump data\r\n",
                          "log<f_name>    - write real time log of system tick value\r\n",
                          "type<f_name>   - display file content\r\n",
                          "del<f_name>    - delete file/directory\r\n",
                          "help           - print this msg\n\r",
};

void Print_Help(void)
{
  uint8_t count;

// clr_scr_rst_cur();

  for(count = 0; count<9; ++count)
  {
    uprintf(help_msg[count]);
  }
}
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBH_Path);
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USER_Path);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/* USER CODE BEGIN Application */
void show_res (char *mesg, FRESULT rc)
{
  const char *res_str =
    "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
    "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
    "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
    "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
  FRESULT i;

  for (i = 0; i != rc && *res_str; i++) {
    while (*res_str++) ;
  }
  uprintf("    %s: res=%d02 FR_%s\n", mesg, (UINT)rc, res_str);
}


void show_dir(char *dirname)
{
  DWORD clust;
  FATFS *fs;
  DWORD TotalSpace, FreeSpace;
  FRESULT res;
  DIR dir;
  FILINFO fileinfo;

  res = f_opendir(&dir, dirname);
  if (res)
  {
    show_res("Cannot open dir", res);
    return;
  }

  while((f_readdir(&dir, &fileinfo) == FR_OK) && fileinfo.fname[0])
  {
    uprintf("%c%c%c%c%c %d04/%d02/%d02 %d02:%d02 %d08  %s\n\r",
        (fileinfo.fattrib & AM_DIR) ? 'D' : '-',
        (fileinfo.fattrib & AM_RDO) ? 'R' : '-',
        (fileinfo.fattrib & AM_HID) ? 'H' : '-',
        (fileinfo.fattrib & AM_SYS) ? 'S' : '-',
        (fileinfo.fattrib & AM_ARC) ? 'A' : '-',
        (fileinfo.fdate >> 9) + 1980, (fileinfo.fdate >> 5) & 15, fileinfo.fdate & 31,
        (fileinfo.ftime >> 11), (fileinfo.ftime >> 5) & 63,
        fileinfo.fsize, &(fileinfo.fname[0]));
  }
  uprintf("%cCalculating disk space...\r\n", 0x09);
  res = f_getfree(dirname, &clust, &fs);
  if(res)
  {
    uprintf("f_getfree() failed....!");
  }

  TotalSpace = (DWORD)((fs->n_fatent - 2) * (fs->csize / 2));
  FreeSpace = (fs->free_clust) * (fs->csize / 2);
  uprintf("%c%d08 KB total disk space.\r\n", 0x09, TotalSpace);
  uprintf("%c%d08 KB available on the disk.\r\n", 0x09, FreeSpace);
}


void write_log(char *fname)
{
  FIL file;
  FRESULT res;
  UINT written;
  uint32_t i;
  char logstr[80];

  res = f_open(&file, fname, FA_CREATE_NEW | FA_WRITE);
  if(res != FR_OK)
  {
    show_res("Cannot open file for writing", res);
    return;
  }

  for(i=0; i<250; i++)
  {
    snprintf(logstr, 80, "Logging some data. SysTick->Val = %u\n\r", SysTick->VAL);

    res = f_write(&file, logstr, strlen(logstr), &written);
    f_sync(&file);

    if(res != FR_OK)
    {
      show_res("Error writing", res);
      break;
    } else
    {
      if(written == strlen(logstr))
      {
        uprintf("write complete\n\r");
      }
      else
      {
        uprintf("incomplete - wrote %d03 bytes\n\r",(int) written);
      }
    }
  }
  res = f_close(&file);
  if(res != FR_OK)
    show_res("Error closing file", res);
  uprintf("\n\r");
}


void type_file(char *fname)
{
  FIL file;
  FRESULT res;
  UINT read;

  res = f_open(&file, fname, FA_READ);
  if(res != FR_OK)
  {
    show_res("Cannot open file for reading", res);
    return;
  }

  while(!f_eof(&file))
  {
    res = f_read(&file, Buffer, 256, &read);

    if(res != FR_OK)
    {
      show_res("Error reading file", res);
      break;
    } else
    {
      UART_Transmit(Buffer, read, MAX_TIMEOUT);
    }
  }
  res = f_close(&file);
  if(res != FR_OK)
    show_res("Error closing file", res);
  uprintf("\n\r");
}


void delete_file(char *fname)
{
  FRESULT res;

  res = f_unlink(fname);

  if(res != FR_OK)
    show_res("Error deleting file", res);
  else
  {
    uprintf("File %s deleted\n\r", fname);
  }

  return;
}


/*
 * Scan the given buffer for arguments,
 * split up the buffer and create *args[]
 * to point to each next argument on the line.
 */
void scan_args(char *buf, char *args[])
{
  int i;

  for(i=0; i<N_ARGS; i++) args[i] = 0L;

  i=0;
  while(*buf && (*buf != ' ')) buf++; /* Skip the command    */
  while(*buf == ' ') buf++;           /* and skip any spaces */
  while(*buf && (i < N_ARGS))
  {
    args[i] = buf;
    while(*buf && (*buf != ' ')) buf++; /* Forward to the next white space */
    while(*buf == ' ') buf++;           /* and skip the spaces             */
    i++;
  }
}
/**
 * @brief Fatfs menu function
 */
void FATFS_CMD(void)
{
  if(strncmp(line, "status", 6)==0)
  {
    if (disk_ioctl(0, GET_SECTOR_COUNT, &p2) == RES_OK)
      { uprintf("Drive size:  %d08 sectors\n\r", p2); }
    if (disk_ioctl(0, GET_SECTOR_SIZE, &w1) == RES_OK)
      { uprintf("Sector size: %d08\n\r", w1); }
    if (disk_ioctl(0, GET_BLOCK_SIZE, &p2) == RES_OK)
      { uprintf("Erase block: %d08 sectors\n\r", p2); }
  }
  else if(strncmp(line, "dir", 3) == 0)
  {
    show_dir(args[0]);
  }
  else if(strncmp(line, "write", 5) == 0)
  {
//      write_file(args[0]);
  }
  else if(strncmp(line, "read", 4) == 0)
  {
//      read_file(args[0]);
  }
  else if(strncmp(line, "log", 3) == 0)
  {
    write_log(args[0]);
  }
  else if(strncmp(line, "type", 4) == 0)
  {
    type_file(args[0]);
  }
  else if(strncmp(line, "del", 3) == 0)
  {
    delete_file(args[0]);
  }
  else if(strncmp(line, "help", 4) == 0)
  {
    Print_Help();
  }
  else
  {
    uprintf("Error: invalid command\n\r");
    Print_Help();
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
