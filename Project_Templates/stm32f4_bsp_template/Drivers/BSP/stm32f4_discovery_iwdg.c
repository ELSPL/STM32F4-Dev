/******************************************************************//**
* @file   stm32f4_discovery_iwdg.c
* @brief  This file contains implementation of STM32F4 IWDG in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_iwdg.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_IWDG
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_IWDG_Public_Types
 * @{
 */

IWDG_HandleTypeDef hiwdg_bsp;


/**
 * @} STM32F4_DISCOVERY_IWDG_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_IWDG_Private_Functions
 * @{
 */

/**
 * @} STM32F4_DISCOVERY_IWDG_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_IWDG_Public_Functions
 * @{
 */

/**
 * @brief IWDG initialization and configuration function
 * @param IWDG_Prescaler prescaler of the IWDG
 *                                    Min timeout (ms)   Max timeout (ms)
 *                                    0x000              0xFFF
 *        @arg IWDG_PRESCALER_4           0.125              512
 *        @arg IWDG_PRESCALER_8           0.25               1024
 *        @arg IWDG_PRESCALER_16          0.5                2048
 *        @arg IWDG_PRESCALER_32          1                  4096
 *        @arg IWDG_PRESCALER_64          2                  8192
 *        @arg IWDG_PRESCALER_128         4                  16384
 *        @arg IWDG_PRESCALER_256         8                  32768
 * @param ReloadCount    the IWDG down-counter reload value
 *        @arg Min_Data = 0 and Max_Data = 0x0FFF
 */
void BSP_IWDG_Init(uint16_t IWDG_Prescaler, uint16_t ReloadCount)
{
  hiwdg_bsp.Instance = IWDG;
  hiwdg_bsp.Init.Prescaler = IWDG_Prescaler;
  hiwdg_bsp.Init.Reload = ReloadCount;
  HAL_IWDG_Init(&hiwdg_bsp);
}


/**
 * @} STM32F4_DISCOVERY_IWDG_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_IWDG End
 */

 /* --------------------------------- End Of File ------------------------------ */
