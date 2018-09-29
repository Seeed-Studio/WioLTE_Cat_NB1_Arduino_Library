/*
 * ublox_sara_r4.cpp
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


#include <ublox_sara_r4.h>
#include <stdio.h>

void peripherial_Init() 
{
	/**
	 * Setting all GPIO to input mode, that avoid power wasting from GPIO
	*/
  for(int i=0; i<64;  i++) {
      pinMode(i, INPUT);
  }      
}

Ublox_sara_r4::Ublox_sara_r4()
{    
  peripherial_Init();  
}

void Ublox_sara_r4::powerOn(void)
{   
  init_AtTransport(); 

  // Set RTS pin down to enable UART communication
  pinMode(RTS_PIN, OUTPUT);
  digitalWrite(RTS_PIN, LOW);

  // Module Power Default HIGH
  pinMode(MODULE_PWR_PIN, OUTPUT);
  digitalWrite(MODULE_PWR_PIN, HIGH); 

  pinMode(PWR_KEY_PIN, OUTPUT);
  digitalWrite(PWR_KEY_PIN, LOW);

  if(isAlive()) {
    return;
  }      
  
  digitalWrite(PWR_KEY_PIN, HIGH);
  delay(800);
  digitalWrite(PWR_KEY_PIN, LOW);
}

void Ublox_sara_r4::turnOnGrovePower(void)
{
  pinMode(GROVE_PWR_PIN, OUTPUT);
  digitalWrite(GROVE_PWR_PIN, HIGH);
}

void Ublox_sara_r4::turnOffGrovePower(void)
{
  pinMode(GROVE_PWR_PIN, OUTPUT);
  digitalWrite(GROVE_PWR_PIN, LOW);
}


void Ublox_sara_r4::turnOnRGBPower(void)
{
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, HIGH);
}

void Ublox_sara_r4::turnOffRGBPower(void)
{
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, LOW);
}

void Ublox_sara_r4::turnOnGNSSPower(void)
{
  pinMode(GNSS_PWR_PIN, OUTPUT);
  digitalWrite(GNSS_PWR_PIN, HIGH);
}

void Ublox_sara_r4::turnOffGNSSPower(void)
{
  pinMode(GNSS_PWR_PIN, OUTPUT);
  digitalWrite(GNSS_PWR_PIN, LOW);
}

bool Ublox_sara_r4::initialAtCommands(void)
{

  // turn echo off
  //  if(RET_OK != disableEchoMode()) {
  //    return false;
  //  }

  // verbose error messages
  if( RET_OK != check_with_cmd("AT+CMEE=2\r\n", "OK", CMD)) {
      return false;
  }

  // enable network identification LED
  if( RET_OK != check_with_cmd("AT+UGPIOC=16,2\r\n", "OK", CMD)) {
      return false;
  }

  // enable mosule power state identification LED
  if( RET_OK != check_with_cmd("AT+UGPIOC=23,10\r\n", "OK", CMD)) {
      return false;
  }

  // SIM check
  if (RET_OK != checkSIMStatus()) {
      return false;
  }

  return true;
} 

bool Ublox_sara_r4::disableEchoMode(void)
{
  if (RET_OK != check_with_cmd("AT E0\r\n", "OK", CMD)) {
      return RET_ERR;
  }

  return RET_OK;
}


bool Ublox_sara_r4::checkSIMStatus(void)
{
    return check_with_cmd("AT+CPIN?\r\n", "+CPIN: READY", CMD);
}

bool Ublox_sara_r4::waitForNetworkRegistered(uint16_t timeout_sec)
{
  bool pass = false;
  uint32_t timeStart = 0;
  
  // check network registration
    timeStart = millis();
    do {
        pass = check_with_cmd("AT+CGATT?\r\n","+CGATT: 1", CMD) && 
               ( check_with_cmd("AT+CREG?\r\n","+CREG: 0,1", CMD) ||
                 check_with_cmd("AT+CREG?\r\n","+CREG: 0,5", CMD));
        if(IS_TIMEOUT(timeStart, timeout_sec * 1000UL)) {
          return false;
        }
    }while(!pass);    

  return true;
}

