/**
  ******************************************************************************
  * @file    stm32f4_gsm.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   Contains all functions support for GSM library on STM32F4
  ******************************************************************************
  *
/* Includes ------------------------------------------------------------------*/

#include "stm32f4_gsm.h"
#include "stm32f4_discovery_uart.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_GSM
  * @{
  */

/** @defgroup STM32F4_GSM
  * @brief This file provides set of firmware functions to manage GSM kit.
  * @{
  */

/** @defgroup STM32F4_GSM_Functions
  * @{
  */

/**
 * @brief This function transmits command for Thermal Printer to change font type or graphic type
 * @param Cmd buffer name consist command for GSM
 */
void BSP_GSM_Cmd(uint8_t *Cmd)
{
  while(*Cmd!='\0')
  {
    uprintf("%c",*Cmd );
    Cmd++;
  }
}


/**
 * @brief This function receives data from GSM and polling method is used
 * @param dataBuf  buffer consist data from GSM
 * @param length   total number of elements data to be read from GSM
 */
void BSP_GSM_Read(uint8_t *dataBuf ,uint8_t length)
{
  uint8_t i=0 ;
 	while (length--)
       {
		dataBuf[i++]= ugetche(TIME_BLOCKING);
       }
      dataBuf[i]='\0';
}


/**
 * @brief This function receives data from GSM
 * @return value
 */
char BSP_GSM_ReadADV(void)

{
  int8_t Data,sbuf[2];
	uint8_t i=0,k=0;
	while(i<160)
	{
		i++;
		Data = ugetche(TIME_BLOCKING);
		sbuf[0]=sbuf[1];
		sbuf[1]=Data;
		if((sbuf[0]=='O')&&(sbuf[1]=='K'))
		{
			k=1;
			break;
		}
	}
	return(k);
}


/**
 * @brief This function receives data from GSM
 * @param dataBuf buffer consist data from GSM
 * @param length total number of elements data to be read from GSM
 */
void BSP_GSM_ReadADV2(uint8_t *dataBuf ,uint16_t length)
{
  uint16_t i=0;
 	while (length--)
  {
		dataBuf[i++] = ugetche(TIME_BLOCKING);
  }
  dataBuf[i]='\0';
}


/**
 * @brief This function checks the connection of microcontroller and GSM modem
 * @return value
 */
char BSP_GSM_ConnectTest(void)
{
	uint8_t cmd[3] = "AT\r";
	BSP_GSM_Cmd(cmd);
	if(BSP_GSM_ReadADV()==1)
	{
	  return(1);
	}
	else
	{
	  return(0);
	}
}


/**
 * @brief This function scroll the phone book in the sim memory.
 * @param num it is number to be called.
 * @return value
 */
char BSP_GSM_Call(uint8_t *num)
{
  uint8_t *cmd="ATD";
  uint8_t *cmd2=";\r";
 	BSP_GSM_Cmd(cmd);
 	BSP_GSM_Cmd(num);
 	BSP_GSM_Cmd(cmd2);
	if(BSP_GSM_ReadADV()==1)
	{
	  return(1);
	}
	else
	{
	  return(0);
	}
}


/**
 * @brief This function used to give a missed call to other phone.
 * @return value
 */
char BSP_GSM_MissedCall()
{
  uint8_t *msdcall="ATH\r";
  BSP_GSM_Cmd(msdcall);
  if(BSP_GSM_ReadADV()==1)
  {
    return(1);
  }
	else
	{
	  return(0);
	}
}


/**
 * @brief This function scroll the phone book in the sim memory
 * @param dataBuf it is received data from Gsm through Uart
 */
void BSP_GSM_ScrollPB(uint8_t *dataBuf)
{
  uint8_t *cmd="AT+CPBR=1,50\r";
	BSP_GSM_Cmd(cmd);
 	BSP_GSM_ReadADV2(dataBuf,400);
}


/**
 * @brief This function write the contact to the phone book in the sim memory
 * @param location it enters the location where the phonebook entry is done
 * @param number it enters the phone number which is to be stored
 * @param name it enters the contact name
 */
void BSP_GSM_WritePB(uint8_t *location,uint8_t *number,uint8_t *name)

{
  uint8_t *cmd1 = "AT+CPBW=";
  uint8_t *cmd2=",\"";
  uint8_t *national_no="\",129,\"";
  uint8_t *cmd3="\"\r";

  BSP_GSM_Cmd(cmd1);
 	BSP_GSM_Cmd(location);
 	BSP_GSM_Cmd(cmd2);
 	BSP_GSM_Cmd(number);
 	BSP_GSM_Cmd(national_no);
 	BSP_GSM_Cmd(name);
 	BSP_GSM_Cmd(cmd3);
}


/**
 * @brief This function find the contact of the phone book from the sim memory
 * @param name it enters the contact name
 * @param nameBuf it is received data from Gsm through Uart
 */
void BSP_GSM_FindPB(uint8_t *name,uint8_t *nameBuf)
{

  uint8_t *cmd11="AT+CPBF=\"";
  uint8_t *cmd2="\"\r";
  uint8_t i=0,j=0;

	BSP_GSM_Cmd(cmd11);
 	BSP_GSM_Cmd(name);
	BSP_GSM_Cmd(cmd2);
 	BSP_GSM_ReadADV2(nameBuf,250);
}


/**
 * @brief This function selects the text mode of GSM modem
 * @return value
 */
char BSP_GSM_TMODE(void)
{
  uint8_t *mode="AT+CMGF=1\r";   //select textmode
	BSP_GSM_Cmd(mode);
 	if(BSP_GSM_ReadADV()==1)
 	{
 	  return(1);
 	}
	else
	{
	  return(0);
	}
}


/**
 * @brief This function used to send message to a particular number
 * @param number it enters the phone number
 * @param message it enters message to be send
 * @return value
 */
char BSP_GSM_SendSMS(uint8_t *number,uint8_t *message)
{
  uint8_t *cmd1 ="AT+CMGS=\"";
  uint8_t *cmd2="\"\r";
  uint8_t cmd3[]={26,'\0'};
	BSP_GSM_Cmd(cmd1);
	BSP_GSM_Cmd(number);
	BSP_GSM_Cmd(cmd2);
	HAL_Delay(500);
	BSP_GSM_Cmd(message);
	BSP_GSM_Cmd(cmd3);
	if(BSP_GSM_ReadADV()==1)
	{
	  return(1);
	}
	else
	{
	  return(0);
	}
}


/**
 * @brief This function reads the message from a specific location stored in the sim memory
 * @param location it enters the location where the message is stored
 * @param message it reads message
 */
void BSP_GSM_ReadSMS(uint8_t *location, uint8_t *message)
{
  uint8_t *cmd1="AT+CMGR=";
  uint8_t *cmd2="\r";
  BSP_GSM_Cmd(cmd1);
  BSP_GSM_Cmd(location);
  BSP_GSM_Cmd(cmd2);
  BSP_GSM_ReadADV2(message,400);
}

/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
