/**
  ******************************************************************************
  * @file    stm32f4_discovery_ts_cal.c
  * @author  Dwijay.Edutech learning solutions
  * @version V1.0.0
  * @date    25-April-2015
  * @brief   This file includes the Touch Screen driver for (TS_STMPE811)
  ******************************************************************************
  */

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the touch screen module of the STM32F4_DISCO_BB
     board on the SSD2119 LCD mounted on Discovery daughter board .
   - The STMPE811 IO expander device component driver must be included with this
     driver in order to run the TS module commanded by the IO expander device
     mounted on the evaluation board.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the TS module using the BSP_TS_Init() function. This
       function includes the MSP layer hardware resources initialization and the
       communication layer configuration to start the TS use. The LCD size properties
       (x and y) are passed as parameters.
     o If TS interrupt mode is desired, you must configure the TS interrupt mode
       by calling the function BSP_TS_ITConfig(). The TS interrupt mode is generated
       as an external interrupt whenever a touch is detected.

  + Touch screen use
     o The touch screen state is captured whenever the function BSP_TS_GetState() is
       used. This function returns information about the last LCD touch occurred
       in the TS_StateTypeDef structure.
     o If TS interrupt mode is used, the function BSP_TS_ITGetStatus() is needed to get
       the interrupt status. To clear the IT pending bits, you should call the
       function BSP_TS_ITClear().
     o The IT is handled using the corresponding external interrupt IRQ handler,
       the user IT callback treatment is implemented on the same external interrupt
       callback.

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_ts_cal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Types_Definitions
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Variables
  * @{
  */
static TS_DrvTypeDef *ts_driver;
static uint16_t ts_x_boundary, ts_y_boundary;
static uint8_t  ts_orientation;

MATRIX_Type cmatrix;
TS_StateTypeDef gTouch;
__IO uint8_t TReady=FALSE;
__IO uint8_t CalTouch=TRUE;

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Function_Prototypes
  * @{
  */
uint8_t BSP_TS_SetCalibrationMatrix(TS_StateTypeDef *displayPtr, TS_StateTypeDef *screenPtr, MATRIX_Type *matrixPtr);
uint8_t BSP_TS_Cal_Values(TS_StateTypeDef *displayPtr, MATRIX_Type *matrixPtr);

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Functions
  * @{
  */

/**
  * @brief  Initializes and configures the touch screen functionalities and
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  xSize: Maximum X size of the TS area on LCD
  *         ySize: Maximum Y size of the TS area on LCD
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_Init(uint16_t xSize, uint16_t ySize)
{
  uint8_t ret = TS_ERROR;

  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
  {
    /* Initialize the TS driver structure */
    ts_driver = &stmpe811_ts_drv;

    /* Initialize x and y positions boundaries */
    ts_x_boundary  = xSize;
    ts_y_boundary  = ySize;
//    ts_orientation = TS_SWAP_XY;
    ts_orientation = TS_SWAP_X;
    ret = TS_OK;
  }

  if(ret == TS_OK)
  {
    /* Initialize the LL TS Driver */
    ts_driver->Init(TS_I2C_ADDRESS);
    ts_driver->Start(TS_I2C_ADDRESS);
  }

  return ret;
}

/**
  * @brief  Configures and enables the touch screen interrupts.
  * @param  None
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_ITConfig(void)
{
  /* Call component driver to enable TS ITs */
  ts_driver->EnableIT(TS_I2C_ADDRESS);

  return TS_OK;
}

/**
  * @brief  Gets the touch screen interrupt status.
  * @param  None
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_ITGetStatus(void)
{
  /* Call component driver to enable TS ITs */
  return (ts_driver->GetITStatus(TS_I2C_ADDRESS));
}