bool Ublox_sara_r4::getSignalStrength(int *signal)
{
  //AT+CSQ                        --> 6 + CR = 10
  //+CSQ: <rssi>,<ber>            --> CRLF + 5 + CRLF = 9                     
  //OK                            --> CRLF + 2 + CRLF =  6

    byte i = 0;
    char Buffer[26];
    char *p, *s;
    char buffers[4];
    flush_serial();
    send_cmd("AT+CSQ\r");
    clean_buffer(Buffer, 26);
    read_buffer(Buffer, 26);
    if (NULL != (s = strstr(Buffer, "+CSQ:"))) {
        s = strstr((char *)(s), " ");
        s = s + 1;  //We are in the first phone number character 
        p = strstr((char *)(s), ","); //p is last character """
        if (NULL != s) {
            i = 0;
            while (s < p) {
                buffers[i++] = *(s++);
            }
            buffers[i] = '\0';
        }
        *signal = atoi(buffers);
        return true;
    }
    return false;
}

bool Ublox_sara_r4::set_CFUN(int mode)
{
  char txbuf[20] = {'\0'};
  sprintf(txbuf, "AT+CFUN=%d", mode);
  send_cmd(txbuf);
  return check_with_cmd("\n\r", "OK", CMD, 2, 2000);
}

bool Ublox_sara_r4::AT_PowerDown(void)
{
  return check_with_cmd("AT+CPWROFF\r\n", "OK", CMD, 1, 2000);
} 

void Ublox_sara_r4::GetRealTimeClock(char *time)
{
  int i = 0;
  char *p;
  // Command: AT+CCLK?
  // +CCLK: "18/09/17,07:37:20"
  // OK
  char buffer[64] = {'\0'};
  send_cmd("AT+CCLK?\r");
  // read_string_until(buffer, sizeof(buffer), "OK", 2);
  read_buffer(buffer, sizeof buffer);

  if(NULL != (p = strstr(buffer, "+CCLK:")))
  {
    i = 8;
    while(*(p+i) != '\"' && *(p+i) != '\0')
    {
      *(time++) = *(p+i);
      i++;
    }
  }
  else{
    Log_error("Read Real Time Clock Failed.");
    return;
  }
}


bool Ublox_sara_r4::isAlive(void)
{
  bool retVal = check_with_cmd("AT\r\n", "OK", CMD, 1UL);
  return  retVal;
}

bool Ublox_sara_r4::network_Init(uint16 timeout_sec)
{
    bool pass = false;
    uint32_t timeStart = 0;
    
    initialAtCommands();

    //AT+CPIN? 
    timeStart = millis();
    do {
      pass = checkSIMStatus();    
      if(IS_TIMEOUT(timeStart, timeout_sec * 1000UL)) {
        Log_error("check SIM card timeout.");
        return false;
      }
    } while(!pass);

    //AT+CREG?
    if(!waitForNetworkRegistered(timeout_sec)) return false;
    
    //Synchronize the current PDP content
    if(!parse_ugdcont()) return false;

    // Get operator name
    if(!getOperator()) return false;

    return true;
}

bool Ublox_sara_r4::parse_ugdcont(void)
{
  char *p;
  char recvBuffer[128] = {'\0'};    
  int a0,a1,a2,a3;

  // Get IP address, AT+CGDCONT?
  // +CGDCONT: 1,"IP","CMNBIOT1","100.112.210.15",0,0,0,0
  // OK
  clean_buffer(recvBuffer, sizeof(recvBuffer));
  send_cmd("AT+CGDCONT?\r\n");
  read_string_line(recvBuffer, sizeof(recvBuffer));
  debugPrintOut(recvBuffer);
  
  if(NULL != (p = strstr(recvBuffer, "+CGDCONT:")))
  {
    if(5 == (sscanf(p, "+CGDCONT: %*d,\"IP\",\"%[^\"]\",\"%d.%d.%d.%d\",%*d,%*d,%*d,%*d", _apn, &a0, &a1, &a2, &a3)))
    {
      _u32ip = TUPLE_TO_U32IP(a0, a1, a2, a3);
      sprintf(ip_string, IP_FORMAT, a0, a1, a2, a3);
    }
  }    
  else{
      return false;
  }

  return true;
}


