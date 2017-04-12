/**
  ******************************************************************************
  * @file    stm32f4_rfid.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    12-May-2016
  * @brief   Contains all functions support for RFID library on STM32F4
  ******************************************************************************
  *
/* Includes ------------------------------------------------------------------*/

#include "stm32f4_rfid.h"
#include "stm32f4_discovery_uart.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_RFID
  * @{
  */

/** @defgroup STM32F4_RFID
  * @brief This file provides set of firmware functions to manage RFID module.
  * @{
  */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_RFID_Public_Types
 * @{
 */
uint8_t a[3][10]={"Chintan","Ketan","Dwijay"};
uint8_t b[3][13]={"090073EF2CB9","090073C6DF63","090073C569D6"};

/**
 * @} STM32F4_RFID_Public_Types End
 */

/** @defgroup STM32F4_RFID_Functions
  * @{
  */

/**
 * @brief This function reads Data from uart2, sent upon scanning card by RFID module
 * @param *RxBuf  Pointer to the location at which scanned data is to be stored
 */

void BSP_RFID_Read(uint8_t *RxBuf)
{
    uint8_t i=0;

    for(i=0;i<12;i++)
    {
        RxBuf[i]= ugetche(BLOCKING);					//Read Data from uart2
    }
    RxBuf[12]='\0';
}


/**
 * @brief This function Extracts RFID card no. from received data.
 * @param *RxBuf     Pointer to the location at which scanned Card data is stored.
 * @param *RFIDNum   Pointer to the location at which scanned card no. is to be stored
 */


void BSP_RFID_Get_Number(uint8_t *RxBuf,uint8_t *RFIDNum)
{
    uint8_t i=0;

    for(i=0;i<12;i++)
    {
        RFIDNum[i]=RxBuf[i];						//Extract card no. from received data
    }
    RFIDNum[12]='\0';
}

/**
 * @brief This function compares scanned card no. with database.
 * @param *RxBuf     Pointer to the location at which scanned id is to be stored
 * @return 		     This function returns the index of user registered for that card.
 */

uint8_t BSP_Match_Card(uint8_t *RFIDNum)
{
  uint8_t i,j,flag=0;

  for(i=0;i<3;i++)
  {
    for(j=0;j<13;j++)
    {
      if((RFIDNum[j])!=b[i][j])         //comparing card no. with database.
      {
        flag=0;
        break;
      }
      else
        flag=i+1;
    }
    if(flag!=0)
      break;
  }

  if(flag==0)
    return 0;
  else
    return (i+1);
}


/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
