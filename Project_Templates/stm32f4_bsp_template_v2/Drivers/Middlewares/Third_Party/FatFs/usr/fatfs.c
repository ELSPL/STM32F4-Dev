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
#include "ff.h"
#include "stm32f4_discovery_uart.h"

uint8_t retUSBH;    /* Return value for USBH */
char USBH_Path[4];  /* USBH logical drive path */
uint8_t retUSER,j=0;    /* Return value for USER */
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
const char *help_msg[11]={"\r\nCommand List:\r\n",
    "status         - disk status\r\n",
    "dir<dir name>  - list directory/files\r\n",
    "tree           - View all files and directory\r\n",
    "write<f_name>  - create file with f_name and dump some data\r\n",
    "append<f_name> - append file in f_name\r\n",
    "read<f_name>   - read file with f_name and compare dump data\r\n",
    "log<f_name>    - write real time log of system tick value\r\n",
    "del<f_name>    - delete file/directory\r\n",
    "size<dir name> - Size of directory\r\n",
    "help           - print this msg\n\r",
};

void Print_Help(void)
{
  uint8_t count;

  // clr_scr_rst_cur();

  for(count = 0; count<=10; ++count)
  {
    uprintf(&huart4,help_msg[count]);
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
  uprintf(&huart4,"\n\r    %s: res=%d02 FR_%s\n\n\r", mesg, (UINT)rc, res_str);
}

void size(void)
{
  DWORD clust;
  FATFS *fs;
  DWORD TotalSpace, FreeSpace;
  FRESULT res;
  DIR dir;
  FILINFO fileinfo;

  res = f_opendir(&dir, '/');
  if (res!= FR_OK)
  {
    show_res("\n\rCannot open dir\n\r", res);
    return;
  }

  uprintf(&huart4,"\n\r%cCalculating disk space...\r\n", 0x09);
  res = f_getfree('/', &clust, &fs);
  if(res)
  {
    uprintf(&huart4,"f_getfree() failed....!\n\r");
  }

  TotalSpace = (DWORD)((fs->n_fatent - 2) * (fs->csize / 2));
  FreeSpace = (fs->free_clust) * (fs->csize / 2);
  uprintf(&huart4,"%c%d08 KB total disk space.\r\n", 0x09, TotalSpace);
  uprintf(&huart4,"%c%d08 KB available on the disk.\r\n", 0x09, FreeSpace);
  res = f_closedir (&dir);
  if(res != FR_OK)
    show_res("\n\rError closing Directory\n\r", res);
}

void show_dir(char *dirname)
{
  DWORD clust;
  FATFS *fs;
  DWORD TotalSpace, FreeSpace;
  FRESULT res;
  DIR dir;
  static FILINFO fileinfo;

  res = f_opendir(&dir, dirname);

  if (res!= FR_OK)
  {
    show_res("\n\rCannot open dir\n\r", res);
    return;
  }
  for(;;)
  {
    res=f_readdir(&dir, &fileinfo);
    if(res == FR_OK)
    {
      if(fileinfo.fname[0] != 0)
      {
        uprintf(&huart4,"%c%c%c%c%c %d04/%d02/%d02 %d02:%d02 %d08  %s\n\r",
            (fileinfo.fattrib & AM_DIR) ? 'D' : '-',
                (fileinfo.fattrib & AM_RDO) ? 'R' : '-',
                    (fileinfo.fattrib & AM_HID) ? 'H' : '-',
                        (fileinfo.fattrib & AM_SYS) ? 'S' : '-',
                            (fileinfo.fattrib & AM_ARC) ? 'A' : '-',
                                (fileinfo.fdate >> 9) + 1980, (fileinfo.fdate >> 5) & 15, fileinfo.fdate & 31,
                                (fileinfo.ftime >> 11), (fileinfo.ftime >> 5) & 63,
                                fileinfo.fsize, &(fileinfo.fname[0]));
      }
    }
    if(fileinfo.fname[0] == 0)
    {
      break;
    }
  }
  res = f_closedir (&dir);
  if(res != FR_OK)
    show_res("\n\rError closing Directory\n\r", res);
  uprintf(&huart4,"\n\r%s Directory is closed\n\r",fileinfo.fname);
}



FRESULT tree(char *dirname)
{

  DWORD clust;
  FATFS *fs;
  DWORD TotalSpace, FreeSpace;
  FRESULT res;
  DIR dir;
  uint8_t i;
  static FILINFO fileinfo;

  res = f_opendir(&dir,dirname);
  j++;
  if (res == FR_OK)
  {
    for(;;)
    {
      res = f_readdir(&dir, &fileinfo);
      if(res != FR_OK || fileinfo.fname[0] == 0)  break;
      if(fileinfo.fattrib & AM_DIR)
      {
        uprintf(&huart4,"|");
        i=strlen(dirname);
        int a;
        for(a=j;a>1;a--)
        {
          uprintf(&huart4,"  |");
        }
        uprintf(&huart4,"->%s\n\r",fileinfo.fname);//PATH / FILE NAME
        sprintf(&dirname[i],"/%s", fileinfo.fname);
        res = tree(dirname);
        if(res != FR_OK)
          break;
        dirname[i]=0;
      }
      else
      {
        uprintf(&huart4,"|");
        int a;
        for(a=j;a>1;a--)
        {
          uprintf(&huart4,"  |");
        }
        uprintf(&huart4,"->%s\n\r",fileinfo.fname);
      }
    }
    f_closedir (&dir);
    j--;
  }
  return res;
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
    show_res("\n\rCannot open file for writing\n\r", res);
    return;
  }

  for(i=0; i<250; i++)
  {
    snprintf(logstr, 80, "Logging some data. SysTick->Val = %u\n\r", SysTick->VAL);

    res = f_write(&file, logstr, strlen(logstr), &written);
    f_sync(&file);

    if(res != FR_OK)
    {
      show_res("\n\rError writing\n\r", res);
      break;
    } else
    {
      if(written == strlen(logstr))
      {
        uprintf(&huart4,"\n\rwrite complete\n\r");
      }
      else
      {
        uprintf(&huart4,"incomplete - wrote %d03 bytes\n\r",(int) written);
      }
    }
  }
  res = f_close(&file);
  if(res != FR_OK)
    show_res("\n\rError closing file\n\r", res);
  uprintf(&huart4,"\n\rFile is closed\n\r");
}