bool setAPN(char *APN, char *user, char *passwd)
{
  // TO-DO has not found AT commands for setting APN
  //AT+CGDCONT=
   
}

bool Ublox_sara_r4::getIPAddr()
{
  // AT+CGPADDR=1 
  // +CGPADDR: 1,100.88.38.200
  char *p;
  char rxBuf[64] = {'\0'};
  uint8_t a0, a1, a2, a3;

  send_cmd("AT+CGPADDR=1\r\n");
  read_buffer(rxBuf, sizeof rxBuf);  

  if(NULL != (p = strstr(rxBuf, "+CGPADDR:")))
  {
    if(4 == sscanf(p, "+CGPADDR: %*d,%d.%d.%d.%d", &a0, &a1, &a2, &a3))
    {      
      _u32ip = TUPLE_TO_U32IP(a0, a1, a2, a3);
      sprintf(ip_string, IP_FORMAT, a0, a1, a2, a3);
    }
  }
  else
  {
    Log_error("+CGPADDR failed");
    return RET_ERR;
  }

  return RET_OK;
}

bool Ublox_sara_r4::getOperator()
{
  // AT+COPS?
  // +COPS: 0,0,"operator",9
  char *p, *s;
  char rxBuf[64] = {'\0'};

  send_cmd("AT+COPS?\r\n");
  read_buffer(rxBuf, sizeof rxBuf);  

  if(NULL != (p = strstr(rxBuf, "+COPS:")))
  {
    if(1 == sscanf(p, "+COPS: %*d,%*d,\"%[^\"]\",%*d", _operator))
    {
      // debugPrint("Operator: ");
      // debugPrintln(_operator);
    }
  }
  else
  {
    Log_error("+COPS failed");
    return RET_ERR;
  }

  return RET_OK;
}


uint32_t Ublox_sara_r4::str_to_u32(const char* str)
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

int Ublox_sara_r4::createSocket(Socket_type sock_type, uint16_t port) {
    /**
     * The ramge of socket id goes from 0 to 6.
    */
    uint8_t unusedId;
    bool no_free_socket;
    char txBuf[64];
    char rxBuf[32];
    char *p;
    int newSockid = -1;

    clean_buffer(txBuf, sizeof txBuf);

    // Check is there free socket in the range(0~6)
    for(unusedId = 0; unusedId < 7; unusedId++) 
    {
      if(!usedSockId[unusedId]) break;
    }
    if(unusedId > 6) return -1; 

    if(port > 0){
        sprintf(txBuf, "AT+USOCR=%d,%lu\r\n", sock_type, port);
    }
    else{
        sprintf(txBuf, "AT+USOCR=%d\r\n", sock_type);
    }

    send_cmd(txBuf);
    read_buffer(rxBuf, 64);  //+USOCR: 6
    if(NULL != (p = strstr(rxBuf, "+USOCR:")))
    {
      if(1 == sscanf(p, "+USOCR: %d", &newSockid))
      {
        usedSockId[newSockid] = true;
      }
    }
    
    // return sockIndex;
    return newSockid;    
}

