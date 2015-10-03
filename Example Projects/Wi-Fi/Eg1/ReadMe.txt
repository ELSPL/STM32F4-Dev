/*****************************************************************************
 * This is Discover-WIFI interface example in which
 *
 *   0: Get WiFi status  - It will give current WIFI status like WIFI On or
 *                         Off, join to any network
 *   1: Wifi Scan - It will show available wifi network in its range
 *   2: Wifi Join - To connect the available WIFI network
 *   3: Get IP - It will give the current IP address of the Discover-WIFI
 *               0 - in STA mode(when Discover-WIFI is connected to any network )
 *               1 - in AP mode(Discover-WIFI works as Access Point)
 *   4: TCP client - Discover-WIFI works as TCP client
 *   5: TCP sever - Discover-WIFI works as TCP server
 *   6: Send from sock - To send data
 *   7: WiFi Leave - Disconnect from the connected network
 *   8: AP On/Off - Access Point mode On or Off
 *   9: UDP client - Discover-WIFI works as UDP client
 *   a: UDP server - Discover-WIFI works as UDP Server
 *   b: Wifi Off - To turn on the Discover-WIFI
 *   c: Wifi On - To turn off the Discover-WIFI
 *   m: Show Menu - It will show the menu
 *   q: Quit - To Quit from the Discover-WIFI menu
 *
 ******************************************************************************/

################################################################################
	 		    Configure UART for WIFI
################################################################################	

--> UART 1 is used for Discover-WIFI communication with Baud Rate: 921600

On STM32F4 UART 1 Conncction
GPIO pin PB6(P2-23) - TX, pin PB7(P2-24) - RX

On Discover-WIFI UART Connection
connector J6 pin 3 - RX, pin 5 - TX

Connect PB6 to Pin 3 on Discover-WIF and PB7 to pin 5 on Discover-WIFI 

-->UART 6 is used for User communication with Baud Rate: 115200

################################################################################
       			      Example Explanation
################################################################################

--> 0: Get WiFi status  - It will give current WIFI status like WIFI On or
                      Off, join to any network
---------------------------------------------------------------------------------
The output will be : 

0
-GetStatus
WiFi On.  Mac: 00:0B:6C:41:7F:CB.  Not joined any network.
.
if it is connected to any network then the output will be:

0
-GetStatus
WiFi On.  Mac: 00:0B:6C:41:7F:CB.  Joined SSID: db
. 
----------------------------------------------------------------------------------

--> 1: Wifi Scan - It will show available wifi network in its range

The output will be :

1
-WifiScan
.
.
SSID: db CH: 06 RSSI: 230 Sec: 00 
SSID: BSNL_AP CH: 01 RSSI: 170 Sec: 01 
.

@NOTE WIFI scan will give you available wifi network SSID with Received signal
	  strength indication(RSSI), Channel and Security algorithm 
-----------------------------------------------------------------------------------

--> 2: Wifi Join - To connect the available WIFI network
The output will be :

2
-WifiDisconn
Enter SSID: 
db
Enter Security Mode (e.g., 0 for open, 2 for WPA TKIP, 4 for WPA2 AES, 6 for WPA2 MIXED):
0
-WifiJoin
.
Network UP
.
Join success
.
-SnicInit
-SnicIPConfig
.
IPConfig OK
.
-------------------------------------------------------------------------------------

--> 3: Get IP - It will give the current IP address of the Discover-WIFI
                0 - in STA mode(when Discover-WIFI is connected to any network )
                1 - in AP mode(Discover-WIFI works as Access Point)
 
The output will be :

If Discover-WIFI is connccted to any other wifi network then station(STA) mode will
show the ip address given by the network
3
-SnicInit

Interface Type? (0: STA  1: AP) 
0
-SnicGetDhcp
.
IP assigned as 192.168.043.005 
.

If Discover-WIFI is working as AP(Access Point) then it will give the gateway address
3
-SnicInit

Interface Type? (0: STA  1: AP) 
1
 -SnicGetDhcp
.
IP assigned as 172.031.000.001 
.
---------------------------------------------------------------------------------------
--> 4: TCP client - Discover-WIFI works as TCP client
	
To work as TCP client or server another wifi device required
STM32F4DISCOVERY Kit>>Discover Wi-Fi ---------- Works as a CLIENT
Computer -------------------------------------- Works as a SERVER 

Connect your computer/PC to the same wireless network (Embest in this
case), and get your IP address: 192.168.43.8

