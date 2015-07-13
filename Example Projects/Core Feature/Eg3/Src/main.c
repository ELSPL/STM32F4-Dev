/******************************************************************//**
* @file   main.c
* @brief  Write a program to change privilege to unprivilege mode and vice versa.
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
#include "stm32f4_discovery_vcp.h"
#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"




/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//SVC_Handler(void);

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

  /* Initialize UART */
  BSP_UART_Init(115200);
  uprintf("LED5 (PD.14 RED) will be turned on in privilege mode \n\r");
  uprintf("LED6 (PD.15 BLUE) will be turned on in un-privilege mode\n\r");
  /* Initialize On Board LEDs */
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  BSP_LED_Init(LED4);

  /* Thread mode is privilege out of reset */
  /* First checking Thread mode is privilege or un-privilege */
  if(BSP_Check_Priviledge_Status() == THREAD_PRIVILEGED)
  {
    uprintf("\n\rThread mode is privileged!\n\n\r");
    BSP_LED_On(LED5);
  }
  else
  {
    uprintf("Thread mode is unprivileged! It's not compliant with Cortex-M3 Technical\n\r");
    while(1);             //Error loop
  }

  uprintf("Press '1' to change to unprivilege mode ...\n\r");
  ugetche(BLOCKING);   // Wait for input data '1'

  /* Changing to un-privilege mode */
  BSP_Set_UnprivilegeMode();
  uprintf("Changed to unprivilege mode!\n\n\r");

  /* Check */
   if(BSP_Check_Priviledge_Status()==THREAD_UNPRIVILEGED)
  {
    uprintf("Check: Thread mode change to unprivilege successful!\n\n\r");
    BSP_LED_Off(LED5);
    BSP_LED_On(LED6);
  }
  else
  {
    uprintf("Check: Thread mode is still privileged! ERROR...\n\r");
    while(1); //Error loop
  }
  /* Wait user press '2' character to change to privilege mode */

  /*
   * As we know to change mode we need to be privilege first
   * and thread mode currently is in un-privilege level so we cannot
   * execute privilege/system commands.
   * By default exception handlers are in privilege level so through
   * SVC exception we will change the Thread mode to Privilege level.
   */
  uprintf("Press '2' to change to privilege mode by calling system call exception...\n\r");
  ugetche(BLOCKING);  // wait for input data '2'

  /* Call system call exception to re-change Thread mode into privilege */
  __SVC();
  uprintf("Called system call exception!\n\r");

  /* Check */
  if(BSP_Check_Priviledge_Status() == THREAD_PRIVILEGED)
  {
    uprintf("Check: Thread mode change to privilege successful!\n\r");
    BSP_LED_Off(LED6);
    BSP_LED_On(LED5);
  }
  else
  {
    uprintf("Check: Thread mode is still unprivileged! ERROR...\n\r");
    while(1); //Error loop
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(500);
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8;  // Comment this line if you want to use LSI clock
//  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;     // Remove comment if you want to use LSI clock
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
#endif /* USE_STM32F4_RTC */
}

/* USER CODE BEGIN 4 */

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
