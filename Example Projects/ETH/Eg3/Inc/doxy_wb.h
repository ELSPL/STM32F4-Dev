/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Ethernet functions \n
*         -   Study the programs related to the Ethernet and UDP protocol
*
* @section section2 Example1 :
*    Objective: Write a program to test Ethernet server(UDP protocol).
*
* @section section3 Program Description:
*    This program demonstrates the testing of Ethernet in server mode.
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref lwip/opt.h        	  | @ref lwip/opt.c        	  |
*   | @ref lwip/init.h          | @ref lwip/init.c          |
*   | @ref lwip/netif.h         | @ref lwip/netif.c         |
*   | @ref lwip/lwip_timers.h   | @ref lwip/lwip_timers.c   |
*   | @ref netif/etharp.h       | @ref netif/etharp.c       |
*   | @ref ethernetif.h         | @ref ethernetif.c         |
*   | @ref udp_echoserver.h     | @ ref udp_echoserver.c    |
*   |                           | @ref main.c               |
*
* \n
*
* @section section5 Connection
*   | STM32F407 Reference |
*   | :------------------:|
*   | J1                  |
*
* @section section6 Program Folder Location
*       <Eg1>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Ethernet cable \n
*
*
* @section section9 Hardware Configuration
*   - Connect Ethernet cable between target board and PC.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    Ethernet is properly working in Ethernet server mode(UDP protocol)
*\n
*\n
*******************************************************************************/