Now run the testserver.exe utility in command prompt by pressing shift key+ right click
ex. C: testserver.exe 192.168.43.8 1234
	   testserver.exe <ip> <port num>

It will show options 
	0 TCP client
	1 TCP server
	2 Send from sock
	3 UDP server
	4 UDP send to soft AP
	q Quit

Press '1' to set computer as TCP server. It will create Socket.

Trying to create socket, and accept incoming connections.
Socket 0 listening on port 0x4d2

Set Discover-WIFI as Client by pressing '4'

The Output will be :

4
-tcpCreateSocket
Socket:06 opened
.
Enter server IP to connect: 
192.168.43.8
Enter server port number: 
1234
-tcpConnectToServer
.
Socket connection UP
.

On computer the command prompt window will show

Connection accepted
Socket 1 will be used to send data.

---------------------------------------------------------------------------------------
--> 6: Send from sock - To send data
 
 After above connction press '6' in Discover-WIFI terminal connection to send data

 The output will be :

6
Enter socket number to send from: 
6
Content Option? (0: Default 128bytes will be sent) 
0
-sendFromSock
pkt 024, 128 bytes sent 

check the opened socket in the connection. here is socket no. 6

On computer side the command prompt window will show

Recv:128

Now press '2' in Computer command prompt window
it will send 200bytes to connected device

Trying to send 200 bytes from socket 1.
Bytes sent: 200

On Discover-WIFI size 200bytes received 

200 bytes received from socket 06 
--------------------------------------------------------------------------------------
--> 5: TCP sever - Discover-WIFI works as TCP server

The above steps can be done in the opposite direction.
--------------------------------------------------------------------------------------
-->  7: WiFi Leave - Disconnect from the connected network

The output will be :

7
-SnicCleanup
-WifiDisconn
.
Network Down
.

It can be checked by pressing '0', it will give wifi status 
--------------------------------------------------------------------------------------
--> 8: AP On/Off - Access Point mode On or Off

The output will be :

8
 -AP status
AP is OFF
.

8
 -AP status
AP is ON
.
--------------------------------------------------------------------------------------
--> 9: UDP client - Discover-WIFI works as UDP client

STM32F4DISCOVERY Kit>>Discover Wi-Fi ---------- Work as CLIENT
Computer/PC ------------------------------------Work as SERVER

Make sure the Discover Wi-Fi module is in AP mode then connect computer to  Discover-WIFI
network and get ip address 172.31.0.3

Now run the testserver.exe utility in command prompt by pressing shift key+ right click
ex. C: testserver.exe 172.31.0.3 2222
	   testserver.exe <ip> <port num>

It will show options 
	0 TCP client
	1 TCP server
	2 Send from sock
	3 UDP server
	4 UDP send to soft AP
	q Quit

Press '3' to set computer as TCP server. It will create Socket.

Create a UDP socket and start recv in port 0x8AE.

Set Discover-WIFI as UDP Client by pressing '9'

The output will be :

9
-udpCreateSocket
Socket:07 opened
.
Enter server IP to connect: 
172.31.0.3
Enter server port number: 
2222
Send 10
-udpSendFromSock
.
It will send 10 UDP packets and The SERVER will acknowledge by displaying received packets
----------------------------------------------------------------------------------------
--> a: UDP server - Discover-WIFI works as UDP Server

The above steps can be done in the opposite direction.

@NOTE Computer(client) will send 200 packets to server but only below 256 value 
	  will be shown in server side.
----------------------------------------------------------------------------------------
--> b: Wifi Off - To turn on the Discover-WIFI

The output will be :

b
-SnicCleanup
-WifiOff
.
.
----------------------------------------------------------------------------------------
--> c: Wifi On - To turn off the Discover-WIFI

The output will be :

c
-WifiOn
.
----------------------------------------------------------------------------------------
--> m: Show Menu - It will show the menu

The output will be :
----------------------
0 Get WiFi status
1 Wifi Scan
2 Wifi Join
3 Get IP
4 TCP client
5 TCP sever
6 Send from sock
7 WiFi Leave
8 AP On/Off
9 UDP client
a UDP server
b Wifi Off
c Wifi On
m: Show Menu
q: press q to Quit 
-----------------------
.
-------------------------------------------------------------------------------------------
--> q: Quit - To Quit from the Discover-WIFI menu
-------------------------------------------------------------------------------------------