/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TS_State: Pointer to touch screen current state structure
  * @retval TS_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
  static uint32_t _x = 0, _y = 0;
  uint16_t xDiff, yDiff , x , y;
  uint16_t swap;

  TS_State->TouchDetected = ts_driver->DetectTouch(TS_I2C_ADDRESS);

  if(TS_State->TouchDetected)
  {
    ts_driver->GetXY(TS_I2C_ADDRESS, &x, &y);

    if(ts_orientation & TS_SWAP_X)
    {
      x = 4096 - x;
    }

    if(ts_orientation & TS_SWAP_Y)
    {
      y = 4096 - y;
    }

    if(ts_orientation & TS_SWAP_XY)
    {
      swap = y;
      y = x;
      x = swap;
    }

    xDiff = x > _x? (x - _x): (_x - x);
    yDiff = y > _y? (y - _y): (_y - y);

    if (xDiff + yDiff > 5)
    {
      _x = x;
      _y = y;
    }

    TS_State->x = (ts_x_boundary * _x) >> 12;
    TS_State->y = (ts_y_boundary * _y) >> 12;
  }

  return TS_OK;
}

/**
  * @brief  Clears all touch screen interrupts.
  * @param  None
  * @retval None
  */
void BSP_TS_ITClear(void)
{
  ts_driver->ClearIT(TS_I2C_ADDRESS);
}


/********************************************************************//**
* @brief Touch Screen Calibration
**********************************************************************/

/**
 * @brief
 * @param matrixPtr
 */
void BSP_TS_Cal_Init(MATRIX_Type *matrixPtr)
{
  uint8_t n;
  MATRIX_Type matrix[4],avgmatrix;
  TS_StateTypeDef RTouch,T_State;
  TS_StateTypeDef screenSample[5];
  TS_StateTypeDef displaySample[5] =  {
      { 0, 20, 20, 0 },
      { 0, 300,  20, 0 },
      { 0, 300, 220, 0 },
      { 0, 20, 220, 0 },
      { 0, 160, 120, 0 },
  };

  TS_StateTypeDef displaytriangle1[3] = {displaySample[0],displaySample[4],displaySample[1]};
  TS_StateTypeDef displaytriangle2[3] = {displaySample[1],displaySample[4],displaySample[2]};
  TS_StateTypeDef displaytriangle3[3] = {displaySample[2],displaySample[4],displaySample[3]};
  TS_StateTypeDef displaytriangle4[3] = {displaySample[3],displaySample[4],displaySample[0]};

  BSP_LCD_Clear(White); //Clear Screen

  // For 5 Ref Points
  for(n=0; n<5; n++)
  {
    switch (n) {
      case 0:
        BSP_LCD_SetTextColor(Red);
        BSP_LCD_DrawCircle(20,20,3);
        break;

      case 1:
        BSP_LCD_SetTextColor(Red);
        BSP_LCD_DrawCircle(300,20,3);
        break;

      case 2:
        BSP_LCD_SetTextColor(Red);
        BSP_LCD_DrawCircle(300,220,3);
        break;

      case 3:
        BSP_LCD_SetTextColor(Red);
        BSP_LCD_DrawCircle(20,220,3);
        break;

      case 4:
        BSP_LCD_SetTextColor(Red);
        BSP_LCD_DrawCircle(160,120,3);
        break;

      default:
        break;
    }

    HAL_Delay(1000);
    T_State.x = 0;
    T_State.y = 0;
    while((T_State.x == 0) && (T_State.y == 0))
    {
      BSP_TS_GetState(&T_State);
      HAL_Delay(100);
    }

    screenSample[n].x = T_State.x;
    screenSample[n].y = T_State.y;
  }

  TS_StateTypeDef screentriangle1[3] = {screenSample[0],screenSample[4],screenSample[1]};
  TS_StateTypeDef screentriangle2[3] = {screenSample[1],screenSample[4],screenSample[2]};
  TS_StateTypeDef screentriangle3[3] = {screenSample[2],screenSample[4],screenSample[3]};
  TS_StateTypeDef screentriangle4[3] = {screenSample[3],screenSample[4],screenSample[0]};


  BSP_TS_SetCalibrationMatrix(displaytriangle1, screentriangle1, &matrix[0]);
  BSP_TS_SetCalibrationMatrix(displaytriangle2, screentriangle2, &matrix[1]);
  BSP_TS_SetCalibrationMatrix(displaytriangle3, screentriangle3, &matrix[2]);
  BSP_TS_SetCalibrationMatrix(displaytriangle4, screentriangle4, &matrix[3]);


  matrixPtr->An = ( matrix[0].An + matrix[1].An + matrix[2].An + matrix[3].An ) / 4;
  matrixPtr->Bn = ( matrix[0].Bn + matrix[1].Bn + matrix[2].Bn + matrix[3].Bn ) / 4;
  matrixPtr->Cn = ( matrix[0].Cn + matrix[1].Cn + matrix[2].Cn + matrix[3].Cn ) / 4;
  matrixPtr->Dn = ( matrix[0].Dn + matrix[1].Dn + matrix[2].Dn + matrix[3].Dn ) / 4;
  matrixPtr->En = ( matrix[0].En + matrix[1].En + matrix[2].En + matrix[3].En ) / 4;
  matrixPtr->Fn = ( matrix[0].Fn + matrix[1].Fn + matrix[2].Fn + matrix[3].Fn ) / 4;
  matrixPtr->Divider = ( matrix[0].Divider + matrix[1].Divider + matrix[2].Divider + matrix[3].Divider ) / 4;

  BSP_LCD_Clear(White);
}


