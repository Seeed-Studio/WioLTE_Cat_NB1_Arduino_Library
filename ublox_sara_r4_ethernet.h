/*
 * ublox_sara_r4_ethernet.h
 *
 * Copyright (c) 2018 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : lambor
 * Create Time: May 2018
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
#include <stdio.h>

#include <stdio.h>
#include <config.h>
#include <ublox_sara_r4.h>
#include <UART_Interface.h>

#define NONE_IP ((IP_t)0)

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

class UBLOX_SARA_R4_Ethernet : public Ublox_sara_r4
{
public:
    uint32_t _u32ip;
    char ip_string[20] = {'\0'};
    char _operator[32] = {'\0'};
    bool usedSockId[7] = {false};
    Ublox_sara_r4 ublox;

    /** Check network registration status
     *  @returns
     *      0 on success
     *      -1 on error
     */
    bool network_Init(uint16 timeout_sec = 20);

    /** Get current PDP Content
     *  @return
     *      true on successfully
     *      flase on failed
     */
    bool syncPDPContent(void);

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
    int8_t createSocket(Socket_type sock_type, uint16_t port = 0);
    bool connect(uint8_t sockid, char *ip, char *port);
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

private:
    

};
extern 


