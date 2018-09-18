/*
 * ublox_sara_r4_ethernet.cpp
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

#include <ublox_sara_r4_ethernet.h>

bool UBLOX_SARA_R4_Ethernet::network_Init(uint16 timeout_sec)
{
    bool pass = false;
    uint32_t timeStart = 0;
    //AT+CPIN? 
    timeStart = millis();
    while(!check_with_cmd("AT+CPIN?\r\n","+CPIN: READY", CMD, 2)){
        if(millis() - timeStart > timeout_sec*1000UL){
            ERROR("Check SIM card timeout.");
            return false;
        }
    }
    //AT+CREG?
    waitForNetworkRegistered(timeout_sec);          
    //Synchronize the current PDP content
    syncPDPContent();
    return true;
}

bool UBLOX_SARA_R4_Ethernet::syncPDPContent(void)
{
    char *p, *s;
    int i = 0;
    int tries = 0;
    char operate[32] = {'\0'};
    char recvBuffer[128] = {'\0'};    
    int a0,a1,a2,a3;

    // Get IP address, AT+CGDCONT?
    // +CGDCONT: 1,"IP","CMNBIOT1","100.112.210.15",0,0,0,0
    // OK
    clean_buffer(recvBuffer, sizeof(recvBuffer));
    send_cmd("AT+CGDCONT?\r\n");
    read_buffer(recvBuffer, sizeof(recvBuffer));
    DEBUG(recvBuffer);

    if(1 == (sscanf(recvBuffer, "+CGDCONT: 1,\"IP\",\"%s\",\"%d.%d.%d.%d\",0,0,0,0", operate, &a0, &a1, &a2, &a3)))
    {
        _u32ip = TUPLE_TO_IP(a0, a1, a2, a3);
        sprintf(ip_string, "%d.%d.%d.%d", a0, a1, a2, a3);
        if(strlen(operate) > 0)
        {
            memcpy(_operator, operate, strlen(operate));
        }
    }
    else{
        return false;
    }

    return true;
    // p = strtok(recvBuffer, ",");  // +CGDCONT: 1,"IP","CMNBIOT1","100.112.210.15",0,0,0,0
    // p = strtok(NULL, ",");  // "IP","CMNBIOT1","100.112.210.15",0,0,0,0
    // p = strtok(NULL, ",");  // "CMNBIOT1","100.112.210.15",0,0,0,0
    // if(p != NULL) s=p;

    // save operator name
    // s+=1;
    // clean_buffer(_operator, sizeof _operator);
    // while((*(s+i) != '\"') && (*(s+i) != '\0')){
    //     _operator[i] = *(s+i);
    //     i++;
    // }

    // // save IP address
    // p = strtok(NULL, ",");  // "100.112.210.15",0,0,0,0
    // if(p != NULL) s=p;
    // s+=1, i=0;
    // clean_buffer(ip_string, sizeof ip_string);
    // while((*(s+i) != '\"') && (*(s+i) != '\0')){
    //     ip_string[i] = *(s+i);
    //     i++;
    // }

    // ip_string[i] = '\0';
    // _u32ip = str_to_u32(ip_string);
    // if(_u32ip != 0) {
    //     return true;
    // }

}

uint32_t UBLOX_SARA_R4_Ethernet::str_to_u32(const char* str)
{
    uint32_t ip = 0;
    char *p = (char*)str;
    
    for(int i = 0; i < 4; i++) {
        ip |= atoi(p);
        p = strchr(p, '.');
        if (p == NULL) {
            break;
        }
        if(i < 3) ip <<= 8;
        p++;
    }
    return ip;
}

int8_t UBLOX_SARA_R4_Ethernet::createSocket(Socket_type sock_type, uint16_t port) {
    /**
     * The ramge of socket id goes from 0 to 6.
    */
    uint8_t sockIndex;
    bool no_free_socket;
    char sendBuffer[64];
    // char recvBuffer[64];

    clean_buffer(sendBuffer, 64);
    // clean_buffer(recvBuffer, 64);

    // Check is there free socket in the range(0~6)
    // for(sockIndex = 0; sockIndex < 7; sockIndex++) any_sock_free &= usedSockId[sockIndex];
    // if(any_sock_free) return sockIndex=-1; 
    if(port > 0){
        sprintf(sendBuffer, "AT+USOCR=%d,%lu\r\n", sock_type, port);
    }
    else{
        sprintf(sendBuffer, "AT+USOCR=%d\r\n", sock_type);
    }

    // send_cmd(sendBuffer);
    // read_buffer(recvBuffer, 64);
    // if(!check_with_cmd(sendBuffer, "OK", CMD, 2)) return sockIndex=-1;

    // for(sockIndex = 0; sockIndex < 7; sockIndex++) {
    //     if(!usedSockId[sockIndex]) {
    //         usedSockId[sockIndex] = true;
    //         break;
    //     }
    // }

    // return sockIndex;
    return check_with_cmd(sendBuffer, "OK", CMD, 2);
    
}
bool UBLOX_SARA_R4_Ethernet::connect(uint8_t sockid, char *ip, char *port)
{
    char sendBuffer[64];
    
    if(!usedSockId[sockid]) {
        DEBUG("Sockect id not exist.");
        return false;
    }
    sprintf(sendBuffer, "AT+USOCO=%d,\"%s\",%s\r\n", sockid, ip, port);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool UBLOX_SARA_R4_Ethernet::socketWrite(uint8_t sockid, char *ip, char *port, char oneByte)
{
    char sendBuffer[64];
    
    if(!usedSockId[sockid]) {
        DEBUG("Sockect id not exist.");
        return false;
    }
    sprintf(sendBuffer, "AT+USOWR=%d,%d,%c\r\n", sockid, 1, oneByte);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool UBLOX_SARA_R4_Ethernet::socketWrite(uint8_t sockid, char *ip, char *port, char *content)
{
    char sendBuffer[64];

    if(!usedSockId[sockid]) {
        DEBUG("Sockect id not exist.");
        return false;
    }

    sprintf(sendBuffer, "AT+USOWR=%d,%d,%c\r\n", sockid, strlen(content), content);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool UBLOX_SARA_R4_Ethernet::udpSendTo(uint8_t sockid, char *ip, char *port, char oneByte)
{
    char sendBuffer[64];

    // if(!usedSockId[sockid]) {
    //     DEBUG("Sockect id not exist.");
    //     return false;
    // }

    sprintf(sendBuffer, "AT+USOST=%d,\"%s\",%s,%d,\"%c\"\r\n", sockid, ip, port, 1, oneByte);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5, 1000);
}

bool UBLOX_SARA_R4_Ethernet::udpSendTo(uint8_t sockid, char *ip, char *port, char *content) 
{
    char sendBuffer[64];

    // if(!usedSockId[sockid]) {
    //     DEBUG("Sockect id not exist.");
    //     return false;
    // }

    sprintf(sendBuffer, "AT+USOST=%d,\"%s\",%s,%d,\"%s\"\r\n", sockid, ip, port, strlen(content), content);
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool UBLOX_SARA_R4_Ethernet::socketClose(uint8_t sockid)
{
    char sendBuffer[16];

    if(!usedSockId[sockid]) return false;
    sprintf(sendBuffer, "AT+USOCL=%d\r\n", sockid);    
    return check_with_cmd(sendBuffer, "OK", CMD);
}