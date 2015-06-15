/******************************************************************//**
* @file   stm32f4_discovery_rtc.c
* @brief  This file contains implementation of STM32F4 RTC in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_rtc.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_RTC
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_RTC_Public_Types
 * @{
 */

RTC_HandleTypeDef hrtc_bsp;

uint16_t Year[1];

/**
 * @} STM32F4_DISCOVERY_RTC_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_RTC_Private_Functions
 * @{
 */

/**
 * @brief This function enable the RTC by enabling the peripheral clock
 * @param hrtc
 *        @arg hrtc_bsp RTC handle
 */
static void BSP_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

    HAL_NVIC_SetPriority(TAMP_STAMP_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TAMP_STAMP_IRQn);
  }
}
/**
 * @} STM32F4_DISCOVERY_RTC_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_RTC_Public_Functions
 * @{
 */

/**
 * @brief RTC Initialization function
 */
void BSP_RTC_Init(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

    /**Initialize RTC and set the Time and Date
    */
  hrtc_bsp.Instance = RTC;
  hrtc_bsp.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc_bsp.Init.AsynchPrediv = 124;               //LSI clock AsyncPrediv 127 HSE clock AsyncPrediv 124
  hrtc_bsp.Init.SynchPrediv = 7999;               //LSI clock SyncPrediv  255  HSE clock SyncPrediv 7999
  hrtc_bsp.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc_bsp.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc_bsp.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

  BSP_RTC_MspInit(&hrtc_bsp);
  HAL_RTC_Init(&hrtc_bsp);

  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.SubSeconds = 0;
  sTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc_bsp, &sTime, FORMAT_BIN);

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 00;
  HAL_RTC_SetDate(&hrtc_bsp, &sDate, FORMAT_BIN);
}


/**
 * @brief RTC Wakeup Timer configuration function using interrupt
 * @param hrtc
 *        @arg hrtc_bsp
 * @param WakeUptime_ms   provide time in 1 to 10 millisecond
 *                        and 1 second to  60 seconds (in msec unit)
 * NOTE minimum value of WakeUptime_ms is 1 msec.
 */
void BSP_RTC_WakeUpTimer_Init(uint16_t WakeUptime_ms)
{
  /********************************************************************//**
  * @brief RTC_WAKEUPCLOCK_RTCCLK_DIV2 = 500KHZ
  *        1 millisecond count = 500000/1000
  *        RTC_WAKEUPCLOCK_CK_SPRE_16BITS - 1HZ clock selected
  *        RTC_WAKEUPCLOCK_RTCCLK_DIV16 - 1MHZ/16 = 62500
  *        62500 count = 1sec
  **********************************************************************/
  uint8_t WakeUp_clock=0;
  uint16_t WakeUpcount=0;

  if (WakeUptime_ms <= 10 ) //1 msec to 10 msec
  {
    WakeUp_clock = RTC_WAKEUPCLOCK_RTCCLK_DIV2;
    WakeUpcount = (500000 * WakeUptime_ms)/1000;
  }

  else if (1000 <= WakeUptime_ms <= 60000) //1 sec to 60 sec
  {
    WakeUp_clock = RTC_WAKEUPCLOCK_CK_SPRE_16BITS;
    WakeUpcount = (WakeUptime_ms/1000) - 1 ;
  }
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc_bsp, WakeUpcount, WakeUp_clock);
}


/**
 * @brief RTC Alarm configuration function using interrupt
 * @param sel_Alarm   Select the Alarm
 *        @arg AlarmA
 *        @arg AlarmB
 *        @arg BothAlarm  To set the Alarm A and Alarm B
 * @param hour        Set the Hour for Alarm interrupt
 * @param minute      Set the Minute for Alarm interrupt
 * @param second      Set the Second for Alarm interrupt
 * @param dayselect   Select the alarm type
 *        @arg RTC_ALARMDATEWEEKDAYSEL_DATE      Alarm interrupt occur day of every month
 *        @arg RTC_ALARMDATEWEEKDAYSEL_WEEKDAY   Alarm interrupt occur day of every week
 *
 * NOTE If you want to configure both alarm with different time then
 *      type the whole structure differently for both alarm with Set alarm function.
 */
