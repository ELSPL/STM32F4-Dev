/******************************************************************//**
* @file   main.c
* @brief  Write a program to interface Discover-WIFI
* @version  v1.0
* @date   Apr 16, 2015
* @author Bhavin R. Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_global.h"
#include "stm32f4_discovery_uart.h"
//#include "stm32f4_discovery_vcp.h"
//#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"
#include "stm32f4_discovery_wifi.h"
#include <string.h>


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/*****************************************************************************
 * This is Discover-WIFI interface example in which
 *
 *   0: Get WiFi status  - It will give current WIFI status like WIFI On or
 *                         Off, join to any network
 *   1: Wifi Scan - It will show available wifi network in its range
 *   2: Wifi Join - To connect the available WIFI network
 *   3: Get IP - It will give the current IP address of the Discover-WIFI
 *               0 - in STA mode(when Discover-WIFI is connected to any network )
 *               1 - in AP mode(Discover-WIFI works as Access Point)
 *   4: TCP client - Discover-WIFI works as TCP client
 *   5: TCP sever - Discover-WIFI works as TCP server
 *   6: Send from sock - To send data
 *   7: WiFi Leave - Disconnect from the connected network
 *   8: AP On/Off - Access Point mode On or Off
 *   9: UDP client - Discover-WIFI works as UDP client
 *   a: UDP server - Discover-WIFI works as UDP Server
 *   b: Wifi Off - To turn on the Discover-WIFI
 *   c: Wifi On - To turn off the Discover-WIFI
 *   m: Show Menu - It will show the menu
 *   q: Quit - To Quit from the Discover-WIFI menu
 *
 ******************************************************************************/

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  trace_printf("Hello\n");
  uint8_t ch;
  uint32_t DBGU_RxBufferTail = 0;
  uint32_t DBGU_RxBufferHead = 0;
  uint8_t DBGU_RxBuffer[DBGU_RX_BUFFER_SIZE];
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  BSP_UART_Init(115200);
  SN8200_HAL_Init(921600);
  uprintf("\n\rHello!\n\r");
  uprintf("\n\r");

  WifiOn(seqNo++);
  uprintf("\n\r");

  ShowMenu();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
    if(HAL_UART_Receive(&huart6,&ch,1,100) == HAL_OK)
    {
      switch (ch)
      {
       case 0x7F:
         if(DBGU_RxBufferHead != DBGU_RxBufferTail)
         {
           DBGU_RxBufferHead = (DBGU_RxBufferHead - 1) % DBGU_RX_BUFFER_SIZE;
           HAL_UART_Transmit_IT(&huart6,0x7F,1);
         }
         break;

       case 0x0D:
         DBGU_RxBuffer[DBGU_RxBufferHead] = ch;
         DBGU_RxBufferHead = (DBGU_RxBufferHead + 1) % DBGU_RX_BUFFER_SIZE;
         DBGU_InputReady = 1;
         break;

       case In_BACKSPACE:
         HAL_UART_Transmit_IT(&huart6,&ch,1);
         break;

       default:
         DBGU_RxBuffer[DBGU_RxBufferHead] = ch;
         HAL_UART_Transmit_IT(&huart6,&ch,1);
         DBGU_RxBufferHead = (DBGU_RxBufferHead + 1) % DBGU_RX_BUFFER_SIZE;
         key = ch;
         break;
      }
    }
    if(DBGU_InputReady)
    {
      ProcessUserInput();
    }
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(quit_flag)
    {
      break;
    }
  }
  uprintf("Goodbye\n\r");
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