void append_file(char *fname)
{
  FIL file;
  FRESULT res;
  UINT written;
  uint32_t i;
  uint16_t size;
  uint8_t line2[255];

  res = f_open(&file, fname, FA_WRITE | FA_OPEN_ALWAYS);
  if(res != FR_OK)
  {
    show_res("\n\rCannot open file for writing\n\r", res);
    return;
  }
  else
  {
    res = f_lseek(&file, f_size(&file));
    if (res != FR_OK)
      f_close(&file);
  }

  uprintf(&huart4,"\n\rEnter data to write in file\n\r");

  size=uget_line(&huart4,line2, sizeof(line2));

  res = f_puts(line2 , &file);
  f_sync(&file);
  uprintf(&huart4,"\n\rData is appended\n\r");
  res = f_close(&file);
  if(res != FR_OK)
    show_res("Error closing file", res);
  else
    uprintf(&huart4,"\n\rFile is closed\n\r");
}




void write_file(char *fname)
{
  FIL file;
  FRESULT res;
  UINT written;
  uint32_t i;
  uint16_t size;
  uint8_t line2[255];

  res = f_open(&file, fname, FA_WRITE | FA_CREATE_ALWAYS);
  if(res != FR_OK)
  {
    show_res("\n\rCannot open file for writing\n\r", res);
    return;
  }
  uprintf(&huart4,"\n\rEnter data to write in file\n\r");
  size=uget_line(&huart4,line2, sizeof(line2));

  res = f_write(&file, line2, size, &written);
  f_sync(&file);

  if(res != FR_OK)
  {
    show_res("Error writing", res);
  }
  else
  {
    uprintf(&huart4,"\n\rComplete - wrote %d03 bytes\n\r",(int) written);
  }
  res = f_close(&file);
  if(res != FR_OK)
    show_res("Error closing file", res);
  uprintf(&huart4,"\n\rFile is closed\n\r");
}

