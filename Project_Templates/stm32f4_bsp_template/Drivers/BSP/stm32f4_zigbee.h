/**
  ******************************************************************************
  * @file    stm32f4_zigbee.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    12-May-2016
  * @brief   Contains all function for zigbee module library on STM32F4
  ******************************************************************************
  */

#ifndef __STM32F4_ZIGBEE_H
#define __STM32F4_ZIGBEE_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "stm32f4_global.h"


#ifdef __cplusplus
 extern "C" {
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_ZIGBEE_Public_Types
  * @{
  */

 extern uint8_t Enter_Cmd_Mode[];    /**< command for entering in command mode*/
 extern uint8_t Default_Conf[];    /**< command for configuring zigbee in default configuration*/
 extern uint8_t Zigbee_Channel[];    /**< command for setting zigbee communication channel*/
 extern uint8_t Zigbee_Baud[];     /**< command for setting zigbee communication baudrate*/
 extern uint8_t Zigbee_Source[];   /**< command for setting source address of zigbee module*/
 extern uint8_t Zigbee_Des[];      /**< command for setting destination address of zigbee module*/
 extern uint8_t Zigbee_Mem[];      /**< command for saving current configuration of zigbee in memory*/
 extern uint8_t Exit_Cmd_Mode[];   /**< command for exiting from command mode*/
 extern uint32_t Src_Addr;     /**< 32 bit source addr*/
 extern uint32_t Des_Addr;     /**< 32 bit dest. addr*/

/**
 * @} STM32F4_ZIGBEE_Public_Types End
 */


/** @defgroup STM32F4_ZIGBEE_Exported_Functions
  * @{
  */

 uint8_t BSP_ZigBee_Enter_Cmd_Mode(void);
 uint8_t BSP_ZigBee_Init(void);
 uint8_t BSP_Zigbee_Ch(uint8_t Ch_No);
 uint8_t BSP_Zigbee_Baudrate(void);
 uint8_t BSP_Zigbee_Source_Add(void);
 uint8_t BSP_Zigbee_Dest_Add(void);
 uint8_t BSP_Zigbee_Save(void);
 uint8_t BSP_ZigBee_Exit(void);
 uint8_t BSP_Read_Ch_No();

/**
 * @} STM32F4_ZIGBEE_Exported_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_ZIGBEE_H */

 /****************************************END OF FILE******************************************/
