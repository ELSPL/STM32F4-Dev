/******************************************************************//**
* @file		  LCD_Touch_Calibration.c
* @brief	  LCD touch screen calibration
* @version	V1.0
* @date		  May 8, 2015
* @author	  Dwijay.Edutech learning solutions
**********************************************************************/


/* Includes ------------------------------------------------------------------- */
#include "LCD_Touch_Calibration.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4_discovery_ts_cal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_LCD
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TSC_CAL
  * @brief
  * @{
  */
/* Private define ------------------------------------------------------------*/
/*Calibration accuracy, + /-x pixels*/
#define CALIBRATION_RANGE      (10)

/*1st calibration point  position*/
#define CAL_POINT1_X           (20)
#define CAL_POINT1_Y           (20)

/*2nd calibration point position*/
#define CAL_POINT2_X          (300)
#define CAL_POINT2_Y           (20)

/*3rd calibration point position*/
#define CAL_POINT3_X           (20)
#define CAL_POINT3_Y          (220)

/*4th calibration point position*/
#define CAL_POINT4_X          (300)
#define CAL_POINT4_Y          (220)

/*calibration test point position*/
#define TST_POINT_X           (160)
#define TST_POINT_Y           (120)

#define TOUCH_AD_VALUE_MAX    (4000)
#define TOUCH_AD_VALUE_MIN    (100)

#define ABS(X)  ((X) > 0 ? (X) : -(X))

/* Private variables ---------------------------------------------------------*/
Point_Struct point_new, point_old;
TS_StateTypeDef TS_State;
/*Variables definition for calibration point*/
static Point_Struct point_Base[5] = {
  {CAL_POINT1_X,CAL_POINT1_Y},
  {CAL_POINT2_X,CAL_POINT2_Y},
  {CAL_POINT3_X,CAL_POINT3_Y},
  {CAL_POINT4_X,CAL_POINT4_Y},
  {TST_POINT_X,TST_POINT_Y}
};

LCD_TOUCH_CALIBRATION_EXT Adjust_Struct adjust_Para;
Point_Struct left_upper_point,right_upper_point,left_down_point,right_down_point,tst_point;

/** @defgroup STM32F4_DISCOVERY_TSC_CAL_Functions
  * @{
  */
/**
  * @brief  Lcd_Touch Calibration  test
  * @param  None
  * @retval None
  */
void Lcd_Touch_Calibration(void)
{
#define CURSOR_LEN    (10)
  uint8_t k,i;
  float ratio1,ratio2;
  Point_Struct left_upper_point,right_upper_point,left_down_point,right_down_point,tst_point;
  int tpx_sum = 0,tpy_sum = 0;

  /*Indicates whether Calibration is OK*/
  uint8_t adjust_OK_Falg = 0;
  TS_StateTypeDef *pstate = NULL;

  /* Clear the LCD */
  BSP_LCD_Clear(White);
  BSP_LCD_SetTextColor(Black);
  HAL_Delay(100);
  while (!adjust_OK_Falg) {
    tpx_sum = 0;
    tpy_sum = 0;
    /*wait for Calibration */
    for (k = 0;k < 4;k++) {
      BSP_LCD_DrawLine( point_Base[k].x - CURSOR_LEN, point_Base[k].y,
                        point_Base[k].x + CURSOR_LEN, point_Base[k].y);

      BSP_LCD_DrawLine( point_Base[k].x, point_Base[k].y - CURSOR_LEN,
                        point_Base[k].x, point_Base[k].y + CURSOR_LEN);

      HAL_Delay(100);
      TS_State.x = TS_State.y = 0;
      while((TS_State.x == 0) && (TS_State.y == 0))
      {
        BSP_TS_GetState(&TS_State);
        HAL_Delay(10);
      }
      HAL_Delay(10);
      /*Read AD convert result*/
      for(i = 0; i < 16; i++)
      {
        tpx_sum += TS_State.x;
        tpy_sum += TS_State.y;
        HAL_Delay(2);
      }
      TS_State.x = 0;
      TS_State.y = 0;

      tpx_sum >>= 4;
      tpy_sum >>= 4;

      switch (k) {
        case 0:
          left_upper_point.x = tpx_sum;
          left_upper_point.y = tpy_sum;
          break;

        case 1:
          right_upper_point.x = tpx_sum;
          right_upper_point.y = tpy_sum;
          break;

        case 2:
          left_down_point.x = tpx_sum;
          left_down_point.y = tpy_sum;
          break;

        case 3:
          right_down_point.x = tpx_sum;
          right_down_point.y = tpy_sum;
          break;

        default:
          break;
      }
      HAL_Delay(200);
    }

    ratio1 = (float)((point_Base[1].x - point_Base[0].x) + (point_Base[3].x - point_Base[2].x)) / 2.0;
    ratio2 = (float)((right_upper_point.x - left_upper_point.x) + (right_down_point.x - left_down_point.x)) / 2.0;
    adjust_Para.xScale = ratio1 / ratio2;

    ratio1 = (float)((point_Base[2].y - point_Base[0].y) +
             (point_Base[3].y - point_Base[1].y)) / 2.0;
    ratio2 = (float)((left_down_point.y - left_upper_point.y) +
             (right_down_point.y - right_upper_point.y)) / 2.0;
    adjust_Para.yScale = ratio1 / ratio2;

    ratio1 = (((float)right_upper_point.x * adjust_Para.xScale - (float)point_Base[1].x)
             + ((float)left_upper_point.x * adjust_Para.xScale - (float)point_Base[0].x)) / 2.0;
    adjust_Para.xOffset = (int)ratio1;

    ratio1 = (((float)right_upper_point.y * adjust_Para.yScale - (float)point_Base[1].y)
             + ((float)left_upper_point.y * adjust_Para.yScale - (float)point_Base[0].y)) / 2.0;

    adjust_Para.yOffset = (int)ratio1;

    /*Draw cross sign for calibration points*/
    BSP_LCD_DrawLine(point_Base[4].x - CURSOR_LEN,
                    point_Base[4].y,
                    point_Base[4].x + CURSOR_LEN,
                    point_Base[4].y);

    BSP_LCD_DrawLine(point_Base[4].x,
                    point_Base[4].y - CURSOR_LEN,
                    point_Base[4].x,
                    point_Base[4].y + CURSOR_LEN);

    HAL_Delay(100);
    while((TS_State.x == 0) && (TS_State.y == 0))
    {
      BSP_TS_GetState(&TS_State);
      HAL_Delay(10);
    }
    HAL_Delay(10);
    tpx_sum = 0;
    tpy_sum = 0;
    /*Read AD convert result*/
    for(i = 0; i < 16; i++) {
      tpx_sum += TS_State.x;
      tpy_sum += TS_State.y;
      HAL_Delay(2);
    }
    tpx_sum >>= 4;
    tpy_sum >>= 4;

    tst_point.x = tpx_sum;
    tst_point.y = tpy_sum;

    tst_point.x = (int)(tst_point.x * adjust_Para.xScale - adjust_Para.xOffset);
    tst_point.y = (int)(tst_point.y * adjust_Para.yScale - adjust_Para.yOffset);

    if (tst_point.x > (point_Base[4].x + CALIBRATION_RANGE)
        || tst_point.x < (point_Base[4].x - CALIBRATION_RANGE)
        || tst_point.y > (point_Base[4].y + CALIBRATION_RANGE)
        || tst_point.y < (point_Base[4].y - CALIBRATION_RANGE)) {
      adjust_OK_Falg = 0;
      BSP_LCD_DisplayStringAtLine(8,"   Calibration Fail!   ");
      HAL_Delay(200);
      BSP_LCD_Clear(White);
      HAL_Delay(300);
    } else {
      adjust_OK_Falg = 1;
    }
  }
  BSP_LCD_DisplayStringAtLine(3,"  Calibration OK!  ");
  HAL_Delay(200);
}


void TSC_Read_Cal_Value(Point_Struct* tpoint)
{
  TS_State.x = TS_State.y = 0;
  while((TS_State.x == 0) && (TS_State.y == 0))
  {
    BSP_TS_GetState(&TS_State);
  }

  tpoint->x = TS_State.x;
  tpoint->y = TS_State.y;

  /*Calculate coordinates*/
  tpoint->x = ((int)(tpoint->x * adjust_Para.xScale - adjust_Para.xOffset));
  tpoint->y = ((int)(tpoint->y * adjust_Para.yScale - adjust_Para.yOffset));

  if (tpoint->x >= LCD_PIXEL_WIDTH) {
    tpoint->x = LCD_PIXEL_WIDTH -1;
  }
  if (tpoint->y >= LCD_PIXEL_HEIGHT) {
    tpoint->y = LCD_PIXEL_HEIGHT -1;
  }
}

/**
  * @brief  Display the value of calibration point
  * @param  None
  * @retval None
  */
void Calibration_Test_Dispose(void)
{
  TS_StateTypeDef *pstate = NULL;
  uint8_t text[50];
  uint8_t b_flag = 1;

  /*Init Variables*/
  point_new.x = 0;
  point_new.y = 0;
  point_old.x = 0;
  point_old.y = 0;

  /*Display backgroup of LCD*/
  BSP_LCD_Clear(White);
  BSP_LCD_SetTextColor(Black);

  BSP_LCD_DisplayStringAtLine(9,"please touch the screen");
  while (1) {
    TS_State.x = TS_State.y = 0;
    while((TS_State.x == 0) && (TS_State.y == 0))
    {
      BSP_TS_GetState(&TS_State);
    }
    point_new.x = TS_State.x;
    point_new.y = TS_State.y;

    /* If AD result is abnormal, then LCD show nothing */
    if ((point_new.x > TOUCH_AD_VALUE_MAX)
        || (point_new.x < TOUCH_AD_VALUE_MIN)
        || (point_new.y > TOUCH_AD_VALUE_MAX)
        || (point_new.y < TOUCH_AD_VALUE_MIN)) {
      continue;
    }
    if (b_flag) {
      b_flag = 0;
      BSP_LCD_Clear(White);
    }
    BSP_LCD_SetTextColor(Blue);
    sprintf((char*)text,"   X_AD = %d ",point_new.x);
    BSP_LCD_DisplayStringAtLine(4,text);
    sprintf((char*)text,"   y_AD = %d ",point_new.y);
    BSP_LCD_DisplayStringAtLine(5,text);
    /*Calculate coordinates*/
    point_new.x = ((int)(point_new.x * adjust_Para.xScale - adjust_Para.xOffset));
    point_new.y = ((int)(point_new.y * adjust_Para.yScale - adjust_Para.yOffset));

    if (point_new.x >= LCD_PIXEL_WIDTH) {
      point_new.x = LCD_PIXEL_WIDTH -1;
    }
    if (point_new.y >= LCD_PIXEL_HEIGHT) {
      point_new.y = LCD_PIXEL_HEIGHT -1;
    }
    sprintf((char*)text,"   X_POS = %d ", point_new.x);
    BSP_LCD_DisplayStringAtLine(7,text);
    sprintf((char*)text,"   y_POS = %d ", point_new.y);
    BSP_LCD_DisplayStringAtLine(8,text);
    BSP_LCD_SetTextColor(Red);
    /*Judge whether the inut point is the 1st point*/
    if ((point_old.x == 0) && (point_old.y == 0)) {
      point_old.x = point_new.x;
      point_old.y = point_new.y;
    } else {
      BSP_LCD_DrawLine(point_old.x, point_old.y, point_new.x, point_new.y);
      point_old.x = point_new.x;
      point_old.y = point_new.y;
    }
  }
}


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
 /****************************************** End Of File **************************************/