void BSP_RTC_Alarm_Init(Alarm_Typedef sel_Alarm, uint8_t hour, uint8_t minute, uint8_t second, uint8_t dayselect)
{
  RTC_AlarmTypeDef sAlarm;

  sAlarm.AlarmTime.Hours = hour;
  sAlarm.AlarmTime.Minutes = minute;
  sAlarm.AlarmTime.Seconds = second;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;

  if (dayselect == RTC_ALARMDATEWEEKDAYSEL_WEEKDAY)
  {
    sAlarm.AlarmDateWeekDaySel = dayselect;
    sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  }
  else if (dayselect == RTC_ALARMDATEWEEKDAYSEL_DATE)
  {
    sAlarm.AlarmDateWeekDaySel = dayselect;
    sAlarm.AlarmDateWeekDay = 1;
  }

  if(sel_Alarm == AlarmA)
  {
    sAlarm.Alarm = RTC_ALARM_A;
    HAL_RTC_SetAlarm_IT(&hrtc_bsp, &sAlarm, FORMAT_BIN);
  }
  else if (sel_Alarm == AlarmB)
  {
    sAlarm.Alarm = RTC_ALARM_B;
    HAL_RTC_SetAlarm_IT(&hrtc_bsp, &sAlarm, FORMAT_BIN);
  }
  else if (sel_Alarm == BothAlarm)
  {
    sAlarm.Alarm = RTC_ALARM_A;
    HAL_RTC_SetAlarm_IT(&hrtc_bsp, &sAlarm, FORMAT_BIN);

    sAlarm.Alarm = RTC_ALARM_B;
    HAL_RTC_SetAlarm_IT(&hrtc_bsp, &sAlarm, FORMAT_BIN);
  }
}


/**
 * @brief RTC Time stamp configuration function
 * @param TSpinEdgeSel  Select the rising edge or falling edge
 *        @arg RTC_TIMESTAMPEDGE_RISING
 *        @arg RTC_TIMESTAMPEDGE_FALLING
 *
 * NOTE Time stamp event generated on the PC13 GPIO pin by apply
 */
void BSP_RTC_TimeStamp_Init(uint8_t TsPinEdgeSel)
{
  HAL_RTCEx_SetTimeStamp_IT(&hrtc_bsp, TsPinEdgeSel, RTC_TIMESTAMPPIN_PC13);
}


/**
 * @brief RTC Tamper configuration function
 * @param TampTrigEdge  Select the rising edge or falling edge
 *       @arg RTC_TAMPERTRIGGER_RISINGEDGE
 *       @arg RTC_TAMPERTRIGGER_FALLINGEDGE
 */
void BSP_RTC_Tamper_Init(uint8_t TampTrigEdge)
{
  RTC_TamperTypeDef sTamper;
  sTamper.Tamper = RTC_TAMPER_1;
  sTamper.PinSelection = RTC_TAMPERPIN_PC13;
  sTamper.Trigger = RTC_TAMPERTRIGGER_RISINGEDGE;
  sTamper.Filter = RTC_TAMPERFILTER_DISABLE;
  sTamper.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
  sTamper.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  sTamper.TamperPullUp = RTC_TAMPER_PULLUP_ENABLE;
  sTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;

  HAL_RTCEx_SetTamper_IT(&hrtc_bsp, &sTamper);
}


/**
 * @brief It asks user to set new Time
 * @return status
 */
