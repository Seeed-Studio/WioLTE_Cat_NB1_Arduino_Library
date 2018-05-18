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

bool UBLOX_SARA_R4_Ethernet::network_Init(void)
{
    bool pass = false;
    uint32_t timeStart = 0;

    //AT+CPIN? 
    timeStart = millis();
    while(!check_with_cmd("AT+CPIN?\r\n","+CPIN: READY", CMD, 2)){
        if(millis() - timeStart > 10000){
            return false;
        }
    }

    //AT+CREG?
    // timeStart = millis();
    // while(!check_with_cmd("AT+CREG?\r\n","+CREG: 0,1", CMD, 2) && 
    //       !check_with_cmd("AT+CREG?\r\n","+CREG: 0,3", CMD, 2)) 
    // {
    //     if((millis() - timeStart) > 15000) {
    //         return false;
    //     }
    // }
    
    timeStart = millis();
    do {
        pass = check_with_cmd("AT+CREG?\r\n","+CREG: 0,1", CMD, 2) | check_with_cmd("AT+CREG?\r\n","+CREG: 0,3", CMD, 2);
        if((millis() - timeStart) > 20000) {
            return false;
        }
    }while(!pass);

    return true;
}

bool UBLOX_SARA_R4_Ethernet::join(const char *apn, const char *username, const char *passwd)
{
    char sendBuffer[64];
    int errCount = 0;
    uint32_t timeStart = 0;

    // Setting APN, AT+QICSGP=1,1,APN
    clean_buffer(sendBuffer,64);
    sprintf(sendBuffer, "AT+QICSGP=1,1,\"%s\",\"%s\",\"%s\",1\r\n", apn, username, passwd);
    if(!check_with_cmd(sendBuffer, "OK", CMD, DEFAULT_TIMEOUT, 2000)){
        return false;
    }
    
    // Activate PDP context AT+QIACT=1
    timeStart = millis();
    while(!check_with_cmd("AT+QIACT?\r\n","+QIACT:", CMD, 2, 2000)){
        if(!check_with_cmd("AT+QIACT=1\r\n", "OK", CMD, 10, 2000)){
            check_with_cmd("AT+QIDEACT=1\r\n", "OK", CMD, 10, 2000);
        }
        if((millis() - timeStart) > 30000) {
            return false;
        }
    }
    DEBUG("PDP context activating...");
    // Get ip address
    if(!getIP()){
        return false;
    }

    return true;
}

bool UBLOX_SARA_R4_Ethernet::getIP(void)
{
    char *p;
    int i = 0;
    int errCount = 0;
    char ipAddr[32];
    char sendBuffer[32];
    char recvBuffer[64];

    // Get IP address, AT+QICAT
    clean_buffer(ipAddr, 32);    
    clean_buffer(recvBuffer, 64);
    send_cmd("AT+QIACT?\r\n");
    read_buffer(recvBuffer, 64);
    DEBUG(recvBuffer);

    errCount = 0;
    while(NULL == (p = strstr(recvBuffer,"+QIACT:"))) {
        clean_buffer(recvBuffer, 64);
        send_cmd("AT+QIACT?\r\n");
        read_buffer(recvBuffer, 64);
        if(errCount > 5){
            return false;
        }
        errCount++;
    }

    p = strtok(recvBuffer, ",");  // +QIACT: 1,1,1,"10.72.134.66"
    p = strtok(NULL, ",");  // 1,1,"10.72.134.66"
    p = strtok(NULL, ",");  // 1,"10.72.134.66"
    p = strtok(NULL, ",");  // "10.72.134.66"
    p += 1;

    clean_buffer(ip_string, 20);
    while((*(p+i) != '\"') && (*(p+i) != '\0')){
        ip_string[i] = *(p+i);
        i++;
    }

    ip_string[i] = '\0';
    _ip = str_to_ip(p);
    if(_ip != 0) {
        return true;
    }

    return false;
}

uint32_t UBLOX_SARA_R4_Ethernet::str_to_ip(const char* str)
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

char* UBLOX_SARA_R4_Ethernet::recoverIP()
{
    uint8_t a = (_ip>>24)&0xff;
    uint8_t b = (_ip>>16)&0xff;
    uint8_t c = (_ip>>8)&0xff;
    uint8_t d = _ip&0xff;

    snprintf(ip_string, sizeof(ip_string), "%d.%d.%d.%d", a,b,c,d);
    return ip_string;
}

// bool UBLOX_SARA_R4_Ethernet::isNetworkRegistered(void)
// {
//     delay(1000);
//     if((!check_with_cmd("AT+CREG?\r\n","+CREG: 0,1",CMD, DEFAULT_TIMEOUT*3) &&
//         !check_with_cmd("AT+CREG?\r\n","+CREG: 0,3", CMD, 1))) {
//         ERROR("ERROR:CGREG");
//         return false;
//     }
    
//     delay(1000);
//     if(!check_with_cmd("AT+CGATT?\r\n","+CGATT: 1",CMD, DEFAULT_TIMEOUT)) {
//         ERROR("ERROR:CGATT");
//         return false;
//     }
//     return true;
// }

