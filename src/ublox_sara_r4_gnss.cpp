/*
    gnss.cpp
    A library for WioLTE Cat NB1

    Copyright (c) 2018 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Author     : lambor
    Create Time: September 2018
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <ublox_sara_r4_gnss.h>


bool UBLOX_SARA_R4_GNSS::open_GNSS(void) {
    SerialGNSS.begin(SerialGNSS_BAUDRATE);
    turnOnGNSSPower();
}

bool UBLOX_SARA_R4_GNSS::close_GNSS() {
    turnOffGNSSPower();
}

bool UBLOX_SARA_R4_GNSS::dataFlowMode(void) {
    while (SerialGNSS.available()) {
        SerialDebug.write(SerialGNSS.read());
    }
    while (SerialDebug.available()) {
        SerialGNSS.write(SerialDebug.read());
    }
}

// bool UBLOX_SARA_R4_GNSS::getCoordinate(void)
// {
//   int tmp = 0;
//   char *p = NULL;
//   uint8_t str_len = 0;
//   char buffer[128];

//   clean_buffer(buffer, 128);
//   send_cmd("AT+QGPSLOC?\r\n");
//   read_buffer(buffer, 128, 2);
//   // SerialDebug.println(buffer);
//   if(NULL != (p = strstr(buffer, "+CME ERROR")))
//   {
//     return false;
//   }

//   // +QGPSLOC: 084757.700,2235.0272N,11357.9730E,1.6,40.0,3,171.43,0.0,0.0,290617,10
//   else if(NULL != (p = strstr(buffer, "+QGPSLOC:")))
//   {
//     p += 10;
//     p = strtok(buffer, ","); // time
//     p = strtok(NULL, ",");  // latitude
//     sprintf(str_latitude, "%s", p);
//     latitude = strtod(p, NULL);
//     tmp = (int)(latitude / 100);
//     latitude = (double)(tmp + (latitude - tmp*100)/60.0);

//     // Get North and South status
//     str_len = strlen(p);
//     if ((*(p+str_len-1) != 'N') && (*(p+str_len-1) != 'S')){
//       North_or_South[0] = '0';
//       North_or_South[1] = '\0';
//     } else {
//       North_or_South[0] = *(p+str_len-1);
//       North_or_South[1] = '\0';
//     }

//     p = strtok(NULL, ",");  // longitude
//     sprintf(str_longitude, "%s", p);
//     longitude = strtod(p, NULL);

//     // Get West and East status
//     str_len = strlen(p);
//     if ((*(p+str_len-1) != 'W') && (*(p+str_len-1) != 'E')){
//       West_or_East[0] = '0';
//       West_or_East[1] = '\0';
//     } else {
//       West_or_East[0] = *(p+str_len-1);
//       West_or_East[1] = '\0';
//     }

//     tmp = (int)(longitude / 100);
//     longitude = (double)(tmp + (longitude - tmp*100)/60.0);

//     // if(North_or_South[0] == 'S'){
//     //     // latitude = 0.0 - latitude;
//     // } else if(North_or_South[0] = 'N'){
//     //     latitude = 0.0 - latitude;
//     // }

//     // if(West_or_East[0] == 'W'){
//     //     // longitude = 0.0 - longitude;
//     // } else if(West_or_East[0] = 'E'){
//     //     longitude = 0.0 - longitude;
//     // }

//     doubleToString(longitude, latitude);
//   }
//   else{
//     return false;
//   }
//   return true;
// }

void UBLOX_SARA_R4_GNSS::doubleToString(double longitude, double latitude) {
    int u8_lon = (int)longitude;
    int u8_lat = (int)latitude;
    uint32_t u32_lon = (longitude - u8_lon) * 1000000;
    uint32_t u32_lat = (latitude - u8_lat) * 1000000;

    sprintf(str_longitude, "%d.%lu", u8_lon, u32_lon);
    sprintf(str_latitude, "%d.%lu", u8_lat, u32_lat);
}
