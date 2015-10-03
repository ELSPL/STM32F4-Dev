/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the WIFI functions \n
*         -   Study the programs related to the WIFI interfacing
*
* @section section2 Example1 :
*    Objective: Write a program to interface Discover-WIFI.
*
* @section section3 Program Description:
*    This program demonstrates
*      Discover-WIFI interface example in which
*
*         0: Get WiFi status  - It will give current WIFI status like WIFI On or
*                         Off, join to any network
*         1: Wifi Scan - It will show available wifi network in its range
*         2: Wifi Join - To connect the available WIFI network
*         3: Get IP - It will give the current IP address of the Discover-WIFI
*               0 - in STA mode(when Discover-WIFI is connected to any network )
*               1 - in AP mode(Discover-WIFI works as Access Point)
*         4: TCP client - Discover-WIFI works as TCP client
*         5: TCP sever - Discover-WIFI works as TCP server
*         6: Send from sock - To send data
*         7: WiFi Leave - Disconnect from the connected network
*         8: AP On/Off - Access Point mode On or Off
*         9: UDP client - Discover-WIFI works as UDP client
*         a: UDP server - Discover-WIFI works as UDP Server
*         b: Wifi Off - To turn on the Discover-WIFI
*         c: Wifi On - To turn off the Discover-WIFI
*         m: Show Menu - It will show the menu
*         q: Quit - To Quit from the Discover-WIFI menu
*
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files     	           |
*   | :----------------------------:| :---------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	       |
*   | @ref stm32f4_discovery_uart.h | @ref stm32f4_discovery_uart.c|
*   | @ref stm32f4_discovery_wifi.h | @ref stm32f4_discovery_wifi.c|
*   |                               | @ref main.c                  |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(Discover-WIFI) |
*   | :------------------:| :--------------------:|
*   | P2.23(PB6)          | Pin 3(J6)             |
*   | P2.24(PB7)          | Pin 5(J6)             |
*
* @section section6 Connection
*   | STM32F407 Reference |
*   | :------------------:|
*   | COM1                |
*
* @section section7 Program Folder Location
*       <Eg1>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - Serial Cable \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Discover-WIFI Board \n

*
* @section section9 Hardware Configuration
*   - Connect Discover-WIFI Board to Target board using DuPont cable. PB6-->Pin3(J6) and PB7-->Pin5(J6).
*   - Connect the Target board using USB port of PC using USB cable.
*   - Connect the Discover-WIFI board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    Discover-WIFI works properly with Target board.
*\n
*\n
*******************************************************************************/
