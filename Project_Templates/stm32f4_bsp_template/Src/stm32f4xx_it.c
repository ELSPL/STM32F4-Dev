/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @date    20/04/2015 17:22:47
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */
#include "stm32f4_global.h"
#include "stm32f4_discovery_timer.h"
#include "stm32f4_discovery.h" // Comment it when On board PB is not used
#include "stm32f4_discovery_rtc.h"

#ifdef USE_STM32F4_UART
#include "stm32f4_discovery_uart.h"
#endif

#ifdef USE_STM32F4_VCP
#include "stm32f4_discovery_vcp.h"
#define VCP_SEL 1
#endif

#ifdef USE_STM32F4_HID
#include "stm32f4_discovery_hid.h"
#define HID_SEL 1
#endif

#ifdef USE_STM32F4_MSC
#include "stm32f4_discovery_msc.h"
#endif

#ifdef USE_STM32F4_ASK25
#include "stm32f4_ask25.h"
#endif

#ifdef USE_STM32F4_TFT
#include "stm32f4_discovery_tsc.h"
#endif

#ifdef USE_STM32F4_CAMERA
#include "stm32f4_discovery_camera.h"
#endif

/* Conditional Checking */
#if (VCP_SEL && HID_SEL)
  #error Both VCP and HID are active, Only one can work at a time, so remove any definition.
#endif
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/********************************************************************************************/
#ifdef USE_STM32F4_DISCO

/**
* @brief This function handles USB On The Go FS global interrupt.
*/
void OTG_FS_IRQHandler(void)
{
#ifdef USE_STM32F4_VCP
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_VCP);
#endif  /* USE_STM32F4_VCP */

#ifdef USE_STM32F4_HID
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_HID);
#endif

#ifdef USE_STM32F4_MSC
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
#endif
}


/**
* @brief This function handles EXTI Line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief User GPIO Interrupt Callback
 * @param GPIO_Pin  This parameter determines interrupt pin.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    HAL_GPIO_TogglePin(GPIOD, (1<<12));
  }
}
#endif  /*USE_STM32F4_DISCO*/
/********************************************************************************************/


#ifdef USE_STM32F4_ASK25

#if USE_BUTTON_INT
/**
* @brief This function handles EXTI Line4 interrupt.
*/
void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}


/**
* @brief This function handles EXTI Line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}


/**
 * @brief User GPIO Interrupt Callback
 * @param GPIO_Pin  This parameter determines interrupt pin.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_4)
  {
    HAL_GPIO_TogglePin(GPIOE,(1<<8));
  }
  else if(GPIO_Pin == GPIO_PIN_5)
  {
    HAL_GPIO_TogglePin(GPIOE,(1<<9));
  }
  else if(GPIO_Pin == GPIO_PIN_6)
  {
    HAL_GPIO_TogglePin(GPIOE,(1<<10));
  }
  else if(GPIO_Pin == GPIO_PIN_7)
  {
    HAL_GPIO_TogglePin(GPIOE,(1<<11));
  }
}
#endif

/**
* @brief This function handles I2C1 error interrupt.
*/
void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1_at24c16);
}

/**
* @brief This function handles I2C1 event interrupt.
*/
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1_at24c16);
}

/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi1_25aa160a);
}
#endif  /* USE_STM32F4_ASK25 */
/********************************************************************************************/


#ifdef USE_STM32F4_UART

#ifndef POLLING_MODE
/**
* @brief This function handles USART6 global interrupt.
*/
void USART6_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart6);
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  UartReady = 1;
}
#endif  /* Interrupt Mode */

#ifdef DMA_MODE
/**
* @brief This function handles DMA2 Stream6 global interrupt.
*/
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
}

/**
* @brief This function handles DMA2 Stream1 global interrupt.
*/
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
//  HAL_UART_Transmit_DMA(&huart6, &read , 1);
}
#endif  /* DMA Mode */

#endif  /* USE_STM32F4_UART */
/********************************************************************************************/


#ifdef USE_STM32F4_TFT
/**
* @brief This function handles DMA1 Stream6 global interrupt.
*/
void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}

/**
* @brief This function handles DMA1 Stream0 global interrupt.
*/
void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}
#endif  /* USE_STM32F4_TFT */
/********************************************************************************************/


#ifdef USE_STM32F4_CAMERA
/**
* @brief This function handles DCMI global interrupt.
*/
void DCMI_IRQHandler(void)
{
  HAL_DCMI_IRQHandler(&hdcmi_camera);
}

/**
* @brief This function handles DMA2 Stream1 global interrupt.
*/
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_dcmi);
}

/**
  * @brief  Error DCMI callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_ErrorCallback could be implemented in the user file
   */
}

/**
  * @brief  Line Event callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_LineEventCallback could be implemented in the user file
   */
}

