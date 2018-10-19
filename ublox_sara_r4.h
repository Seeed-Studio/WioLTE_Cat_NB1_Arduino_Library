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

#include "CMSIS/cmsis_gcc.h"
#include "CMSIS/core_cm4.h"

#define STR_AT	"AT"
#define STR_OK	"OK"
#define STR_ERR	"ERROR"

#define CTRL_Z '\x1A'
#define IP_FORMAT "%d.%d.%d.%d"

#define U32IP_TO_TUPLE(x) (uint8_t)(((x) >> 24) & 0xFF), \
                       (uint8_t)(((x) >> 16) & 0xFF), \
                       (uint8_t)(((x) >> 8) & 0xFF), \
                       (uint8_t)(((x) >> 0) & 0xFF)

#define TUPLE_TO_U32IP(a1, a2, a3, a4) ((((uint32_t)a1) << 24) | \
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

class Ublox_sara_r4
{
	public:	
		uint32_t _u32ip = 0;
		char _str_ip[20] = {'\0'};
		char _operator[32] = {'\0'};				
		char _apn[32] = {'\0'};
		char _user[32] = {'\0'};
		char _passwd[32] = {'\0'};
		bool usedSockId[7] = {false};

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
		void SystemReset(void);
		
		/************************** State Checking and Setting **************************/	
		/**
		 * Initialize module with AT commands
		*/
		bool initialAtCommands(void);

		/**
		 * Turn off echo mode
		*/
		bool disableEchoMode(void);

		/**
		 * Check SIM card status
		 * @return true for SIM ready, false for SIM not ready
		*/
		bool checkSIMStatus(void);

		/** 
		 * Wait for network registration
		 * @returns true on success, false on error
		*/
		bool waitForNetworkRegistered(uint16_t timeout_sec);

		/** 
		 * Get Signal Strength from SIM900 (see AT command: AT+CSQ)
		 * @param signal store signal quality number into.
		 * @return true on success, false on error
		 */
		bool getSignalStrength(int *signal);
		
		
		/** 
		 * Set phone functionarity mode
		 * @param mode is the paramater for CFUN command.  
		 *      0, sleep mode, use the least power consumption;
		 *      1, full function mode;
		 *      4, shut down RF send and receive function;
		 */
		bool set_CFUN(int mode);

		/** 
		 * Set module into sleep mode
		 * @return true on sleep successfully, false on sleep failed
		 */
		bool module_sleep(void);

		/** 
		 * Set module wakeup from sleep mode
		 * @return true on wakeup successfully, false on wakeup failed
		 */
		bool module_wakeup(void);

		/**
		 * Turn off module power buy AT commnad
		 */
		bool AT_PowerDown(void);

		/**
		 * Get real time clock
		 * @param time is a string to store UTC type content
		 * @return none
		*/
		void GetRealTimeClock(char *time);

		/**
		 * Check if module alive
		*/
		bool isAlive(void);

////////////////////////////////////////////////////////////////
///// network, TCP, UDP 
////////////////////////////////////////////////////////////////
		/** 
		 * Check network registration status
		 * @return 0 on success, -1 on error
		 */
		bool network_Init(uint16_t timeout_sec = 60);

		/**
		 * +UGDCONT
		 * 
		*/
		bool parse_cgdcont(void);

		/**
		 * Set APN
		 * @param PDP_type can be "IP", "IPV4", "IPV4V6"
		 * @param APN 
		 * @param PDP_addr 
		 * 
		 * @return true for setting success, false for setting failed.
		 */
		bool setAPN(char* PDP_type, char *APN, char *PDP_addr = "");

		/**
		 * Get IP address
		 */
		bool getIPAddr(void);

		/**
		 * Get operator name
		*/
		bool getOperator(void);
		

		/** 
		 * Parse IP string
		 * @return ip in hex
		 */
		uint32_t str_to_u32(const char* str);

		// /** Check if socket connected
		//  *  @returns true on connected, false on disconnect
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
		 * 
		 * @param sock_type use TCP or UDP to create the socket type
		 * @param port local port for socket
		 * @return socket id
		*/
		int createSocket(Socket_type sock_type, uint16_t port = 0);

		/**
		 * Set host server by ip/domain and port, then connect to the server 
		 * @param sockid is local socket id
		 * @param host is remote server ip or domain
		 * @param port is remote server port
		 * @return true for success, false for failure
		*/
		bool sockConnect(uint8_t sockid, char *host, uint16_t port);

		/**
		 * Close connected socket
		 * 
		 * @param sockid socket id will be closed
		 * @return true for success, false for failure.
		*/

		bool sockClose(int sockid);
		
		/**
		 * Get last socket connect error
		 * 
		 * @return socket error code
		*/
		int getSocketError(void);

		/**
		 * Write one byte data to opened socket
		 * 
		 * @param sockid socket id created before 
		 * @param oneByte one byte will be sent to remote
		 * @return true for success, false for failure.
		 * 
		*/
		bool socketWrite(uint8_t sockid, char oneByte);

		/**
		 * Write data to opened socket
		 * 
		 * @param sockid socket id created before 
		 * @param data data array will be sent to remote
		 * @param dataSize
		 * @return true for success, false for failure.
		 * 
		*/
		bool socketWrite(uint8_t sockid, char *data, uint16_t dataSize);

		/**
		 * Write one byte data to opened socket
		 * 
		 * @param data data array will be sent to remote
		 * @param dataSize
		 * @return - none
		 * 
		*/
		void socketWrite(uint8_t *data, uint16_t dataSize);

		/**
		 * Write one byte to opened socket
		 * 
		 * @param data one byte will be sent to remote
		 * @return - none
		 * 
		*/
		void socketWrite(uint8_t data);

		/**
		 * Send udp message once
		 * 
		 * @param sockid - socket id created before.
		 * @param host - remote server ip address to send message to.
		 * @param port - remote server port. 
		 * @param oneByte - one byte to send.
		 * 
		 * @return true for success, false for failure.
		*/
		bool udpSendTo(uint8_t sockid, char *host, uint16_t port, char oneByte);
		
		/**
		 * Send udp message once.
		 * 
		 * @param sockid - socket id created before.
		 * @param host - remote server ip address to send message to.
		 * @param port - remote server port. 
		 * @param data - Content to send.
		 * @param dataSize length of data to be sent
		 * 
		 * @return true for success, false for failure.
		*/
		bool udpSendTo(uint8_t sockid, char *host, uint16_t port, char *data, uint16_t dataSize);

		/**
		 * Enable TCP UDP direct link mode, thus use AT command to operate connected session.
		*/
		bool enableDirectLinkMode(void);

		/**
		 * Disable TCP UDP direct link mode, thus write or read message straightly from model.
		*/
		bool disableDirectLinkMode(void);	

	private:		
		bool isDirectLinkMode;							
};