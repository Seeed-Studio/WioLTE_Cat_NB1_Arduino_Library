// #pragma once

#pragma once
// #include <HardwareSerial.h>

#define UART_DEBUG      (0)
#define WIO_LTE_CAT_NB  (1)

// Module power switch
#define MODULE_PWR_ON  (1)
#define GROVE_PWR_ON   (1)
#define CODEC_PWR_ON   (1)
#define GNSS_ON        (1)
#define ANTENNA_PWR_ON (1)
#define RGB_LED_ON     (1)

#define SerialGrove  Serial   // UART1
#define SerialModule Serial1   // UART2
#define SerialGNSS   Serial2   // UART3
#define SerialDebug  SerialUSB // USB port
#define SerialGNSS_BAUDRATE	9600

#define NOW millis()
#define IS_TIMEOUT(begin, timeout_ms) ((NOW - begin) > timeout_ms)

#define Log_info(x)     SerialDebug.print("[INFO] "), SerialDebug.println(x)
#define Log_error(x)     SerialDebug.print("[ERROR] "), SerialDebug.println(x)
#define Log_in(x)       SerialDebug.print("<"), SerialDebug.println(x)
#define Log_out(x)      SerialDebug.print(">"), SerialDebug.println(x)
#define Log_DumpData(data, dataSize)  for(uint16_t i = 0; i < dataSize; i++) {SerialDebug.print(data[i]);}
 
#if(1==UART_DEBUG)
    #define ERROR(x)            SerialDebug.print("[ERROR] "), SerialDebug.println(x)
    #define DEBUG(x)            SerialDebug.print("[INFO] "), SerialDebug.println(x)
    #define DEBUG_BYTE(x)       SerialDebug.write(x)
    #define DEBUG_IN(x)       SerialDebug.print("<"), SerialDebug.println(x)
    #define DEBUG_OUT(x)      SerialDebug.print(">"), SerialDebug.println(x)
#else
    #define ERROR(x)
    #define DEBUG(x)
    #define DEBUG_BYTE(x)
#endif

