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
#define MOUSE_WINDOW_X                  7
#define MOUSE_WINDOW_Y                  7
#define MOUSE_WINDOW_X_MAX              181
#define MOUSE_WINDOW_Y_MIN              101
#define MOUSE_WINDOW_HEIGHT             213
#define MOUSE_WINDOW_WIDTH              293

int16_t  x_loc  = 0, y_loc  = 0;
__IO int16_t  prev_x = 0, prev_y = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void USR_MOUSE_Init (void);
void HID_MOUSE_UpdatePosition (int8_t x, int8_t y);
void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data);
void USBH_MouseDemo(USBH_HandleTypeDef *phost);

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
   USR_MOUSE_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BSP_USB_HOST_Process();

    if(Appli_state == APPLICATION_READY)
    {
      USBH_MouseDemo(&hUsbHostHID);
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
void USBH_MouseDemo(USBH_HandleTypeDef *phost)
{
  HID_MOUSE_Info_TypeDef *m_pinfo;

  m_pinfo = USBH_HID_GetMouseInfo(phost);
  if(m_pinfo != NULL)
  {
    /* Handle Mouse data position */
    USR_MOUSE_ProcessData(&mouse_info);

    if(m_pinfo->buttons[0])
    {
//      HID_MOUSE_ButtonPressed(0);
    }
    else
    {
//      HID_MOUSE_ButtonReleased(0);
    }

    if(m_pinfo->buttons[1])
    {
//      HID_MOUSE_ButtonPressed(2);
    }
    else
    {
//      HID_MOUSE_ButtonReleased(2);
    }

    if(m_pinfo->buttons[2])
    {
//      HID_MOUSE_ButtonPressed(1);
    }
    else
    {
//      HID_MOUSE_ButtonReleased(1);
    }
  }
}


void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data)
{
  if((data->x != 0) && (data->y != 0))
  {
    HID_MOUSE_UpdatePosition(data->y , data->x);
  }
}


void HID_MOUSE_UpdatePosition (int8_t x, int8_t y)
{
   /* Set the color for button press status */
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

  if ((x != 0) || (y != 0)) {

    x_loc += x/2;
    y_loc += y/2;

    if (y_loc > MOUSE_WINDOW_WIDTH - 12){
      y_loc = MOUSE_WINDOW_WIDTH - 12;
    }
    if (x_loc > MOUSE_WINDOW_HEIGHT - 10){
      x_loc = MOUSE_WINDOW_HEIGHT - 10;
    }
    if (y_loc < 2){
      y_loc = 2;
    }
    if (x_loc < 2){
      x_loc = 2;
    }

    if ((prev_x != 0) && (prev_y != 0))
    {
      BSP_LCD_DisplayChar(MOUSE_WINDOW_Y + prev_y, MOUSE_WINDOW_X + prev_x, ' ');
    } else if((prev_x == 0) && ( prev_y == 0)) {

      BSP_LCD_DisplayChar(MOUSE_WINDOW_Y + 1,
                                MOUSE_WINDOW_X + 1,
                                ' ');
    }

    BSP_LCD_DisplayChar(MOUSE_WINDOW_Y + y_loc ,\
      MOUSE_WINDOW_X + x_loc, 'x');

    prev_x = x_loc;
    prev_y = y_loc;
  }
}

void USR_MOUSE_Init (void)
{
  BSP_LCD_DisplayStringAtLine( LCD_PIXEL_HEIGHT - 42, "                                   ");
  BSP_LCD_DisplayStringAtLine( LCD_PIXEL_HEIGHT - 30, "                                   ");

  /* Display Mouse Window */
  BSP_LCD_DrawRect(8,
                   8,
                   295,
                   225);

//  HID_MOUSE_ButtonReleased(0);
//  HID_MOUSE_ButtonReleased(1);
//  HID_MOUSE_ButtonReleased(2);

  BSP_LCD_SetTextColor(Green);
  BSP_LCD_SetTextColor(Black);

  BSP_LCD_DisplayChar(MOUSE_WINDOW_Y + 1, MOUSE_WINDOW_X + 1, 'x');
  x_loc  = 0;
  y_loc  = 0;
  prev_x = 0;
  prev_y = 0;
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