void read_file(char *fname)
{
  FIL file;
  FRESULT res;
  UINT read;
  uint32_t size=0;

  res = f_open(&file, fname, FA_READ);
  if(res != FR_OK)
  {
    show_res("\n\rCannot open file for reading\n\r", res);
    return;
  }
  uprintf(&huart4,"\n\rHere is the data:-\n\r");
  while(!f_eof(&file))
  {
    res = f_read(&file, Buffer, 512, &read);
    if(res == FR_OK)
    {
      UART_Transmit(&huart4,Buffer, read, MAX_TIMEOUT);
    }
  }
  res = f_close(&file);
  if(res != FR_OK)
    show_res("\n\rError closing file\n\r", res);
  uprintf(&huart4,"\n\r File is closed \n\r");
}


void delete_file(char *fname)
{
  FRESULT res;

  res = f_unlink(fname);

  if(res != FR_OK)
    show_res("Error deleting file", res);
  else
  {
    uprintf(&huart4,"\n\rFile %s is deleted\n\r", fname);
  }
  return;
}


/*
 * Scan the given buffer for arguments,
 * split up the buffer and create *args[]
 * to point to each next argument on the line.
 *  @brief Fatfs menu function
 */
void Scan_and_action(char *buf, char *args[])
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

  if(strncmp(line, "status", 6)==0)
  {
    if (disk_ioctl(0, GET_SECTOR_COUNT, &p2) == RES_OK)
    { uprintf(&huart4,"\n\rDrive size:  %d08 sectors\n\r", p2); }
    if (disk_ioctl(0, GET_SECTOR_SIZE, &w1) == RES_OK)
    { uprintf(&huart4,"\n\rSector size: %d08\n\r", w1); }
    if (disk_ioctl(0, GET_BLOCK_SIZE, &p2) == RES_OK)
    { uprintf(&huart4,"\n\rErase block: %d08 sectors\n\r", p2); }
  }
  else if(strncmp(line, "dir", 3) == 0)
  {
    uprintf(&huart4,"\n\r");
    show_dir(args[0]);
  }
  else if(strncmp(line, "tree", 4) == 0)
  {
    uprintf(&huart4,"\n\r");
    tree(args[0]);
  }
  else if(strncmp(line, "write", 5) == 0)
  {
    uprintf(&huart4,"\n\r");
    write_file(args[0]);
  }
  else if(strncmp(line, "append", 6) == 0)
  {
    uprintf(&huart4,"\n\r");
    append_file(args[0]);
  }
  else if(strncmp(line, "read", 4) == 0)
  {
    uprintf(&huart4,"\n\r");
    read_file(args[0]);
  }
  else if(strncmp(line, "log", 3) == 0)
  {
    uprintf(&huart4,"\n\r");
    write_log(args[0]);
  }
  else if(strncmp(line, "del", 3) == 0)
  {
    uprintf(&huart4,"\n\r");
    delete_file(args[0]);
  }
  else if(strncmp(line, "help", 4) == 0)
  {
    uprintf(&huart4,"\n\r");
    Print_Help();
  }
  else if(strncmp(line, "size", 4) == 0)
  {
    uprintf(&huart4,"\n\r");
    size();
  }
  else
  {
    uprintf(&huart4,"\n\r");
    uprintf(&huart4,"\n\rError: invalid command\n\r");
    Print_Help();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
