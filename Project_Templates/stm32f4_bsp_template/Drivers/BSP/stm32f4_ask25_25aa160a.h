/**
  ******************************************************************************
  * @file    stm32f4_ask25_25aa160a.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    21-April-2015
  * @brief   This file contains definitions for STM32F4-ASK25 Kit's SPI EEPROM.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_ASK25_25AA160A_H
#define __STM32F4_ASK25_25AA160A_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_vcp.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25_25AA160A
  * @{
  */

/** @defgroup STM32F4_ASK25_25AA160A_Exported_Handle
 * @{
 */
extern SPI_HandleTypeDef hspi1_25aa160a;

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_25AA160A_Exported_Types
  * @{
  */


/**
  * @}
  */

/** @defgroup STM32F4_ASK25_25AA160A_Exported_Constants
  * @{
  */
#define EEP_WRITE  0x02;  /* Write bit of IR     */
#define EEP_READ   0x03;  /* Read bit of IR      */
#define EEP_WREN   0x06;  /* Write Enable Latch  */
#define EEP_WRDI   0x04;  /* Write Disable Latch */
#define EEP_RDSR   0x05;  /* Read Status Reg     */
#define EEP_WRSR   0x01;  /* Write Status Reg    */

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_25AA160A_Exported_Functions
  * @{
  */
/* I2C Initialization Functions */
void ASK25_SPI1_Init(void);
void ASK25_SPI1_MspDeInit(SPI_HandleTypeDef* hspi);

uint8_t ASK25_25AA160A_Read_Status_Reg (void);
void ASK25_25AA160A_Read_Status (void);
HAL_StatusTypeDef ASK25_25AA160A_Write_Status_Reg (uint8_t status_reg);
HAL_StatusTypeDef ASK25_25AA160A_Write_Byte (uint16_t eep_address, uint8_t byte_data);
uint8_t ASK25_25AA160A_Read_Byte (uint16_t eep_address);
HAL_StatusTypeDef ASK25_25AA160A_Write (uint16_t eep_address, uint8_t *data_start, uint16_t length);
HAL_StatusTypeDef ASK25_25AA160A_Read (uint16_t eep_address, uint8_t *dest_addr, uint16_t length);
void ASK25_25AA160A_Display_Array (uint8_t *string, uint16_t length);


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

#endif /* __STM32F4_ASK25_25AA160A_H */

/****************************************END OF FILE******************************************/
