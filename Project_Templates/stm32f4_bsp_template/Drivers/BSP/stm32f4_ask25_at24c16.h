/**
  ******************************************************************************
  * @file    stm32f4_ask25_at24c16.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains definitions for STM32F4-ASK25 Kit's I2C EEPROM.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_ASK25_AT24C16_H
#define __STM32F4_ASK25_AT24C16_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25_AT24C16
  * @{
  */

/** @defgroup STM32F4_ASK25_AT24C16_Exported_Handle
 * @{
 */
extern I2C_HandleTypeDef hi2c1_at24c16;

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_AT24C16_Exported_Types
  * @{
  */
/**
 * EEPROM Slave Address or Device Address
 */
#define  E2P24C16_ID    0xA0

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_AT24C16_Exported_Constants
  * @{
  */


/**
  * @}
  */


/** @defgroup STM32F4_ASK25_AT24C16_Exported_Functions
  * @{
  */
/* I2C Initialization Functions */
void ASK25_I2C1_Init(void);
void ASK25_I2C1_MspInit(I2C_HandleTypeDef* hi2c);
void ASK25_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c);

HAL_StatusTypeDef ASK25_AT24C16_Write_Byte (uint16_t eep_address, uint8_t byte_data);
HAL_StatusTypeDef ASK25_AT24C16_Write (uint16_t eep_address, uint8_t* byte_data, uint16_t length);
uint8_t ASK25_AT24C16_Read_Byte (uint16_t eep_address);
HAL_StatusTypeDef ASK25_AT24C16_Read (uint16_t eep_address, uint8_t* buf_data, uint16_t length);
void Display_Eeprom_Array (uint8_t *string, uint16_t length);
void Display_Eeprom_Loc (uint16_t mem_start_address, uint16_t mem_end_address);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_ASK25_AT24C16_H */

/****************************************END OF FILE******************************************/