/**
 * @brief
 * @param displayPtr
 * @param screenPtr
 * @param matrixPtr
 * @return
 */
uint8_t BSP_TS_SetCalibrationMatrix(TS_StateTypeDef *displayPtr, TS_StateTypeDef *screenPtr, MATRIX_Type *matrixPtr)
{
  uint8_t retValue = TS_OK;

  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
      ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));

  if(matrixPtr->Divider == 0)
  {
    retValue = TS_ERROR;
  }
  else
  {
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));

    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x));

    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y;

    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y));

    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x));

    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y;
  }

  return(retValue);
} /* end of setCalibrationMatrix() */


/**
 * @brief
 * @param displayPtr
 * @param matrixPtr
 * @return
 */
uint8_t BSP_TS_Cal_Values(TS_StateTypeDef *displayPtr, MATRIX_Type *matrixPtr)
{
  int  retValue = TS_OK;
  TS_StateTypeDef RTouchn;

  BSP_TS_GetState(&RTouchn);
  displayPtr->z = RTouchn.z;

  if(matrixPtr->Divider != 0)
  {
    /* Operation order is important since we are doing integer */
    /*  math. Make sure you add all terms together before      */
    /*  dividing, so that the remainder is not rounded off     */
    /*  prematurely.                                           */

    displayPtr->x = ( (matrixPtr->An * RTouchn.x) +
        (matrixPtr->Bn * RTouchn.y) +
        matrixPtr->Cn
    ) / matrixPtr->Divider;

    displayPtr->y = ( (matrixPtr->Dn * RTouchn.x) +
        (matrixPtr->En * RTouchn.y) +
        matrixPtr->Fn
    ) / matrixPtr->Divider;
  }
  else
  {
    retValue = TS_ERROR;
  }

  return(retValue);
} /* end of getDisplayPoint() */


///**
// * @brief User GPIO Interrupt Callback
// * @param GPIO_Pin  This parameter determines interrupt pin.
// */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//  if (GPIO_Pin == GPIO_PIN_13)
//  {
//    TS_StateTypeDef Touch;
//    uint8_t INT=0;
//
//    INT = BSP_TS_ITGetStatus();
//
//    if((INT & 0x02) == 0x02)
//    {
//      if (CalTouch == 0)
//      {
//        BSP_TS_Cal_Values(&Touch, &cmatrix);
//        if ((Touch.x <= 320) && (Touch.y <= 240))
//        {
//          gTouch.x = Touch.x;
//          gTouch.y = Touch.y;
//        }
//        TReady = TRUE;
//      }
//      else if (CalTouch == 1)
//      {
//        BSP_TS_GetState(&Touch);
//        if ((Touch.x != 0) && (Touch.y != 0))
//        {
//          gTouch.x = Touch.x;
//          gTouch.y = Touch.y;
//        }
//        TReady = TRUE;
//      }
//
//      BSP_TS_ITClear(); // Use only FIFO Threshold Interrupt
//    }
//  }
//}


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

/************END OF FILE*************/
