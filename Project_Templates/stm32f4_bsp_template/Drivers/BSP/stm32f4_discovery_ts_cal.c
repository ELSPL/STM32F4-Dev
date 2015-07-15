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
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TS_CAL_Private_Function_Prototypes
  * @{
  */
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
