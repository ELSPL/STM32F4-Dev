/**
  ******************************************************************************
  * @file    stm32f4_therm_print.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    12-May-2016
  * @brief   Contains all function for Thermal printer library on STM32F4
  ******************************************************************************
  */

#ifndef __STM32F4_THERM_PRINT_H
#define __STM32F4_THERM_PRINT_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "stm32f4_global.h"


#ifdef __cplusplus
 extern "C" {
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_THERM_PRINT_Public_Types
  * @{
  */

extern uint8_t sw_sh[];
extern uint8_t sw_dh[];
extern uint8_t dw_sh[];
extern uint8_t dw_dh[];

extern uint8_t isw_sh[];
extern uint8_t isw_dh[];
extern uint8_t idw_sh[];
extern uint8_t idw_dh[];

extern uint8_t graphic[];

/**
 * @} STM32F4_THERM_PRINT_Public_Types End
 */


/** @defgroup STM32F4_THERM_PRINT_Exported_Functions
  * @{
  */

 void BSP_ThermalPrinter_Data(uint8_t *Data);
 void BSP_ThermalPrinter_Cmd(uint8_t *cmd);
 void BSP_ThermalPrinter_Graphic(uint8_t *Gcmd,uint8_t length);

/**
 * @} STM32F4_THERM_PRINT_Exported_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_THERM_PRINT_H */

 /****************************************END OF FILE******************************************/
