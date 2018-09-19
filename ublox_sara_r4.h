/*
 * ublox_sara_r4.h
 * A library for SeeedStudio Wio LTE Cat NB1
 *  
 * Copyright (c) 2018 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : lambor
 * Create Time: June 2018
 * Change Log :
 *
 * The MIT License (MIT)
  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <Arduino.h>
#include <UART_Interface.h>

#define STR_AT	"AT"
#define STR_OK	"OK"
#define CTRL_Z '\x1A'

#define CR	"\r"
#define LF	 "\n"
#define CRLF "\r\n"

#define IP_FORMAT "%d.%d.%d.%d"

#define NOW millis()
#define IS_TIMEOUT(begin, timeout_ms) ((NOW - begin) > timeout_ms)

#define IP_TO_TUPLE(x) (uint8_t)(((x) >> 24) & 0xFF), \
                       (uint8_t)(((x) >> 16) & 0xFF), \
                       (uint8_t)(((x) >> 8) & 0xFF), \
                       (uint8_t)(((x) >> 0) & 0xFF)

#define TUPLE_TO_IP(a1, a2, a3, a4) ((((uint32_t)a1) << 24) | \
                                     (((uint32_t)a2) << 16) | \
                                      (((uint32_t)a3) << 8) | \
                                      (((uint32_t)a4) << 0))

enum Socket_type {
    CLOSED = 0,
    TCP    = 6,
    UDP    = 17
};

enum CheckState {
    RET_OK = true,
    RET_ERR = false,
};

// Network registration status.
enum NetworkRegistrationStatuses {
    UnknownNetworkRegistrationStatus = 0,
    Denied,
    NoNetworkRegistrationStatus,
    Home,
    Roaming,
};

// FTP mode.
enum FtpModes {
    ActiveMode = 0,
    PassiveMode,
};

class Ublox_sara_r4
{
	public:
		uint32_t _u32ip;
		char ip_string[20] = {'\0'};
		char _operator[32] = {'\0'};
		char _apn[32] = {'\0'};
		char _user[32] = {'\0'};
		char _passwd[32] = {'\0'};
		bool usedSockId[7] = {false};

		Ublox_sara_r4();
		~Ublox_sara_r4(){};

		/************************** Power Management **************************/
		void powerOn(void);
		void turnOnGrovePower(void);
		void turnOffGrovePower(void);
		void turnOnRGBPower(void);
		void turnOffRGBPower(void);
		void turnOnGNSSPower(void);
		void turnOffGNSSPower(void);
		void debugOn(void);
		void debugOff(void);
		
		/************************** State Checking and Setting **************************/	
		/**
		 * Initialize module with AT commands
		*/
		bool initialAtCommands(void);

		/**
		 * turn off echo mode
		*/
		bool disableEchoMode(void);

		/**
		 * Check SIM card status
		 * @return
		 * 	true for SIM ready
		 * 	false for SIM not ready
		*/
		bool checkSIMStatus(void);

		/** 
		 * Wait for network registration
		 * @returns
		 *  true on success
		 *  false on error
		*/
		bool waitForNetworkRegistered(uint16_t timeout_sec);

		/** 
		 * get Signal Strength from SIM900 (see AT command: AT+CSQ) as integer
		 *  @param
		 *  @returns
		 *    true on success
		 *    false on error
		 */
		bool getSignalStrength(int *signal);
		
		
		/** 
		 * Set phone functionarity mode
		 * @param
		 *      0, least consumption 1, 4
		 *      1, standard mode
		 *      4, shut down RF send and receive function
		 */
		bool set_CFUN(int mode);

		/** Set module into sleep mode
		 * @param
		 * @return 
		 *  true on sleep successfully
		 *  false on sleep failed
		 */
		bool module_sleep(void);

		/** Set module wakeup from sleep mode
		 * @param
		 * @return 
		 *  true on wakeup successfully
		 *  false on wakeup failed
		 */
		bool module_wakeup(void);

		/**
		 * Turn off module power buy AT commnad
		 */
		bool AT_PowerDown(void);

		/**
		 * Get real time clock
		 * @param_in time
		 * @return 
		*/
		void GetRealTimeClock(char *time);

		/**
		 * Check if module alive
		*/
		bool isAlive(void);

