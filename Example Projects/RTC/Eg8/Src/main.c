/******************************************************************//**
* @file   main.c
* @brief  Write a program to demonstrate RTC Tamper
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
#include "stm32f4_discovery.h"
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
uint8_t tamper_test = RESET;

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  trace_printf("Hello\n");
  RTC_TimeTypeDef Tim;
  RTC_DateTypeDef Dat;
  uint8_t bkupreg = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* Initialize LED 4 */
  BSP_LED_Init(LED4);
  /* Initialize RTC */
  BSP_RTC_Init();

  BSP_RTC_Tamper_Init(RTC_TAMPERTRIGGER_RISINGEDGE);  //Time stamp event detection on PIN C13 Rising edge mode
  /* Initialize UART with 115200 baud rate */
  BSP_UART_Init(115200);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&hrtc_bsp,RTC_FLAG_TSF);
    __HAL_RTC_TAMPER_CLEAR_FLAG(&hrtc_bsp,RTC_FLAG_TAMP1F);

    if ((hrtc_bsp.Instance->TAFCR & RTC_TAMPER_1) == RESET) //check if TAMPER is disable or not
    {
      __HAL_RTC_TAMPER1_ENABLE(&hrtc_bsp);
    }

    for(bkupreg = 0; bkupreg < 20; bkupreg++)  // Write data to Back Up registers
    {
      HAL_RTCEx_BKUPWrite(&hrtc_bsp,bkupreg,(bkupreg));
    }
      /* Check Data is stored on the Back Up registers */
    for(bkupreg = 0; bkupreg < 20; bkupreg++)
    {
      if(HAL_RTCEx_BKUPRead(&hrtc_bsp,bkupreg != (bkupreg)))
      {
        HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
      }
    }
    while (tamper_test == RESET)
    {
      HAL_RTC_GetTime(&hrtc_bsp,&Tim,FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc_bsp,&Dat,FORMAT_BIN);
      Tim.SubSeconds = 1000 - ((hrtc_bsp.Instance->SSR*1000)/hrtc_bsp.Init.SynchPrediv);                // Count msec
      uprintf("\x1b[3;1H %d02 : %d02 : %d02 : %d04",Tim.Hours,Tim.Minutes,Tim.Seconds,Tim.SubSeconds);
      uprintf("\x1b[4;1H %d02 / %d02 / %d04", Dat.Date,Dat.Month,Dat.Year);
    }

    __HAL_RTC_TAMPER1_DISABLE(&hrtc_bsp);

    for(bkupreg = 0; bkupreg < 20; bkupreg++)  // Read Back Up registers after tamper event detection
    {
      if(HAL_RTCEx_BKUPRead(&hrtc_bsp,bkupreg != 0x0))
      {
        BSP_LED_Toggle(LED5);
      }
    }
    tamper_test = RESET;

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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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
void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
{
  BSP_LED_Toggle(LED4);
  tamper_test = SET;
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
