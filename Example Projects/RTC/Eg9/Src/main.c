/******************************************************************//**
* @file   main.c
* @brief  Write a program to use RTC as stopwatch with 10 time lapse storage.
* @version  v1.0
* @date   May 30, 2015
* @author Bhavin R. Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_global.h"
#include "stm32f4_discovery_uart.h"
#include "stm32f4_discovery_vcp.h"
//#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"
#include "stm32f4_discovery_rtc.h"



/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
__IO uint16_t msec, sec = 0, min = 0;
__IO uint8_t stopwatch_state = 0, lap = 1;
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  uint32_t mask_msec,mask_sec,mask_min,laps;
  trace_printf("Hello\n");
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */

  /* Initialize RTC */
  BSP_RTC_Init();
  BSP_RTC_WakeUpTimer_Init(1);
  /* Initialize UART with 115200 baud rate */
  BSP_UART_Init(115200);

  uprintf("Stopwatch using RTC\n\r");
  uprintf("Press s to start stopwatch\n\r"); //  s to start stopwatch
  uprintf("Press x to stop stopwatch\n\r"); // x to stop stopwatch
  uprintf("Press l for Laps\n\r"); // laps 1 to 10
  uprintf("Press r to reset stopwatch\n\r"); // r to reset stopwatch
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    switch (ugetche(NONE_BLOCKING))
    {
    case 's':
      min = 0;
      sec = 0;
      stopwatch_state = 1;
      break;
    case 'x':
      stopwatch_state = 2;
      break;
    case 'r':
      stopwatch_state = 3;
      break;
    case 'l':
      HAL_RTCEx_BKUPWrite(&hrtc_bsp, lap, (msec/10 | sec<<8 | min<<16));
      mask_msec = HAL_RTCEx_BKUPRead(&hrtc_bsp,lap) & 0x000000FF;
      mask_sec = (HAL_RTCEx_BKUPRead(&hrtc_bsp,lap) & 0x0000FF00)>>8;
      mask_min = (HAL_RTCEx_BKUPRead(&hrtc_bsp,lap) & 0x00FF0000)>>16;
      uprintf("\x1b[8;1H Lap%d02", lap);
      uprintf("\x1b[8;7H %d02 : %d02 : %d02", mask_min,mask_sec,mask_msec);
      lap++;
      if (lap == 11)
      {
        lap = 1;
        for (laps=1; laps<11; laps++)
        {
          mask_msec = HAL_RTCEx_BKUPRead(&hrtc_bsp,laps) & 0x000000FF;
          mask_sec = (HAL_RTCEx_BKUPRead(&hrtc_bsp,laps) & 0x0000FF00)>>8;
          mask_min = (HAL_RTCEx_BKUPRead(&hrtc_bsp,laps) & 0x00FF0000)>>16;
          uprintf(" \n\rLap%d02", laps);
          uprintf("\t%d02 : %d02 : %d02", mask_min,mask_sec,mask_msec);
        }
        stopwatch_state = 2;
      }
      break;
    }

    if(stopwatch_state == 1)
    {
      __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT();
      __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc_bsp,RTC_FLAG_WUTF);
      uprintf("\x1b[7;1H %d02 : %d02 : %d02",min,sec,msec/10);
    }
    if (stopwatch_state == 2)
    {
      __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_IT();
      uprintf("\x1b[7;1H %d02 : %d02 : %d02",min,sec,msec/10);
    }
    if (stopwatch_state == 3)
    {

      min = 0;
      sec = 0;
      msec =0;
      uprintf("\x1b[7;1H %d02 : %d02 : %d02",min,sec,msec);
    }
  }
  /* USER CODE END 3 */

}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  //add RCC_OSCILLATORTYPE_LSI for LSI Clock
//  RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;        // Added for LSI Clock
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8;  //Comment this line if you want to use LSI clock
//  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;     // Remove comment if you want to use LSI clock
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  msec++;
  if (msec == 999)
  {
    msec = 0;
    sec++;
  }
  if(sec == 60)
  {
    sec = 0;
    min++;
  }
  if (min == 60)
  {
    stopwatch_state == 2;
    min = 0;
    sec = 0;
    msec = 0;
  }

}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/*****END OF FILE****/
