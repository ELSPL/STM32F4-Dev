/******************************************************************//**
* @file   stm32f4_discovery_wdg.c
* @brief  This file contains implementation of STM32F4 WWDG and IWDG in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_wdg.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WDG
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WDG_Public_Types
 * @{
 */

WWDG_HandleTypeDef hwwdg_bsp;         // window watchdog handle
IWDG_HandleTypeDef hiwdg_bsp;         // independent watchdog handle

uint8_t wwdgflag = RESET;             // Watchdog flag used for wwdg interrupt in user application

/**
 * @} STM32F4_DISCOVERY_WDG_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WDG_Private_Functions
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
    HAL_NVIC_SetPriority(WWDG_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
  }
}

/**
 * @} STM32F4_DISCOVERY_WDG_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WDG_Public_Functions
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
/*********************************************************************************
 * WWDG clock counter = (PCLK1 (42MHz)/4096)/8) = 1281 Hz (~780 us)
 * WWDG Window value = 80 means that the WWDG counter should be refreshed only
 * when the counter is below 80 (and greater than 64) otherwise a reset will
 * be generated.
 * WWDG Counter value = 127, WWDG timeout = ~780 us * 64 = 49.9 ms
 *********************************************************************************/
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
 * @brief IWDG initialization and configuration function
 * @param ResetTime_ms    the IWDG target board reset time
 *        @arg 0.125ms to 32768ms
 */
void BSP_IWDG_Init(uint16_t ResetTime_ms)
{
  hiwdg_bsp.Instance = IWDG;
  if(ResetTime_ms < 4096)
  {
    if (ResetTime_ms < 512)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_4;
      hiwdg_bsp.Init.Reload = ((LSI_VALUE/4000)*ResetTime_ms) - 1;
    }
    else if (ResetTime_ms < 1024)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_8;
      hiwdg_bsp.Init.Reload = ((LSI_VALUE/8000)*ResetTime_ms) - 1;
    }
    else if (ResetTime_ms < 2048)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_16;
      hiwdg_bsp.Init.Reload = ((LSI_VALUE/16000)*ResetTime_ms) - 1;
    }
    else if (ResetTime_ms < 4096)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_32;
      hiwdg_bsp.Init.Reload = ((float)(LSI_VALUE/32000)*ResetTime_ms) - 1;
    }
  }
  else if (ResetTime_ms > 4096)
  {
    if (ResetTime_ms < 8192)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_64;
      hiwdg_bsp.Init.Reload = ((float)(LSI_VALUE/64000)*ResetTime_ms) - 1;
    }
    else if (ResetTime_ms < 16384)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_128;
      hiwdg_bsp.Init.Reload = ((float)(LSI_VALUE/128000)*ResetTime_ms) - 1;
    }
    else if (ResetTime_ms < 32769)
    {
      hiwdg_bsp.Init.Prescaler = IWDG_PRESCALER_256;
      hiwdg_bsp.Init.Reload = ((float)(LSI_VALUE/256000)*ResetTime_ms) - 1;
    }
  }
  HAL_IWDG_Init(&hiwdg_bsp);

  /* Start IWDG */
  HAL_IWDG_Start(&hiwdg_bsp);
}

/**
 * @} STM32F4_DISCOVERY_WDG_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_WDG End
 */

 /* --------------------------------- End Of File ------------------------------ */
