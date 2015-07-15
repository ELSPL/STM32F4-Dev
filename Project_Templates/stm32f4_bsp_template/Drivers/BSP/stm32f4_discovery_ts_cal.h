/**
  ******************************************************************************
  * @file    stm32f4_discovery_ts_cal.h
  * @author  Dwijay.edutech leaning solutions
  * @version V1.0.0
  * @date    28-April-2015
  * @brief   This file contains all the functions prototypes for the
  *          stm32f4_discovery_ts_cal.c driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_TS_CAL_H
#define __STM32F4_DISCOVERY_TS_CAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
/* Include IOExpander(STMPE811) component Driver */
#include "../Components/stmpe811/stmpe811.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Exported_Types
  * @{
  */
typedef struct
{
  uint16_t TouchDetected;
  uint16_t x;
  uint16_t y;
  uint16_t z;
}TS_StateTypeDef;
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Exported_Constants
  * @{
  */
#define TS_SWAP_NONE                    0x00
#define TS_SWAP_X                       0x01
#define TS_SWAP_Y                       0x02
#define TS_SWAP_XY                      0x04

typedef enum
{
  TS_OK       = 0x00,
  TS_ERROR    = 0x01,
  TS_TIMEOUT  = 0x02
}TS_StatusTypeDef;
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Exported_Functions
  * @{
  */
uint8_t BSP_TS_Init(uint16_t xSize, uint16_t ySize);
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State);
uint8_t BSP_TS_ITConfig(void);
uint8_t BSP_TS_ITGetStatus(void);
void    BSP_TS_ITClear(void);

/**
  * @}
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

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_TS_CAL_H */

/*************END OF FILE****/