uint8_t BSP_RTC_Change_Time (void)
{
  RTC_TimeTypeDef T;

  char cp;                          /* input from keyboard */
  char RTCdata;                     /* buffer */
  uint8_t TimeFlag=0;               /* flag for time data is available */
  uint8_t count = 0;                /* input character counter */
  uint8_t State = 0;                /* state for hh=1, mm=2, ss=3 */

  uint8_t digit = 0;                /* indication for 1st and 2nd digit of hh, mm, ss */
  uint8_t MaxCount = 6;             /* maximum no. of digits */
  char AsciiLSB;                    /* temporary storage for lsb ascii input */
  char AsciiMSB;                    /* temporary storage for msb ascii input */

  T.Hours = 0;
  T.Minutes = 0;
  T.Seconds = 0;
  T.SubSeconds = 0;
  T.TimeFormat = RTC_HOURFORMAT12_AM;
  T.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  T.StoreOperation = RTC_STOREOPERATION_RESET;

  while(1)
  {
    cp = ugetche(BLOCKING);         /* get input character */

    if (cp == In_ESC)               /* if ESCAPE pressed then exit */
    {
      return (cp);
    }
    else if (cp == In_CR)           /* CARRIAGE RETURN ? */
    {
      if(count==0)                  /* any characters at all ? */
      {
        continue;                   /* no, so get another character */
      }

      if(TimeFlag==1)
      {                             /* update real time clock  */
          /* Write changes to RTC */
        HAL_RTC_SetTime(&hrtc_bsp, &T, FORMAT_BIN);
        return(0);
      }
      else
        break;                      /* check for any character */
    }
    else if (cp == In_DELETE || cp == In_BACKSPACE) /* delete or back space */
    {
      if(count == 0)                /* any characters entered */
      {
        continue;                   /* no, so get another character */
      }

      if(digit == 0)
      {
        uErase_SemiColon();
        digit = 1;                /* now its point to 2nd digit */
        State--;                  /* point to one up state */
        count--;                  /* decrement character count */
        continue;
      }
      else
      {
        if(TimeFlag == 1)
        {
          uErase_Char_With_UnderScore();
          TimeFlag = 0;          /* changed mind so not ready */
          digit = 1;             /* point to 2nd digit */
          count--;               /* decrement count */
          continue;              /* goback and get another input */
        }

        uErase_Char_With_UnderScore();

        digit = 0;               /* now its point to 1st digit */
        count--;                 /* decrement character count */
        continue;
      }
    }
    else if (cp>= '0' && cp <='9')/* is character between 0 to 9? */
    {
      RTCdata = cp;
      /*********** check hours **********/
      if(count < MaxCount)
      {
        if(State == 0)
        {
          if(digit == 0)
          {
            uprintf("%c",RTCdata);     /* echo 1st character */
            AsciiMSB = RTCdata;        /* store 1st byte MSB  */
            count++;                   /* increment character count,will be 1 */
            digit++;                   /* increment digit, will be 1 */
            continue;
          }
          else
          {
            uprintf("%c", RTCdata);               /* echo 2nd character */
            AsciiLSB = RTCdata;                   /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10);   /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);         /* add to nibble, integer */

            if(RTCdata <=23)                      /* check hour range ( 0-23) */
            {
              T.Hours = RTCdata;                  /* store hh in time buffer */

              State++;                            /* increment to mm state */
              count++;                            /* increment character count,will be 2 */
              digit = 0;                          /* set digit to zero  */

              uprintf(":");                       /* and write ':' on the screen */
              continue;                           /* continue for minutes */
            }
            else
            {
              uErase_And_RingTheBell();
              continue;
            }
          }
        }

        /********** check minutes ***********/
        if(State == 1)
        {
          if(digit==0)
          {
            uprintf("%c", RTCdata);         /* echo 1st character */
            AsciiMSB = RTCdata;             /* store 1st byte MSB  */
            count++;                        /* increment character count,will be 3 */
            digit++;                        /* increment digit, will be 1  */
            continue;
          }
          else
          {
            uprintf("%c", RTCdata);             /* echo 2nd character */
            AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

            if(RTCdata <=59)                    /* check minutes range ( 0-59) */
            {
              T.Minutes = RTCdata;              /* store mm in time buffer */
              State++;                          /* increment to ss state */
              count++;                          /* increment character count,will be 4 */
              digit = 0;                        /* set digit to zero  */
              uprintf(":");                     /* and write ':' on the screen */

              continue;                         /* continue for seconds */
            }
            else
            {
              uErase_And_RingTheBell();
              continue;
            }
          }
        }

        /*********** check seconds ***************/
        if(State == 2)
        {
          if(digit==0)
          {
            uprintf("%c", RTCdata);         /* echo 1st character */
            AsciiMSB = RTCdata;             /* store 1st byte MSB  */
            count++;                        /* increment character count,will be 5 */
            digit++;                        /* increment digit no.  */

            continue;
          }
          else
          {
            uprintf("%c", RTCdata);              /* echo 2nd character */
            AsciiLSB = RTCdata;                  /* store 2nd byte LSB  */
            RTCdata = ((AsciiMSB & 0x0F) * 10);  /* store nibble */
            RTCdata += (AsciiLSB & 0x0F);        /* add to nibble, integer */

            if(RTCdata <=59)                     /* check seconds range ( 0-59) */
            {
              T.Seconds = RTCdata;               /* store ss in time buffer */

              TimeFlag = 1;                      /* Time data is ready for RTC */
              count++;                           /* increment character count,will be 6 */

              continue;                          /* go back and find the decision  */
            }
            else
            {
              uErase_And_RingTheBell();
              continue;
            }
          }
        }

        continue;                                /* number of char is more than read */
        /* so go back to find the decision */
      }
      /*************************************************/
      else
      {
        uprintf("%c", cp);                       /* echo character */
        uErase_Char_With_UnderScore();
        uprintf("\7");                           /* ring the bell */

        continue;
      }
    }
  }
  return(0);
}


