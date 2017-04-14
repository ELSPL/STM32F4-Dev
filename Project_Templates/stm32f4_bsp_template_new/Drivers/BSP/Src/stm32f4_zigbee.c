/**
  ******************************************************************************
  * @file    stm32f4_zigbee.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    12-May-2016
  * @brief   Contains all functions support for zigbee(tarang) module library on STM32F4
  ******************************************************************************
  *
/* Includes ------------------------------------------------------------------*/

#include "stm32f4_zigbee.h"
#include "stm32f4_discovery_uart.h"



/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ZIGBEE
  * @{
  */

/** @defgroup STM32F4_ZIGBEE
  * @brief This file provides set of firmware functions to manage zigbee module kit.
  * @{
  */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_ZIGBEE_Public_Types
 * @{
 */

uint8_t enter_cmd_mode[3]="+++";   /**< command for entering in command mode*/
uint8_t default_conf[5]="ATGRD";    /**< command for configuring zigbee in default configuration*/
uint8_t zigbee_channel[8]="ATNCH";  /**< command for setting zigbee communication channel*/
uint8_t zigbee_baud[7]="ATSBD3\r";   /**< command for setting zigbee communication baudrate*/
uint8_t zigbee_source[5]="ATNMY";   /**< command for setting source address of zigbee module*/
uint8_t zigbee_des[5]="ATNDA";    /**< command for setting destination address of zigbee module*/
uint8_t zigbee_mem[5]="ATGWR";    /**< command for saving current configuration of zigbee in memory*/
uint8_t exit_cmd_mode[5]="ATGEX";   /**< command for exiting from command mode*/
uint32_t src_addr=0x0001;   /**< 32 bit source address */
uint32_t des_addr=0x0002;   /**< 32 bit dest. address */

/**
 * @} STM32F4_ZIGBEE_Public_Types End
 */

/** @defgroup STM32F4_ZIGBEE_Functions
  * @{
  */

/**
 * @brief     This function is used to enter into command mode of zigbee module
 * @param[in] None
 * @return    1 if Device Responded OK
 *            0 if Device Responded ERROR
 */

uint8_t BSP_ZigBee_Enter_Cmd_Mode(void)
{
  uint8_t i=0,Data[7]={0};

  uprintf(&huart6,"%c",enter_cmd_mode[0]);   /*sending '+'*/
  HAL_Delay(420);
  uprintf(&huart6,"%c",enter_cmd_mode[1]);   /*sending '+'*/
  HAL_Delay(420);
  uprintf(&huart6,"%c",enter_cmd_mode[2]);   /*sending '+'*/

  for(i=0;i<7;i++)        /*receiving acknowledgment from device*/
  {
    Data[i] = ugetche(&huart6,BLOCKING);
  }
  if((Data[3]=='O') && (Data[4]=='K'))      /*Checking if response is 'OK' or not*/
    return 1;
  else
    return 0;
}


/**
 * @brief     This function is used to initialize zigbee module with
 *        Default Parameters.
 * @param[in] None
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */
uint8_t BSP_ZigBee_Init(void)
{
  uint8_t Data[7]={0},i=0;

  uprintf(&huart6,"%s\r",default_conf);      /*sending Command to set zigbee on default config.*/

  uget_line(&huart6,Data,4);    /*receiving acknowledgment from device*/

  if((Data[0]=='O') && (Data[1]=='K'))      /*Checking if response is 'OK' or not*/
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


/**
 * @brief     This function is used to select channel for communication
 *        between zigbee module
 * @param[in] Channel no.(0-9)
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */

uint8_t BSP_Zigbee_Ch(uint8_t Ch_No)
{
  uint8_t Data[7]={0},i=0;

  uprintf(&huart6,"%s",zigbee_channel);      /*sending Command to set zigbee channel.*/
  uprintf(&huart6,"%c\r",Ch_No);
  uget_line(&huart6,Data,7);

  if((Data[1]=='O') && (Data[2]=='K'))      /*Checking if response is 'OK' or not*/
    return 1;
  else
    return 0;
}


/**
 * @brief     This function is used to set Baud Rate for communication
 *        between zigbee module serial port and Host
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */

uint8_t BSP_Zigbee_Baudrate(void)
{
  uint8_t Data[7]={0},i;

  uprintf(&huart6,"%s",zigbee_baud);       /*sending Command to set zigbee communication Baud rate 9600*/
  uget_line(&huart6,Data,7);    /*receiving acknowlegerment from device*/

  if((Data[1]=='O') && (Data[2]=='K'))      /*Checking if response is 'OK' or not*/
    return 1;
  else
    return 0;
}


/**
 * @brief     This function is used to set Source address of
 *        zigbee module
 * @param[in] None
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */

uint8_t BSP_Zigbee_Source_Add(void)
{
  uint8_t Data[7]={0},i;

  uprintf(&huart6,"%s",zigbee_source);       /*sending Command to set zigbee module source address*/
  uprintf(&huart6,"%x04\r",src_addr);        /*sending 32 bit Source addr.*/

  uget_line(&huart6,Data,7);        /*receiving acknowledgment from device*/

  if((Data[1]=='O') && (Data[2]=='K'))        /*Checking if response is 'OK' or not*/
    return 1;
  else
    return 0;
}


/**
 * @brief     This function is used to set Destination address of
 *        zigbee module
 * @param[in] None
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */

uint8_t BSP_Zigbee_Dest_Add(void)
{
  uint8_t Data[7]={0},i;

  uprintf(&huart6,"%s",zigbee_des);          /*sending Command to set zigbee module Dest address*/
  uprintf(&huart6,"%x04\r",des_addr);        /*sending 32 bit Destination address*/
  uget_line(&huart6,Data,4);                 /*receiving acknowlegerment from device*/

  if((Data[1]=='O') && (Data[2]=='K'))        /*Checking if response is 'OK' or not*/
    return 1;
  else
    return 0;
}


/**
 * @brief     This function is used to store set parameters to memory
 * @param[in] None
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */

uint8_t BSP_Zigbee_Save(void)
{
  uint8_t Data[7]={0},i=0;

  uprintf(&huart6,"%s\r",zigbee_mem);        /*sending Command to store set parameters to memory */
  uget_line(&huart6,Data,4);      /*receiving acknowlegerment from device*/

  if((Data[1]=='O') && (Data[2]=='K'))        /*Checking if response is 'OK' or not*/
    return 1;
  else
    return 0;
}


/**
 * @brief     This function is used to Exit from Command mode of Zigbee module.
 * @param[in] None
 * @return    1 if Device Responded OK
 *        0 if Device Responded ERROR
 */

uint8_t BSP_ZigBee_Exit(void)
{
  uint8_t Data[7]={0},i=0;

  uprintf(&huart6,"%s\r",exit_cmd_mode);       /*sending Command to Exit from command */
  uget_line(&huart6,Data,7);      /*receiving acknowlegerment from device*/

  if((Data[1]=='E') && (Data[2]=='X') && (Data[3]=='I') && (Data[4]=='T'))
                            /*Checking if response is 'EXIT or not*/
    return 1;
  else
    return 0;
  HAL_Delay(500);
}


/**
 * @brief     This function reads channel no entered from MAT-KB and displays on LCD
 * @param[in] none
 * @return    channel no.
 */
uint8_t BSP_Read_Ch_No()
{
  uint8_t i,ch_no;
  ch_no = vgetche(BLOCKING);         /* ID number range 0 to 9*/
  vuprintf("%c",ch_no);
  vuprintf("\r\n");
  return ch_no;
}

/**
 * @} STM32F4_ZIGBEE_Functions End
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
