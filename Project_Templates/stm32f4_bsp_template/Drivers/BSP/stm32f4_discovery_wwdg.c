/******************************************************************//**
* @file   stm32f4_discovery_wwdg.c
* @brief  This file contains implementation of STM32F4 WWDG in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_wwdg.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WWDG
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WWDG_Public_Types
 * @{
 */

WWDG_HandleTypeDef hwwdg_bsp;


/**
 * @} STM32F4_DISCOVERY_WWDG_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WWDG_Private_Functions
 * @{
 */

/**
 * @brief This function enable the WWDG by enabling the peripheral clock
 * @param hwwdg
 *        @arg hwwdg_bsp WWDG handle
 */
static void BSP_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{
  if(hwwdg->Instance==WWDG)
  {
    /* Peripheral clock enable */
    __WWDG_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(WWDG_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
  }
}
/**
 * @} STM32F4_DISCOVERY_WWDG_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WWDG_Public_Functions
 * @{
 */

/**
 * @brief WWDG initialization and configuration function
 * @param WWDG_Prescaler prescaler value of the WWDG
 *        @arg WWDG_PRESCALER_1
 *        @arg WWDG_PRESCALER_2
 *        @arg WWDG_PRESCALER_4
 *        @arg WWDG_PRESCALER_8
 * @param WindowSize     WWDG window value to be compared to the downcounter
 *        @arg value between 64 to 127
 * @param CounterSize    WWDG free-running downcounter value
 *        @arg value between 80 to 127
 */
void BSP_WWDG_Init(uint16_t WWDG_Prescaler, uint8_t WindowSize, uint8_t CounterSize)
{
  hwwdg_bsp.Instance = WWDG;
  hwwdg_bsp.Init.Prescaler = WWDG_Prescaler;
  hwwdg_bsp.Init.Window = WindowSize;
  hwwdg_bsp.Init.Counter = CounterSize;

  BSP_WWDG_MspInit(&hwwdg_bsp);
  HAL_WWDG_Init(&hwwdg_bsp);
}

/**
 * @brief This function disable the WWDG by disabling the peripheral clock
 * @param hwwdg
 *        @arg hwwdg_bsp  WWDG handle
 */
void BSP_WWDG_MspDeInit(WWDG_HandleTypeDef* hwwdg)
{

  if(hwwdg->Instance==WWDG)
  {
    /* Peripheral clock disable */
    __WWDG_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(WWDG_IRQn);
  }
}

/**
 * @} STM32F4_DISCOVERY_WWDG_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_WWDG End
 */

 /* --------------------------------- End Of File ------------------------------ */
