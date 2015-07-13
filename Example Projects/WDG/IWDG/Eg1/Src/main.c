/******************************************************************//**
* @file   main.c
* @brief  Write a program to understand independent watch dog timer.
* @version  v1.0
* @date   Apr 16, 2015
* @author Bhavin R. Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_global.h"
//#include "stm32f4_discovery_uart.h"
//#include "stm32f4_discovery_vcp.h"
#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"
#include "stm32f4_discovery_wdg.h"



/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
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
  BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_EXTI);    // initialize on board switch
  BSP_LED_Init(LED5);                          // initialize on baord LED 4 and LED 5
  BSP_LED_Init(LED4);

  /* Check if the system has resumed from WWDG reset */
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
  {
    /* WWDGRST flag set: Turn LED1 on */
    BSP_LED_On(LED5);

    /* Clear reset flags */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
  else
  {
    /* WWDGRST flag is not set: Turn LED1 off */
    BSP_LED_Off(LED5);
  }
  /* Initialize and start IWDG */
  BSP_IWDG_Init(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    BSP_LED_Toggle(LED4);

    HAL_Delay(1850);
    /* Refresh the IWDG reload count value */
    HAL_IWDG_Refresh(&hiwdg_bsp);
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_LSI;  // uncomment when LSI clock is used for RTC
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;        // uncomment when LSI Clock is used for RTC
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

#ifdef USE_STM32F4_RTC
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8;  // Comment this line if you want to use LSI clock
//  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;     // Remove comment if you want to use LSI clock
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
#endif /* USE_STM32F4_RTC */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  *(__IO uint32_t *) 0xA0001000 = 0xFF;
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
