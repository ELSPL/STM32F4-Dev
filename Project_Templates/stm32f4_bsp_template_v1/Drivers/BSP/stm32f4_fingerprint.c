/**
  ******************************************************************************
  * @file    stm32f4_fingerprint.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    09-May-2016
  * @brief   Contains all functions support for FINGERPRINT library on STM32F4
  ******************************************************************************
  *
/* Includes ------------------------------------------------------------------*/

#include "stm32f4_fingerprint.h"
#include "stm32f4_discovery_uart.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_FINGERPRINT
  * @{
  */

/** @defgroup STM32F4_FINGERPRINT
  * @brief This file provides set of firmware functions to manage FINGERPRINT kit.
  * @{
  */

/** @defgroup STM32F4_FINGERPRINT_Functions
  * @{
  */

/**
 * @brief     This function checks received acknowledgment in case of module
 *        don't succeed to perform operation, and display error msg. on lcd.
 * @param[in] Pointer to the location at which scanned id is to be stored
 * @return    none
 */

void ResultError(Result_typedef Resul)
{
  if(Resul == RESULT_FAILED)
    vuprintf("RESULT_FAILED\r\n");

  if(Resul == RESULT_NOT_MASTER_MODE)
    vuprintf("RESULT_NOT_MASTER_MODE\r\n");

  if(Resul == RESULT_USED_ID)
    vuprintf("RESULT_USED_ID\r\n");

  if(Resul == RESULT_INVALID_ID)
    vuprintf("RESULT_INVALID_ID\r\n");

  if(Resul == RESULT_DB_IS_FULL)
    vuprintf("RESULT_DB_IS_FULL\r\n");

  if(Resul == RESULT_NOT_IN_TIME)
    vuprintf("RESULT_NOT_IN_TIME\r\n");

  if(Resul == RESULT_INVALID_DATASIZE)
    vuprintf("RESULT_INVALID_DATASIZE\r\n");

  if(Resul == RESULT_INVALID_DATA)
    vuprintf("RESULT_INVALID_DATA\r\n");

  if(Resul == RESULT_INVALID_PARAM)
    vuprintf("RESULT_INVALID_PARAM\r\n");

  if(Resul == RESULT_CANCELED)
    vuprintf("RESULT_CANCELED\r\n");

  if(Resul == RESULT_ANOTHER_FINGER)
    vuprintf("RESULT_ANOTHER_FINGER\r\n");

  if(Resul == RESULT_EXTRACT_FAIL)
    vuprintf("RESULT_EXTRACT_FAIL\r\n");

  if(Resul == RESULT_IDENTIFY_TIMEOUT)
    vuprintf("RESULT_IDENTIFY_TIMEOUT\r\n");

  HAL_Delay(2000);
}


/**
 * @brief     This function is used to get connection with finger print device
 * @param[in] none
 * @return    connection status
 */
uint8_t BSP_FP_Connect(void)
{
  uint8_t i,DataBuf[25]={0},RxBuf[25]={0};

  DataBuf[0]=0x7E;
  DataBuf[4]=0x01;                  //command stream to fingerprint
  DataBuf[24]=0x01;                 //0x7e,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1

  UART_Transmit(&DataBuf,25,10);
  UART_Receive(&RxBuf,25,100);

  /**
   *RxBuf[4]==0x01 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */
  if((RxBuf[4] == 0x01) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**
 * @brief     This function changes mode into master mode level of control
 * @param[in] none
 * @return    connection status
 */

uint8_t BSP_FP_MasterMode(void)
{
  uint8_t DataBuf[25]={0},RxBuf[25]={0};

  DataBuf[0]=0x7e;
  DataBuf[4]=0x2f;
  DataBuf[8]=0x03;                  //command stream to fingerprint
  DataBuf[24]=0x32;                 //0x7e,0,0,0,0x2f,0,0,0,0x03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x32

  UART_Transmit(&DataBuf,25,10);   //send command
  UART_Receive(&RxBuf,25,100);    //receive acknowledgment

  /**
   *RxBuf[4]==0x2f : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x2f) && (RxBuf[8] == 0x01) && (RxBuf[12] == 0x03))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**
 * @brief     This Function Asks for an Id No. and then scans the finger print
 *        for that Id Its the first scan .
 *        Note:Id shouldn't be previously registered
 *            and mode should be master mode
 * @param[in] pointer to id no.
 * @return    result status
 */

uint8_t BSP_FP_Enroll_step1(uint8_t *id)
{
  uint8_t checksum = 0xc0,DataBuf[25]={0},RxBuf[25]={0};
  int8_t i=0;

  DataBuf[0]=0x7e;
  DataBuf[4]=0x33;
  DataBuf[16]=0x1A;                       //command stream to fingerprint
  DataBuf[24]=0x4D;                 //0x7e,0,0,0,x33,0,0,0,0,0,0,0,0,0,0,0x1a,0,0,0,0,0,0,0,0,0x4d

  UART_Transmit(&DataBuf,25,10);         //send command

  for(i=3;i>=0;i--)
  {
    uprintf("%c",id[i]);             //send  ascii value of id no. digit by digit
    checksum+=id[i] - 0x30;
  }

  for(i=0;i<25;i++)
  {
    uprintf("%c",0x0);               //send command
  }

  uprintf("%c",checksum);            //send checksum

  UART_Receive(&RxBuf,25,5000);          //receive acknowledgment

  /**
   *RxBuf[4]==0x33 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x33) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**
 * @brief     This function scans the second finger print for the previously
 *        entered Id.
 *        note: It works in conjunction with FP_Enroll_step1
 * @param[in] none
 * @return    result status
 */

uint8_t BSP_FP_Enroll_step2(void)
{
  uint8_t DataBuf[25]={0},RxBuf[25]={0};

  DataBuf[0]=0x7E;
  DataBuf[4]=0x33;
  DataBuf[12]=0x01;                     //command stream to finger print
  DataBuf[24]=0x34;                     //0x7e,0,0,0,0x33,0,0,0,0,0,0,0,0x01,0,0,0,0,0,0,0,0,0,0,0,0x34

  UART_Transmit(&DataBuf,25,10);   //send command

  UART_Receive(&RxBuf,25,5000);    //receive acknowledgment

  /**
   *RxBuf[4]==0x33 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x33) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }

}


/**
 * @brief     This function takes in the Id no and scans the finger print for
 *        comparison with the previously stored Finger Print of that particular Id.
 * @param[in] pointer to the id no. at which matched id no. will be stored.
 * @return    result status
 */

uint8_t BSP_FP_Verify(uint8_t *id)
{
  uint8_t checksum = 0xc0,DataBuf[25]={0},RxBuf[25]={0};
  int8_t i;
  DataBuf[0]=0x7E;
  DataBuf[4]=0x11;
  DataBuf[16]=0x0A;                     //command stream to fingerprint
  DataBuf[24]=0x1B;                     //0x7e,0,0,0,0x11,0,0,0,0,0,0,0,0,0,0,0,0x0a,0,0,0,0,0,0,0,0x1b

  UART_Transmit(&DataBuf,25,10);

  for(i=3;i>=0;i--)
  {
    uprintf("%c",id[i]);             //send  ascii value of id no. digit by digit
    checksum+=id[i] - 0x30;
  }

  for(i=0;i<9;i++)
  {
    uprintf("%c",0x0);               //send command
  }

  uprintf("%c",checksum);            //send checksum

  UART_Receive(&RxBuf,25,5000);    //receive acknowledgment

  /**
   *RxBuf[4]==0x11 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x11) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**
 * @brief     This function scans the finger print for comparing with previously
 *        stored Finger Prints On identification it displays the correct Id No.
 * @param[in] pointer to the id no. at which matched id no. will be stored.
 * @return    connection status
 */
uint8_t BSP_FP_Identify(uint8_t *id)
{
  uint8_t DataBuf[25]={0},RxBuf[25]={0},XtraBuf[14],i;

  DataBuf[0]=0x7E;
  DataBuf[4]=0x12;                      //command stream to fingerprint
  DataBuf[24]=0x12;                     //0x7e,0,0,0,0x12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x12

  UART_Transmit(&DataBuf,25,10);   //send command

  UART_Receive(&RxBuf,25,5000);    //receive acknowledgment
  if(RxBuf[16]!=0)                //if Data size is not zero then read data
  {
    UART_Receive(&XtraBuf,14,5000);
    id[0]=XtraBuf[0];
    id[1]=XtraBuf[1];
    id[2]=XtraBuf[2];
    id[3]=XtraBuf[3];
  }

  /**
   *RxBuf[4]==0x12 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

    if((RxBuf[4] == 0x12) && (RxBuf[8] == 0x01))
    {
      return(RESULT_SUCCEEDED);
    }
    else
    {
      return(RxBuf[8]);
    }
}


/**
 * @brief     This function deletes the finger print record of the id no. entered.
 * @param[in] pointer to the id no. at which id no. which is to be deleted is stored.
 * @return    result status
 */

uint8_t BSP_FP_Delete(uint8_t *id)
{
  uint8_t checksum= 0xc0,DataBuf[25]={0},RxBuf[25]={0};
  int8_t i=0;
  DataBuf[0]=0x7E;
  DataBuf[4]=0x22;
  DataBuf[16]=0x0A;                   //command stream to fingerprint
  DataBuf[24]=0x2C;                   //0x7e,0,0,0,0x22,0,0,0,0,0,0,0,0,0,0,0,0x0a,0,0,0,0,0,0,0,0x2c

  UART_Transmit(&DataBuf,25,10);

  for(i=3;i>=0;i--)
  {
    uprintf("%c",id[i]);             //send  ascii value of id no. digit by digit
    checksum+=id[i] - 0x30;
  }

  for(i=0;i<9;i++)
  {
    uprintf("%c",0x0);               //send command
  }

  uprintf("%c",checksum);            //send checksum

  UART_Receive(&RxBuf,25,5000);    //receive acknowledgment


  /**
   *RxBuf[4]==0x22 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x22) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**
 * @brief     This function brings the mode from master mode to normal mode.
 *              Note : It has to be done in Master Mode
 * @param[in] none
 * @return    result status
 */

uint8_t BSP_FP_LeaveMaster_Mode(void)
{
  uint8_t DataBuf[25]={0},RxBuf[25]={0};

  DataBuf[0]=0x7e;
  DataBuf[4]=0x26;                    //command stream to fingerprint
  DataBuf[24]=0x26;                   //0x7e,0,0,0,0x26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x26

  UART_Transmit(&DataBuf,25,10);
  UART_Receive(&RxBuf,25,5000);    //receive acknoledgement

  /**
   *RxBuf[4]==0x26 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x26) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**
 * @brief     This function Deletes all the logs stored in FP module.
 * @param[in] none
 * @return    Result status
 */

uint8_t BSP_FP_DeleteAll(void)
{
  uint8_t DataBuf[25]={0},RxBuf[25]={0};

  DataBuf[0]=0x7e;
  DataBuf[4]=0x23;                      //command stream to fingerprint
  DataBuf[24]=0x23;                     //0x7e,0,0,0,0x37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x37

  UART_Transmit(&DataBuf,25,10);     //send command
  UART_Receive(&RxBuf,25,5000);      //receive acknoledgement

  /**
   *RxBuf[4]==0x23 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x23) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}


/**********************************************************************************//**
 * @brief     This function reads the device and stores id in database and
 *        also gives total registered user count.
 * @param[in] pointer to the database
 * @param[in] user count variable
 * @return    result status
 ************************************************************************************/

uint8_t BSP_FP_List(uint8_t *database, uint8_t *count)
{
  uint8_t DataBuf[25]={0},RxBuf[25]={0},XtraBuf[998]={0},i,t;
  DataBuf[0]=0x7E;
  DataBuf[4]=0x30;                      //command stream to fingerprint
  DataBuf[24]=0x30;                     //0x7e,0,0,0,0x12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x12

  UART_Transmit(&DataBuf,25,10);         //send command
  UART_Receive(&RxBuf,25,5000);          //receive acknowledgment
  UART_Receive(&XtraBuf,(RxBuf[16]+4),5000);   //receive acknowledgment

  *count=0;
  for(i=0;i<100;i++)
    database[i]=0;

  if(RxBuf[16]>8)
  {
    for(i=4;i<(RxBuf[16]);i+=10)
    {
      t=((XtraBuf[i+2]-0x30)*10+(XtraBuf[i+3]-0x30));
      database[t]=1;                    //storing database.
      *count=*count+1;
    }
  }
  /**
   *RxBuf[4]==0x30 : Received command must be same as transmitted command
   *RxBuf[8]==0x01 : Stands for successful execution of transmitted command
   *If not executed successfully RxBuf[8] is used to determine error status
   */

  if((RxBuf[4] == 0x30) && (RxBuf[8] == 0x01))
  {
    return(RESULT_SUCCEEDED);
  }
  else
  {
    return(RxBuf[8]);
  }
}

/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