/**
 * @brief Check if the entered year is leap year or not
 * @param Year      Only provide last two digits of Year (2015 = 15)
 * @param Century   Provide the Century-1 (2015 = 21-1 = 20)
 * @return status
 */
uint8_t Is_Leap_Year(uint16_t Year, uint16_t Century)
{
  /* If year not a multiple of 4 then not a leap year */
  if( (Year % 4) != 0) return NO;

  /* If year a multiple of 4 and non zero then is a leap year */
  if(Year != 0) return YES;

  /* Must check the century is a multiple of 4 also */
  return ((Century % 4) == 0);
}


/**
 * @brief It asks user to change Date
 * @return status
 */
uint8_t BSP_RTC_Change_Date (void)
{
  uint8_t DaysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  RTC_DateTypeDef D;
  D.WeekDay = RTC_WEEKDAY_MONDAY;
  D.Month = RTC_MONTH_MAY;
  D.Date = 1;
  D.Year = 00;
  uint8_t Daysthismonth;
  uint8_t DateBuffer[1];
  char cp;                               /* input from keyboard */
  char RTCdata;                          /* buffer */
  uint8_t DateFlag=0;                    /* flag for date data is available */
  uint8_t count = 0;                     /* input character counter */
  uint8_t State = 0;                     /* state for dd=1, mm=2, ccyy=3 */

  uint8_t digit = 0;                     /* indication for 1st, 2nd, 3rd and 4th digit */
                                         /* for dd, mm, yycc */
  uint8_t MaxCount = 8;                  /* maximum no. of digits */
  char AsciiLSB;                         /* temp storage for LSB ascii input */
  char AsciiMSB;                         /* temp storage for MSB ascii input */
  uint8_t DaysThisMonth;                 /* number of days for the given month */

    while(1)
    {
      cp = ugetche(BLOCKING);                    /* get input character */

      if(cp == In_ESC)                   /* if ESCAPE pressed then exit */
      {
        return (cp);
      }
      else if (cp == In_CR)              /* CARRIAGE RETURN ? */
      {
        if(count==0)                     /* any characters at all ? */
        {
          continue;                      /* no, so get another character */
        }

        if(DateFlag==1)
        {   /*  update real time clock  */
          /* Read RTC */
          Year[0] = ((DateBuffer[0] & 0xFF) * 100);
          Year[0] += (DateBuffer[1] & 0xFF);
          D.Year = DateBuffer[1];
          HAL_RTC_SetDate(&hrtc_bsp, &D, FORMAT_BIN);
          return(0);
        }
        else
        {
          break;                       /* check for any character */
        }
      }
      else if (cp == In_DELETE || cp == In_BACKSPACE) /* delete or back space */
      {
        if(count==0)                   /* any characters entered */
        {
          continue;                    /* no, so get another character */
        }

        if(State == 2)                 /* State 2 contains 4 digits */
        {
          if(digit == 0)
          {
            uErase_Backslash();
            digit = 1;                 /* now its point to 2nd digit of month */
            State--;                   /* point to month's state */
            count--;                   /* decrement character count */

            continue;
          }
          else
          {
            uErase_Char_With_UnderScore();
            digit--;                     /* point to 2nd digit */
            count--;                     /* decrement count */

            continue;                    /* go back and get another input */
          }
        }
        else
        {
          if(digit ==0)
          {
            uErase_Backslash();
            State--;                     /* point to month's state */
            digit = 1;                   /* now its point to 2nd digit of month */
            count--;                     /* decrement character count */

            continue;
          }
          else
          {
            uErase_Char_With_UnderScore();
            digit = 0;                   /* point to 1st digit */
            count--;                     /* decrement count */

            continue;                    /* goback and get another input */
          }
        }
      }
      else if (cp>= '0' && cp <='9')     /* is character between 0 to 9? */
      {
        RTCdata = cp;

        /****** check date ****/
        if(count < MaxCount)
        {
          if(State == 0)
          {
            if(digit==0)
            {
              uprintf("%c", RTCdata);    /* echo 1st character */
              AsciiMSB = RTCdata;        /* store 1st byte MSB  */
              count++;                   /* increment character count,will be 1 */
              digit++;                   /* increment digit, will be 1 */

              continue;
            }
            else
            {
              uprintf("%c", RTCdata);             /* echo 2nd character */
              AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
              RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
              RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

              if(RTCdata >= 1 && RTCdata <=31)    /* check date range ( 1-31) */
              {
                D.Date = RTCdata;                 /* store DOM in DateBuffer[0] */

                State++;                 /* increment to mm state */
                count++;                 /* increment character count,will be 2 */
                digit = 0;               /* set digit to zero  */
                uprintf("/");            /* and write '/' on the screen */

                continue;                /* continue for minutes */
              }
              else
              {
                uErase_And_RingTheBell();

                continue;
              }
            }
          }
          /************************* check month *************************/
          if(State == 1)
          {
            if(digit==0)
            {
              uprintf("%c", RTCdata);    /* echo 1st character */
              AsciiMSB = RTCdata;        /* store 1st byte MSB  */
              count++;                   /* increment character count,will be 3 */
              digit++;                   /* increment digit, will be 1  */

              continue;
            }
            else
            {
              uprintf("%c", RTCdata);   /* echo 2nd character */
              AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
              RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
              RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

              if(RTCdata >= 1 && RTCdata <=12)    /* check month range ( 1-12) */
              {
                D.Month = RTCdata;       /* store mm in time buffer */

                State++;                 /* increment to ss state */
                count++;                 /* increment character count,will be 4 */
                digit = 0;               /* set digit to zero  */
                uprintf("/");            /* and write '/' on the screen */

                continue;                /* continue for seconds */
              }
              else
              {
                uErase_And_RingTheBell();

                continue;
              }
            }
          }
          /********** check year and century *********/
          if(State == 2)
          {
            if(digit == 0)
            {
              uprintf("%c", RTCdata);    /* echo 1st character */
              AsciiMSB = RTCdata;        /* store 1st byte MSB  */

              count++;                   /* increment character count,will be 5 */
              digit++;                   /* increment digit no.  */

              continue;
            }
            if(digit == 1)
            {
              uprintf("%c", RTCdata);   /* echo 2nd character */
              AsciiLSB = RTCdata;                 /* store 2nd byte LSB  */
              RTCdata = ((AsciiMSB & 0x0F) * 10); /* store nibble */
              RTCdata += (AsciiLSB & 0x0F);       /* add to nibble, integer */

              if(RTCdata >= 0 && RTCdata <=40)   /* check century range (0-39) */
              {
                DateBuffer[0] = RTCdata;          /* store in century buffer */

                count++;                 /* increment character count,will be 6 */
                digit++;                 /* increment digit number, will be 3  */

                continue;
              }
              else
              {
                uErase_Char_With_UnderScore();
                uprintf("\7");          /* ring the bell */

                continue;
              }
            }
            /**********  check year  ************/
            if(digit == 2)
            {
              uprintf("%c", RTCdata);   /* echo 1st character */
              AsciiMSB = RTCdata;       /* store 1st byte MSB  */

              count++;                  /* increment character count,will be 7 */
              digit++;                  /* increment digit no.  */

              continue;
            }

            if(digit == 3)
            {
              uprintf("%c", RTCdata);              /* echo 2nd character */
              AsciiLSB = RTCdata;                  /* store 2nd byte LSB  */
              RTCdata = ((AsciiMSB & 0x0F) * 10);  /* store nibble */
              RTCdata += (AsciiLSB & 0x0F);        /* add to nibble, integer */

              if(DateBuffer[0] == 40)
              {
                if(RTCdata <=95)              /* check year range (00-95) */
                {
                  DateBuffer[1] = RTCdata;    /* store in year buffer */
                  DateFlag = 1;               /* Time data is ready for RTC */
                    count++;                  /* increment character count,will be 8 */
                    digit++;                  /* increment digit number, will be 4  */
                    continue;
                }
                else
                {
                  uErase_Char_With_UnderScore();
                  uprintf("\7");              /* ring the bell */

                  continue;
                }
              }

              if(RTCdata <=99)                     /* check year range (00-99) */
              {
                DateBuffer[1] = RTCdata;           /* store in year buffer */
                DaysThisMonth = DaysPerMonth[D.Month- 1];
                if(Is_Leap_Year(DateBuffer[1], DateBuffer[0]) && (D.Month == 2))
                {
                  ++DaysThisMonth;
                }
                if(D.Date > DaysThisMonth)
                {
                  D.Date = DaysThisMonth;
                }
                DateFlag = 1;                      /* Time data is ready for RTC */
                count++;                           /* increment character count,will be 8 */
                digit++;                           /* increment digit number, will be 4  */

                continue;
              }
              else
              {
                uErase_Char_With_UnderScore();
                uprintf("\7");           /* ring the bell */

                continue;
              }
            }
          }
          continue;                      /* number of char is more than reqd */
          /* so go back to find the decision */
        }
        else
        {
          uprintf("%c", cp);                   /* echo character */
          uErase_Char();
          uprintf("\7");                       /* ring the bell */

          continue;
        }
      }
    }
    return(0);
}


/**
 * @brief This function disable the RTC by disabling the peripheral clock
 * @param hrtc
 *        @arg hrtc_bsp  RTC handle
 */
void BSP_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
    HAL_NVIC_DisableIRQ(TAMP_STAMP_IRQn);
  }
}

/**
 * @} STM32F4_DISCOVERY_TIMER_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_RTC End
 */

 /* --------------------------------- End Of File ------------------------------ */
