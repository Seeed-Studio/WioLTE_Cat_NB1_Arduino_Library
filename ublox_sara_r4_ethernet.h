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

#ifndef __UBLOX_SARA_R4_ETHERNET_H__
#define __UBLOX_SARA_R4_ETHERNET_H__

#include <Arduino.h>
#include <stdio.h>

#include <stdio.h>
#include <config.h>
#include <ublox_sara_r4.h>
#include <UART_Interface.h>

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
    uint32_t _ip;
    char ip_string[20];
    bool usedSockId[7] = {false};

    /** Create Ethernet instance
     *  @param number default phone number during mobile communication
     */
    // Ethernet();

    /** Check network registration status
     *  @returns
     *      0 on success
     *      -1 on error
     */
    bool network_Init(void);

    /** Get IP address
     *  @return
     *      true on successfully
     *      flase on failed
     */
    bool getIP(void);

    /** parse IP string
     *  @return
     *      ip in hex
     */
    uint32_t str_to_ip(const char* str);

    /** Recover Ip address
     *  @return
     *       IP string
     */
    char* recoverIP();

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
    int8_t createSocket(Socket_type sock_type, uint16_t port);
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

#endif 
/* End of file */


