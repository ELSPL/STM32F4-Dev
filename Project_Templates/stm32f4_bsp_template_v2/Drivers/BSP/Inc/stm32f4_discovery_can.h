/******************************************************************//**
* @file   stm32f4_discovery_can.h
* @brief  This file contains declaration for stm32f4_discovery_can.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_CAN_H
#define __STM32F4_DISCOVERY_CAN_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"


/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_CAN
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_CAN_Public_Types
 * @{
 */
extern CAN_HandleTypeDef hcan_bsp1;
extern CAN_HandleTypeDef hcan_bsp2;

typedef enum
{
  FIFO0,
  FIFO1
}Fifo_Assignment_Typedef;

/**
 * @} STM32F4_DISCOVERY_CAN_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_CAN_Public_Functions
 * @{
 */

/* CAN initialization and configuration function */
void BSP_CAN_Init(CAN_HandleTypeDef* hcan, uint32_t can_mode, Fifo_Assignment_Typedef fifo_assignment);

/* CAN deinit function */
void BSP_CAN_MspDeInit(CAN_HandleTypeDef* hcan);

/**
 * @} STM32F4_DISCOVERY_CAN_Public_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_CAN_H */

/**
 * @} STM32F4_DISCOVERY_CAN End
 */

/* --------------------------------- End Of File ------------------------------ */