////////////////////////////////////////////////////////////////
///// network, TCP, UDP 
////////////////////////////////////////////////////////////////
		/** Check network registration status
		 *  @returns
		 *      0 on success
		 *      -1 on error
		 */
		bool network_Init(uint16 timeout_sec = 20);

		/**
		 * +UGDCONT
		 * 
		*/
		bool read_ugdcont(void);

		/**
		 * Set APN 
		 */
		bool setAPN(char *APN, char *user, char *passwd);

		/**
		 * Connect to APN
		 */
		bool connectAPN();

		/**
		 * Get IP address
		 */
		bool getIPAddr();

		/**
		 * Get operator name
		*/
		bool getOperator();
		

		/** parse IP string
		 *  @return
		 *      ip in hex
		 */
		uint32_t str_to_u32(const char* str);
		
		/** Create Ethernet instance
		 *  @param number default phone number during mobile communication
		 */
		// Ethernet();    

		// /** check network is OK or not
		//  *  @returns
		//  *      true on success
		//  *      false on error
		//  */
		// bool networkCheck(void);

		// /** Write data to socket server
		//  *  @param  data    data that will be send to socket server
		//  *  @returns
		//  *      0 on success
		//  *      -1 on error
		//  */
		// bool write(char *data);

		// /** Read data from 
		//  * 
		//  * 
		//  * 
		// */
		// bool read();

		// /** Get socket status
		//  *  @returns
		//  *      -1 on error
		//  *      0 "Initial" connection has not been established
		//  *      1 "Openning" client is connecting ro server is trying to listen
		//  *      2 "Connected" client/incoming connection has been establish
		//  *      3 "Listening" server is listening 
		//  *      4 "Closing" connection is closing
		//  */
		// int getSocketStatus();

		// /** Check if socket connected
		//  *  @returns
		//  *          true on connected
		//  *          false on disconnect
		//  */
		// bool is_connected(void);

		// /** close TCP connection
		//  *  @returns
		//  *      0 on success
		//  *      -1 on error
		//  */
		//  bool httpGET(char *url);
		
		//  /** close TCP connection
		//  *  @returns
		//  *      0 on success
		//  *      -1 on error
		//  */
		// bool httpPUT(char *url);

		/**
		 * Create socket and return socket id
		 * @return socket id
		*/
		int createSocket(Socket_type sock_type, uint16_t port = 0);
		bool sockConnect(uint8_t sockid, char *ip, char *port);
		bool sockClose(int sockid);
		int getSocketError(void);
		bool socketWrite(uint8_t sockid, char *ip, char *port, char oneByte);
		bool socketWrite(uint8_t sockid, char *ip, char *port, char *content);
		bool udpSendTo(uint8_t sockid, char *ip, char *port, char oneByte);
		/**
		 * Send udp message.
		 * @sockid - socket id created before.
		 * @ip - remote server ip address to send message to.
		 * @port - remote server port. 
		 * @message - Content to send.
		 * 
		 * @return - return true for success, false for failure.
		*/
		bool udpSendTo(uint8_t sockid, char *ip, char *port, char *content);
		bool socketClose(uint8_t sockid);

		/************************** MCU Pin Definitions **************************/

		const int CTS_PIN            =  0; // PA0
		const int RTS_PIN            =  1; // PA1
		const int RGB_LED_PWR_PIN    =  8; // PA8
		const int SD_PWR_PIN         = 15; // PA15
		const int BAT_C_PIN          = 16; // PB0
		const int RGB_LED_PIN        = 17; // PB1
		const int MODULE_PWR_PIN     = 21; // PB5
		const int ENABLE_VCCB_PIN    = 26; // PB10    
		const int GNSS_PWR_PIN       = 28; // PB12
		const int RX_LED_PIN         = 29; // PB13
		const int TX_LED_PIN         = 30; // PB14
		const int GNSS_1PPS_PIN      = 31; // PB15
		const int GROVE_PWR_PIN      = 32; // PC0
		const int GNSS_RST_PIN       = 33; // PC1
		const int GNSS_INT_PIN       = 34; // PC2
		const int RESET_MODULE_PIN   = 35; // PC3
		const int PWR_KEY_PIN        = 36; // PC4

				
};

    // {GPIOA, TIMER5, ADC1,  0, 1,    0}, /* D00/PA0  */
    // {GPIOA, TIMER5, ADC1,  1, 2,    1}, /* D01/PA1  */
    // {GPIOA, TIMER5, ADC1,  2, 3,    2}, /* D02/PA2  */
    // {GPIOA, TIMER5, ADC1,  3, 4,    3}, /* D03/PA3  */
    // {GPIOA,   NULL, ADC1,  4, 0,    4}, /* D04/PA4  */
    // {GPIOA,   NULL, ADC1,  5, 0,    5}, /* D05/PA5  */
    // {GPIOA,   NULL, ADC1,  6, 1,    6}, /* D06/PA6  */ // ala check TIMER3
    // {GPIOA,   NULL, ADC1,  7, 0,    7}, /* D07/PA7  */
    // {GPIOA,   NULL, NULL,  8, 0, ADCx}, /* D08/PA8  */ // remap out
    // {GPIOA,   NULL, NULL,  9, 0, ADCx}, /* D09/PA9  */ // remap out
    // {GPIOA,   NULL, NULL, 10, 0, ADCx}, /* D10/PA10 */ // remap out
    // {GPIOA,   NULL, NULL, 11, 0, ADCx}, /* D11/PA11 */ // remap out
    // {GPIOA,   NULL, NULL, 12, 0, ADCx}, /* D12/PA12 */
    // {GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D13/PA13 */
    // {GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D14/PA14 */
    // {GPIOA, TIMER2, NULL, 15, 1, ADCx}, /* D15/PA15 */ // remap in

    // {GPIOB, TIMER3, ADC1,  0, 3,    8}, /* D16/PB0  */
    // {GPIOB, TIMER3, ADC1,  1, 4,    9}, /* D17/PB1  */
    // {GPIOB,   NULL, NULL,  2, 0, ADCx}, /* D18/PB2  */
    // {GPIOB, TIMER2, NULL,  3, 2, ADCx}, /* D19/PB3  */ // remap in
    // {GPIOB, TIMER3, NULL,  4, 1, ADCx}, /* D20/PB4  */ // remap in
    // {GPIOB, TIMER3, NULL,  5, 2, ADCx}, /* D21/PB5  */ // remap in
    // {GPIOB,   NULL, NULL,  6, 0, ADCx}, /* D22/PB6  */ // remap out
    // {GPIOB,   NULL, NULL,  7, 0, ADCx}, /* D23/PB7  */ // remap out
    // {GPIOB,   NULL, NULL,  8, 0, ADCx}, /* D24/PB8  */ // remap out
    // {GPIOB,   NULL, NULL,  9, 0, ADCx}, /* D25/PB9  */ // remap out
    // {GPIOB,   NULL, NULL, 10, 0, ADCx}, /* D26/PB10 */
    // {GPIOB,   NULL, NULL, 11, 0, ADCx}, /* D27/PB11 */
    // {GPIOB,   NULL, NULL, 12, 0, ADCx}, /* D28/PB12 */
    // {GPIOB,   NULL, NULL, 13, 0, ADCx}, /* D29/PB13 */
    // {GPIOB,   NULL, NULL, 14, 0, ADCx}, /* D30/PB14 */
    // {GPIOB,   NULL, NULL, 15, 0, ADCx}, /* D31/PB15 */

    // {GPIOC,   NULL, ADC1,  0, 0,   10}, /* D32/PC0  */
    // {GPIOC,   NULL, ADC1,  1, 0,   11}, /* D33/PC1  */
    // {GPIOC,   NULL, ADC1,  2, 0,   12}, /* D34/PC2  */
    // {GPIOC,   NULL, ADC1,  3, 0,   13}, /* D35/PC3  */
    // {GPIOC,   NULL, ADC1,  4, 0,   14}, /* D36/PC4  */
    // {GPIOC,   NULL, ADC1,  5, 0,   15}, /* D37/PC5  */
    // {GPIOC, TIMER8, NULL,  6, 1, ADCx}, /* D38/PC6  */
    // {GPIOC, TIMER8, NULL,  7, 2, ADCx}, /* D39/PC7  */
    // {GPIOC, TIMER8, NULL,  8, 3, ADCx}, /* D40/PC8  */
    // {GPIOC, TIMER8, NULL,  9, 4, ADCx}, /* D41/PC9  */
    // {GPIOC,   NULL, NULL, 10, 0, ADCx}, /* D42/PC10 */
    // {GPIOC,   NULL, NULL, 11, 0, ADCx}, /* D43/PC11 */
    // {GPIOC,   NULL, NULL, 12, 0, ADCx}, /* D44/PC12 */
    // {GPIOC,   NULL, NULL, 13, 0, ADCx}, /* D45/PC13 */
    // {GPIOC,   NULL, NULL, 14, 0, ADCx}, /* D46/PC14 */
    // {GPIOC,   NULL, NULL, 15, 0, ADCx}, /* D47/PC15 */

    // {GPIOD,   NULL, NULL,  0, 0, ADCx}, /* D48/PD0  */
    // {GPIOD,   NULL, NULL,  1, 0, ADCx}, /* D49/PD1  */
    // {GPIOD,   NULL, NULL,  2, 0, ADCx}, /* D50/PD2  */