bool Ublox_sara_r4::sockConnect(uint8_t sockid, char *host, uint16_t port)
{
    char txBuf[64];
    
    if(!usedSockId[sockid]) {
        Log_error("Sockect id not exist.");
        return false;
    }

    // Connect to server
    sprintf(txBuf, "AT+USOCO=%d,\"%s\",%d\r\n", sockid, host, port);
    if(!check_with_cmd(txBuf, "OK", CMD, 5)) {
      return false;
    }

    // Enter direct link mode
    if(isDirectLinkMode) {
      sprintf(txBuf, "AT+USODL=%d\r\n", sockid);
      if(!check_with_cmd(txBuf, "CONNECT", CMD, 5)) {
        return false;
      }
    }
    
    return true;
}

bool Ublox_sara_r4::sockClose(int sockid)
{
  //AT+USOCL=<socket>

  bool retVal = false;
  char txBuf[64];
        
  
  if(isDirectLinkMode) {
    delay(1000);
    if(!check_with_cmd("+++", "DISCONNECT", CMD, 2u)) {
      Log_error("Can't close socket.");
    }
  } 

  
  sprintf(txBuf, "AT+USOCL=%d\r\n", sockid);    
  retVal = check_with_cmd(txBuf, "OK", CMD, 10u);
  if(retVal == RET_OK)
  {
    usedSockId[sockid] = false;
  }

  return retVal;
}

int Ublox_sara_r4::getSocketError(void)
{
  // AT+USOER
  int err_code;
  char *p;

  char rxBuf[16] = {'\0'};

  send_cmd("AT+USOER\r\n");
  read_buffer(rxBuf, sizeof(rxBuf));

  if(NULL != (p = strstr(rxBuf, "+USOER")))
  {
    if(sscanf(p, "+USOER: %d", &err_code) != 1)
    {
      err_code = -1;
    }
  }

  return err_code;
}

bool Ublox_sara_r4::socketWrite(uint8_t sockid, char oneByte)
{
    char sendBuffer[32];
    
    if(!usedSockId[sockid]) {
        debugPrintln("Sockect id not exist.");
        return false;
    }
    sprintf(sendBuffer, "AT+USOWR=%d,%d,%c\r\n", sockid, 1, oneByte);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool Ublox_sara_r4::socketWrite(uint8_t sockid, char *data, uint16_t dataSize) 
{
    char txBuf[32+dataSize];

    if(!usedSockId[sockid]) {
        debugPrintln("Sockect id not exist.");
        return false;
    }

    sprintf(txBuf, "AT+USOWR=%d,%d,%s\r\n", sockid, dataSize, data);    
    return check_with_cmd(txBuf, "OK", CMD, 5);
}

void Ublox_sara_r4::socketWrite(uint8_t *data, uint16_t dataSize)
{
  for(uint16_t i = 0; i < dataSize; i++)
  {
    send_byte(data[i]);
  }  
}


void Ublox_sara_r4::socketWrite(uint8_t data)
{
  send_byte(data);
}

bool Ublox_sara_r4::udpSendTo(uint8_t sockid, char *host, uint16_t port, char oneByte)
{
    char sendBuffer[64];

    if(!usedSockId[sockid]) {
        debugPrintln("Sockect id not exist.");
        return false;
    }

    sprintf(sendBuffer, "AT+USOST=%d,\"%s\",%d,%d,\"%c\"\r\n", sockid, host, port, 1, oneByte);    
    return check_with_cmd(sendBuffer, "OK", CMD, 5, 1000);
}

bool Ublox_sara_r4::udpSendTo(uint8_t sockid, char *host, uint16_t port, char *data, uint16_t dataSize) 
{
    char sendBuffer[32+dataSize];

    if(!usedSockId[sockid]) {
        debugPrintln("Sockect id not exist.");
        return false;
    }

    sprintf(sendBuffer, "AT+USOST=%d,\"%s\",%d,%d,\"%s\"\r\n", sockid, host, port, dataSize, data);
    return check_with_cmd(sendBuffer, "OK", CMD, 5);
}

bool Ublox_sara_r4::enableDirectLinkMode(void)
{
  isDirectLinkMode = true;
}

bool Ublox_sara_r4::disableDirectLinkMode(void)
{
  isDirectLinkMode = false;
}