/**
  * @brief  VSYNC Event callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_VsyncEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Frame Event callback.
  * @param  hdcmi: pointer to a DCMI_HandleTypeDef structure that contains
  *                the configuration information for DCMI.
  * @retval None
  */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}
#endif /* USE_STM32F4_CAMERA */
/********************************************************************************************/

/**
* @brief This function handles TIM1 Trigger and Commutation interrupts and TIM11 global interrupt.
*/
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1_mcpwm);
  HAL_TIM_IRQHandler(&htim11_cap);
}

/**
* @brief This function handles TIM1 Break interrupt and TIM9 global interrupt.
*/
void TIM1_BRK_TIM9_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1_mcpwm);
  HAL_TIM_IRQHandler(&htim9_pcap);
}

/**
* @brief This function handles TIM1 Update interrupt and TIM10 global interrupt.
*/
void TIM1_UP_TIM10_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1_mcpwm);
  HAL_TIM_IRQHandler(&htim10_cap);
}


/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2_gen);
}

/**
* @brief This function handles TIM3 global interrupt.
*/
void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3_gen);
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim4_gen);
}

/**
* @brief This function handles TIM5 global interrupt.
*/
void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim5_gen);
}

/**
* @brief This function handles TIM7 global interrupt.
*/
void TIM7_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim7_basic);
}


/**
* @brief This function handles TIM8 Trigger and Commutation interrupts and TIM14 global interrupt.
*/
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim8_mcpwm);
  HAL_TIM_IRQHandler(&htim14_cap);
}

/**
* @brief This function handles TIM8 Break interrupt and TIM12 global interrupt.
*/
void TIM8_BRK_TIM12_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim8_mcpwm);
  HAL_TIM_IRQHandler(&htim12_pcap);
}

/**
* @brief This function handles TIM8 Update interrupt and TIM13 global interrupt.
*/
void TIM8_UP_TIM13_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim8_mcpwm);
  HAL_TIM_IRQHandler(&htim13_cap);
}

/**
 * @brief Timer Capture Callback function for Frequency Calculations
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  __HAL_TIM_DISABLE(htim);

  if(htim->Instance == TIM9 || htim->Instance == TIM12)
  {
    /* Get the Input Capture value */
//    if(tim9_12_flag == ENABLE)
      uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//    else
//      uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    if (uwIC2Value != 0)
    {
      /* Duty cycle computation */
//      if(tim9_12_flag == ENABLE)
        uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2)) * 100) / uwIC2Value;
//      else
//        uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)) * 100) / uwIC2Value;

      /* uwFrequency computation
      TIM4 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */
      if(htim->Instance == TIM9)
        uwFrequency1 = (168000000UL)/(TimCapPsc+1) / uwIC2Value;
      else if(htim->Instance == TIM12)
        uwFrequency1 = (168000000UL)/(2*(TimCapPsc+1)) / uwIC2Value;
    }
    else
    {
      uwDutyCycle = 0;
      uwFrequency1 = 0;
    }
  }
  else if(htim->Instance == TIM10 || htim->Instance == TIM11 || htim->Instance == TIM14 || htim->Instance == TIM13)
  {
    if(uhCaptureIndex == 0)
    {
      /* Get the 1st Input Capture value */
      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      uhCaptureIndex = 1;
    }
    else if(uhCaptureIndex == 1)
    {
      /* Get the 2nd Input Capture value */
      uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

      /* Capture computation */
      if (uwIC2Value2 > uwIC2Value1)
      {
        uwDiffCapture = (uwIC2Value2 - uwIC2Value1);
      }
      else  /* (uwIC2Value2 <= uwIC2Value1) */
      {
        uwDiffCapture = ((TimCapPeriod - uwIC2Value1) + uwIC2Value2);
      }

      /* Frequency computation: for this example TIMx (TIM1) is clocked by
         2xAPB2Clk */
      uwFrequency = (2*HAL_RCC_GetPCLK2Freq()) / uwDiffCapture;
      if(htim->Instance == TIM10 || htim->Instance == TIM11)
        uwFrequency = uwFrequency/(TimCapPsc+1);
      else
        uwFrequency = uwFrequency/(2*(TimCapPsc+1));
      uhCaptureIndex = 0;
    }
  }

  __HAL_TIM_ENABLE(htim);
}


/**
* @brief This function handles RTC Wakeup interrupt through the EXTI Line22 interrupt.
*/
void RTC_WKUP_IRQHandler(void)
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc_bsp);
}

/**
* @brief This function handles RTC Alarms (A and B) through EXTI Line17 interrupt.
*/
void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&hrtc_bsp);
}

/**
* @brief This function handles RTC Tamper and TimeStamp interrupts through EXTI Line21 interrupt.
*/
void TAMP_STAMP_IRQHandler(void)
{
  HAL_RTCEx_TamperTimeStampIRQHandler(&hrtc_bsp);
}

/********************************************************************************************/

/* USER CODE END 1 */
/******END OF FILE*******/
