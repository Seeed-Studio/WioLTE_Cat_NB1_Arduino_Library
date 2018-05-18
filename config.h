#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#define UART_DEBUG      (1)
#define WIO_LTE_CAT_NB  (1)

// Module power switch
#define MODULE_PWR_ON  (1)
#define GROVE_PWR_ON   (1)
#define CODEC_PWR_ON   (1)
#define GNSS_ON        (1)
#define ANTENNA_PWR_ON (1)
#define RGB_LED_ON     (1)

#define SerialGrove  Serial1   // UART1
#define SerialModule Serial2   // UART2
#define SerialGNSS   Serial3   // UART3
#define SerialDebug  SerialUSB // USB port

#define SerialGNSS_BAUDRATE	9600


#if(1==UART_DEBUG)
#define ERROR(x)            SerialDebug.println(x)
#define DEBUG(x)            SerialDebug.println(x)
#define DEBUG_BYTE(x)       SerialDebug.write(x)
#else
#define ERROR(x)
#define DEBUG(x)
#define DEBUG_BYTE(x)
#endif

#endif
