/**
  ******************************************************************************
  * @file    rawpacket.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    29-September-2015
  * @brief   This file contains definitions for raw packet transmit function
  ******************************************************************************
  */

#ifndef __RAWPACKET_H_
#define __RAWPACKET_H_

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4_DISCOVERY_ETH
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_ETH_Exported_Handle
 * @{
 */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
// Packet Gen Macros

/** @defgroup STM32F4_DISCOVERY_ETH_Exported_Constants
  * @{
  */
#define TX_PACKET_SIZE    114

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_ETH_Exported_Functions
  * @{
  */
/* Exported functions ------------------------------------------------------- */
void PacketGen(uint8_t *txptr, uint8_t *payload);
void BSP_ETH_Init(ETH_HandleTypeDef* heth);

/**
  * @}
  */

/**
  * @}
  */


#endif /* __RAWPACKET_H_ */

/* --------------------------------- End Of File ------------------------------ */
