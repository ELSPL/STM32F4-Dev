/******************************************************************//**
* @file   main.c
* @brief  Write a program to use HIDs via USB
* @version  v1.0
* @date   Apr 16, 2015
* @author Bhavin R. Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_global.h"
//#include "stm32f4_discovery_vcp.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4_discovery_hid.h"
//#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"




/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define KYBRD_FIRST_COLUMN               (uint16_t)7
#define KYBRD_LAST_COLUMN                (uint16_t)300
#define KYBRD_FIRST_LINE                 (uint8_t)10
#define KYBRD_LAST_LINE                  (uint8_t)220
#define SMALL_FONT_COLUMN_WIDTH          13
#define SMALL_FONT_LINE_WIDTH            20

uint8_t KeybrdCharXpos = 0;
uint16_t KeybrdCharYpos = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void  USR_KEYBRD_Init (void);
void  USR_KEYBRD_ProcessData (uint8_t data);
void USBH_KeybdDemo(USBH_HandleTypeDef *phost);
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
   BSP_HostHID_Init();

  /* Initialize the LCD */
   BSP_LCD_Init();

  /* Clear the LCD */
   BSP_LCD_Clear(White);
   /* Initialize Keyboard */
   USR_KEYBRD_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BSP_USB_HOST_Process();

    if(Appli_state == APPLICATION_READY)
    {
      USBH_KeybdDemo(&hUsbHostHID);
    }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
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

}

/* USER CODE BEGIN 4 */
void USBH_KeybdDemo(USBH_HandleTypeDef *phost)
{
  HID_KEYBD_Info_TypeDef *k_pinfo;
  char c;
  k_pinfo = USBH_HID_GetKeybdInfo(phost);

  if(k_pinfo != NULL)
  {
    c = USBH_HID_GetASCIICode(k_pinfo);
    if(c != 0)
    {
      USR_KEYBRD_ProcessData(c);
    }
  }
}

void  USR_KEYBRD_ProcessData (uint8_t data)
{
  if (data == '\n') {
    KeybrdCharYpos = KYBRD_FIRST_COLUMN;

    /*Increment char X position*/
    KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;

  } else if (data == '\r') {
    /* Manage deletion of charactter and upadte cursor location*/
    if ( KeybrdCharYpos == KYBRD_FIRST_COLUMN)  {
      /*First character of first line to be deleted*/
      if (KeybrdCharXpos == KYBRD_FIRST_LINE) {
        KeybrdCharYpos =KYBRD_FIRST_COLUMN;
      } else {
        KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;
        KeybrdCharYpos =(KYBRD_FIRST_COLUMN+SMALL_FONT_COLUMN_WIDTH);
      }
    } else {
      KeybrdCharYpos +=SMALL_FONT_COLUMN_WIDTH;

    }
    BSP_LCD_DisplayChar(KeybrdCharXpos,KeybrdCharYpos, ' ');
  } else {
    BSP_LCD_DisplayChar(KeybrdCharYpos,KeybrdCharXpos, data);
    /* Update the cursor position on LCD */

    /*Increment char Y position*/
    KeybrdCharYpos +=SMALL_FONT_COLUMN_WIDTH;

    /*Check if the Y position has reached the last column*/
    if (KeybrdCharYpos >= KYBRD_LAST_COLUMN) {
      KeybrdCharYpos = KYBRD_FIRST_COLUMN;

      /*Increment char X position*/
      KeybrdCharXpos+=SMALL_FONT_LINE_WIDTH;
    }
  }

  if (KeybrdCharXpos >= KYBRD_LAST_LINE) {
    KeybrdCharXpos = KYBRD_FIRST_LINE;
  }
}

void  USR_KEYBRD_Init (void)
{
  BSP_LCD_DisplayStringAtLine( LCD_PIXEL_HEIGHT - 42, "                                   ");
  BSP_LCD_DisplayStringAtLine( LCD_PIXEL_HEIGHT - 30, "                                   ");

  BSP_LCD_SetTextColor(Black);

  KeybrdCharXpos = KYBRD_FIRST_LINE;
  KeybrdCharYpos = KYBRD_FIRST_COLUMN;
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
