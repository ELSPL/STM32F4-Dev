/**
  ******************************************************************************
  * @file    stm32f4_therm_print.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    12-May-2016
  * @brief   Contains all functions support for Thermal printer library on STM32F4
  ******************************************************************************
  *
/* Includes ------------------------------------------------------------------*/

#include "stm32f4_therm_print.h"
#include "stm32f4_discovery_uart.h"



/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_THERM_PRINT
  * @{
  */

/** @defgroup STM32F4_THERM_PRINT
  * @brief This file provides set of firmware functions to manage Thermal printer kit.
  * @{
  */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_THERM_PRINT_Public_Types
 * @{
 */

uint8_t sw_sh[] = {0x1B,0x21,0x00};
uint8_t sw_dh[] = {0x1B,0x21,0x10};
uint8_t dw_sh[] = {0x1B,0x21,0x21};
uint8_t dw_dh[] = {0x1B,0x21,0x31};

uint8_t isw_sh[] = {0x1B, 0x21, 0x40};
uint8_t isw_dh[] = {0x1B, 0x21, 0x50};
uint8_t idw_sh[] = {0x1B, 0x21, 0x61};
uint8_t idw_dh[] = {0x1B, 0x21, 0x71};

uint8_t graphic[]= {0x1B,0x58,0x34};
/**
 * @} STM32F4_THERM_PRINT_Public_Types End
 */

/** @defgroup STM32F4_THERM_PRINT_Functions
  * @{
  */

/**
 * @brief This function transmits command for THERM PRINT module
 * @param Cmd buffer name consist command for THERM PRINT
 */

void BSP_ThermalPrinter_Data(uint8_t *Data)
{
  while(*Data!='\0')
  {
    uprintf(&huart6,"%c",*Data );
    Data++;
  }
}


/**
 * @brief This function receives data from THERM PRINT and polling method is used
 * @param dataBuf  buffer consist data from THERM PRINT
 * @param length   total number of elements data to be read from THERM PRINT
 */


void BSP_ThermalPrinter_Cmd(uint8_t *cmd)
{
  uint8_t i=3;
  while(i--)
  {
      uprintf(&huart6,"%c",*cmd++);
  }
}

/**
 * @brief This function receives data from THERM PRINT
 * @return value
 */
void BSP_ThermalPrinter_Graphic(uint8_t*Gcmd,uint8_t length)
{
  while(length--)
  {
      uprintf(&huart6,"%c",*Gcmd++);

  }
}

/**
 * @} STM32F4_THERM_PRINT_Functions End
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
