/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GPIO functions \n
*         -   Study the programs related to EEPROM interface using I2C.
*
* @section section2 Example1 :
*    Objective:  Write a program to Write and Read Byte of I2C based EEPROM (ASK-25A).
*
* @section section3 Program Description:
*    This program demonstrates interfacing of EEPROM using I2C protocol and write and read byte on EEPROM.
*
* @section section4 Included Files:
*
*   | Header Files           	    | Source Files           	     |
*   | :--------------------------:| :------------------------:   |
*   | @ref stm32f4xx_hal_conf.h   | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		    | @ref stm32f4xx_it.c	 	       |
*   | @ref stm32f4_ask25.h        | @ref stm32f4_ask25.c         |
*   | @ref stm32f4_discovery_vcp.h| @ref stm32f4_discovery_vcp.c |
*   |                             | @ref main.c                  |
* \n
*
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(ASK-25)     |
*   | :------------------:| :----------------: |
*   | P2.20 GPIOB.9       | I2C SDA		         |
*   | P2.23 GPIOB.6       | I2C SCL		         |
*
*
*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J3                  | ASK-25 (PL2)    |
*
* @section section7 Program Folder Location
*       <Eg1>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - ASK-25 Rev2.0 \n
*   - 4pin relimate cable
*
* @section section9 Hardware Configuration
*   - Connect ASK 25 to educational practice board using flat cable.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    The data you have entered in EEPROM will be shown on Com-port
*\n
*\n
*******************************************************************************/
