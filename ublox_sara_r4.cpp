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

void peripherial_Init(){
	/**
	 * Setting all GPIO to input mode, that avoid power wasting from GPIO
	*/
    for(int i=0; i<64;  i++){
        pinMode(i, INPUT);
    }    
}

Ublox_sara_r4::Ublox_sara_r4()
{
    peripherial_Init();
    
#if(1 == RGB_LED_ON)
    pinMode(RGB_LED_PWR_PIN, OUTPUT);
    digitalWrite(RGB_LED_PWR_PIN, HIGH);  // RGB LED Power
#endif   
}   

bool Ublox_sara_r4::Check_If_Power_On(void)
{
    if(check_with_cmd(F("AT\r\n"), "OK", CMD, 2, 2000)){
        return true;
    }
    return false;
}

void Ublox_sara_r4::powerOn(void)
{
  int pwr_status = 1;
  int errCnt = 0;

  serialPort_init();

  // Set RTS pin down to enable UART communication
  pinMode(RTS_PIN, OUTPUT);
  digitalWrite(RTS_PIN, LOW);

  if(Check_If_Power_On()){
    return;
  }    

#if(1 == MODULE_PWR_ON)
  pinMode(MODULE_PWR_PIN, OUTPUT);
  digitalWrite(MODULE_PWR_PIN, HIGH);     // Module Power Default HIGH
#endif 

#if(1 == GROVE_PWR_ON)
  pinMode(GROVE_PWR_PIN, OUTPUT);
  digitalWrite(GROVE_PWR_PIN, HIGH);    // VCC_B Enable pin
#endif

  pinMode(PWR_KEY_PIN, OUTPUT);
  digitalWrite(PWR_KEY_PIN, LOW);
  digitalWrite(PWR_KEY_PIN, HIGH);
  delay(800);
  digitalWrite(PWR_KEY_PIN, LOW);
}

void Ublox_sara_r4::enableGrovePower(void)
{
  pinMode(GROVE_PWR_PIN, OUTPUT);
  digitalWrite(GROVE_PWR_PIN, HIGH);
}

void Ublox_sara_r4::disableGrovePower(void)
{
  pinMode(GROVE_PWR_PIN, OUTPUT);
  digitalWrite(GROVE_PWR_PIN, LOW);
}


void Ublox_sara_r4::enableRGBPower(void)
{
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, HIGH);
}

void Ublox_sara_r4::disableRGBPower(void)
{
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, LOW);
}

void Ublox_sara_r4::enableGNSSPower(void)
{
  pinMode(GNSS_PWR_PIN, OUTPUT);
  digitalWrite(GNSS_PWR_PIN, HIGH);
}

void Ublox_sara_r4::disableGNSSPower(void)
{
  pinMode(GNSS_PWR_PIN, OUTPUT);
  digitalWrite(GNSS_PWR_PIN, LOW);
}

bool Ublox_sara_r4::initialAtCommands(void)
{
  // verbose error messages
  if( ATSuccess != check_with_cmd("AT+CMEE=2", "OK", CMD)) {
      return false;
  }

  // enable network identification LED
  if( ATSuccess != check_with_cmd("AT+UGPIOC=16,2\r\n", "OK", CMD)) {
      return false;
  }

  // enable mosule power state identification LED
  if( ATSuccess != check_with_cmd("AT+UGPIOC=23,10\r\n", "OK", CMD)) {
      return false;
  }

  // SIM check
  if (ATSuccess != checkSIMStatus()) {
      return false;
  }

  return true;
} 

bool Ublox_sara_r4::disableEchoMode(void)
{
  if (ATSuccess != check_with_cmd("AT E0\r\n", "OK", CMD)) {
      return ATError;
  }

  return ATSuccess;
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
        pass = check_with_cmd("AT+CGATT?\r\n","+CGATT: 1", CMD, 2);
        if((millis() - timeStart) > timeout_sec*1000UL) {
            return false;
        }
    }while(!pass);

    timeStart = millis();
    do {
        pass = check_with_cmd("AT+CREG?\r\n","+CREG: 0,1", CMD, 2) | // Registered, Home network
               check_with_cmd("AT+CREG?\r\n","+CREG: 0,3", CMD, 2);  // Registered, Roaming 
        if((millis() - timeStart) > timeout_sec*1000UL) {
            return false;
        }
    }while(!pass);

  return true;
}

// bool Ublox_sara_r4::write(char *data)
// {
//     /** Socket client write process
//      * 1.Open
//      *      AT+QIOPEN=1,0,"TCP","mbed.org",80,0,1
//      * 2 Set data lenght 
//      *      AT+QISEND=0,53
//      * 3.Put in data
//      *      GET /media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n
//      * 4.Close socket
//      *      AT+QICLOSE=0
//     */

//     char cmd[32];
//     int len = strlen(data); 
//     snprintf(cmd,sizeof(cmd),"AT+QISEND=0,%d\r\n",len);
//     if(!check_with_cmd(cmd,">", CMD, 2*DEFAULT_TIMEOUT)) {
//         ERROR("ERROR:QISEND\r\n"
//               "Data length: ");
//         ERROR(len);
//         return false;
//     }
        
//     send_cmd(data);
//     send_cmd("\r\n");
//     // if(!check_with_cmd("\r\n","SEND OK", DATA, 2*DEFAULT_TIMEOUT)) {
//     //     ERROR("ERROR:SendData");
//     //     return false;
//     // }   
//     return true;
// }

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
  char txbuf[20];
  clean_buffer(txbuf, 20);
  sprintf(txbuf, "AT+CFUN=%d", mode);
  send_cmd(txbuf);
  return check_with_cmd("\n\r", "OK", CMD, 2, 2000);
}

bool Ublox_sara_r4::AT_PowerDown(void)
{
  return check_with_cmd("AT+CPWROFF\n\r", "OK", CMD, 1, 2000);
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
  read_string_until(buffer, sizeof(buffer), "OK", 2);

  if(NULL != (p = strstr(buffer, "+CCLK:")))
  {
    i = 8;
    Log_out(p);
    while(*(p+i) != '\"' && *(p+i) != '\0')
    {
      *(time++) = *(p+i);
      i++;
    }
  }
  else{
    ERROR("Read Real Time Clock Failed.");
    return;
  }
}


bool Ublox_sara_r4::isAlive(void)
{
    return check_with_cmd("AT\r\n", "OK", CMD, 1UL);
}