bool UBLOX_SARA_R4_Ethernet::connect(const char *ip, int port, int connectType, uint8_t dataAccessMode)
{
    /**
     * @breif EC21 has 12 socketid can be used, in this function we always use socketid NO.0.
     * @dataAccessMode:
     * 
     * 0.In SOCKET_BUFFER_MODE
     *  - Use "AT+QISEND=0,len" to send data.
     *  - Use "AT+QIRD=0,0" to check buffer size and read out data.
     * 
     * 1.In SOCKET_DIRECT_PUSH_MODE 
     *  - Use "AT+QISEND=0,len" to send data
     *  - Received data will push out directly.
     * 
     * 2.In SOCKET_TRANSPARANT_MODE
     *  - After connect 
     *  - Write data to Module Serial directly.
     *  - Any received data will push out directly.
    */
    uint8_t errCount = 0;
    char cipstart[64];


    if(connectType == TCP) { sprintf(cipstart, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,%d\r\n", ip, port, dataAccessMode); }
    else if(connectType == UDP) { sprintf(cipstart, "AT+QIOPEN=1,0,\"UDP\",\"%s\",%d,0,%d\r\n", ip, port, dataAccessMode); }
    else { return false; } 
        
    while(!check_with_cmd(cipstart, "+QIOPEN: 0", CMD, 2*DEFAULT_TIMEOUT, 2000)) {// connect tcp
        ERROR("ERROR:QIOPEN");
        if(errCount > 3){
            return false;
        }
        errCount++;
    }

    return true;
}


bool UBLOX_SARA_R4_Ethernet::write(char *data)
{
    /** Socket client write process
     * 1.Open
     *      AT+QIOPEN=1,0,"TCP","mbed.org",80,0,1
     * 2 Set data lenght 
     *      AT+QISEND=0,53
     * 3.Put in data
     *      GET /media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n
     * 4.Close socket
     *      AT+QICLOSE=0
    */

    char cmd[32];
    int len = strlen(data); 
    snprintf(cmd,sizeof(cmd),"AT+QISEND=0,%d\r\n",len);
    if(!check_with_cmd(cmd,">", CMD, 2*DEFAULT_TIMEOUT)) {
        ERROR("ERROR:QISEND\r\n"
              "Data length: ");
        ERROR(len);
        return false;
    }
        
    send_cmd(data);
    send_cmd("\r\n");
    // if(!check_with_cmd("\r\n","SEND OK", DATA, 2*DEFAULT_TIMEOUT)) {
    //     ERROR("ERROR:SendData");
    //     return false;
    // }   
    return true;
}

/**
 * \brief Read data from buffer
 * \return bool
*/
bool UBLOX_SARA_R4_Ethernet::read()
{
    char cmd[32];
    snprintf(cmd,sizeof(cmd),"AT+QIRD=0,1500\r\n");
    if(!check_with_cmd(cmd,"+QIURC: \"closed\"", CMD, 2*DEFAULT_TIMEOUT)) {
        ERROR("ERROR:QIRD");
        return false;
    }

    return true;
}

int UBLOX_SARA_R4_Ethernet::getSocketStatus()
{
    /*
        AT+QISTATE=1,0

        +QISTATE: 0,"TCP","xx.xx.xx.xx",xxxx,26824,2,1,0,0,"uart1"

        OK    
    */

    char *p;
    int i = 0;
    int errCount = 0;
    char connect_status[8];
    char recvBuffer[64];

    // Get IP address, AT+QICAT    
    clean_buffer(recvBuffer, 64);
    send_cmd("AT+QISTATE=1,0\r\n");
    read_buffer(recvBuffer, 64);
    DEBUG(recvBuffer);

    errCount = 0;
    while(NULL == (p = strstr(recvBuffer,"+QISTATE:"))) {
        clean_buffer(recvBuffer, 64);
        send_cmd("AT+QISTATE=1,0\r\n");
        read_buffer(recvBuffer, 64);
        if(errCount > 5){
            return -1;
        }
        errCount++;
    }

    p = strtok(recvBuffer, ",");
    p = strtok(NULL, ",");  
    p = strtok(NULL, ",");  
    p = strtok(NULL, ",");
    p = strtok(NULL, ",");
    p = strtok(NULL, ",");
    p = strtok(NULL, ",");  

    clean_buffer(connect_status, 20);
    while((*(p+i) != '\"') && (*(p+i) != '\0')){
        connect_status[i] = *(p+i);
        i++;
    }

    connect_status[i] = '\0';
    return atoi(connect_status);
}

bool UBLOX_SARA_R4_Ethernet::is_connected(void)
{
    return (2 == getSocketStatus()) ? true : false;
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

    sprintf(sendBuffer, "AT+USOCR=%d,%lu\r\n", sock_type, port);
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

    sprintf(sendBuffer, "AT+USOST=%d,\"%s\",%s,%d,%c\r\n", sockid, port, 1, oneByte);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5, 1000);
}

bool UBLOX_SARA_R4_Ethernet::udpSendTo(uint8_t sockid, char *ip, char *port, char *content) 
{
    char sendBuffer[64];

    // if(!usedSockId[sockid]) {
    //     DEBUG("Sockect id not exist.");
    //     return false;
    // }

    sprintf(sendBuffer, "AT+USOST=%d,\"%s\",%s,%d,%s\r\n", sockid, port, strlen(content), content);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool UBLOX_SARA_R4_Ethernet::socketClose(uint8_t sockid)
{
    char sendBuffer[16];

    if(!usedSockId[sockid]) return false;
    sprintf(sendBuffer, "AT+USOCL=%d\r\n", sockid);    
    return check_with_cmd(sendBuffer, "OK", CMD);
}