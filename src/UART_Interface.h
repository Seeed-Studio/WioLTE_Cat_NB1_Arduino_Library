/*
 * EC21_Common.h
 * A library for SeeedStudio Wio Tracker
 *
 * Copyright (c) 2017 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : lawliet zou, lambor
 * Create Time: April 2017
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
#include <stdbool.h>

// #define CR	"\r"
#define LF	 "\n"
#define CRLF "\r\n"
#define PRE_FIX_ERR "[ERROR] "
#define PRE_FIX_INFO "[INFO] "

#define DEFAULT_TIMEOUT              (1UL)   //seconds
#define DEFAULT_INTERCHAR_TIMEOUT (3000UL)   //miliseconds

#define SerialGrove  Serial   // UART1
#define SerialGSM    Serial1   // UART2
#define SerialGNSS   Serial2   // UART3
#define SerialDebug  SerialUSB // USB port
#define SerialGNSS_BAUDRATE	9600

#define Log(x)     (SerialDebug.print(x))
#define Logln(x)     (SerialDebug.println(x))
#define Log_info(x)     (SerialDebug.print("[INFO] "), SerialDebug.println(x))
#define Log_error(x)     (SerialDebug.print("[ERROR] "), SerialDebug.println(x))
#define Log_prolog_in(x)       (SerialDebug.print("<<"), SerialDebug.println(x))
#define Log_prolog_out(x)      (SerialDebug.print(">>"), SerialDebug.println(x))

// #define UART_DBEUG

#ifdef UART_DBEUG
#define debugPrint(x)     SerialDebug.print(x)
#define debugPrintln(x)   SerialDebug.println(x)
#define debugPrintIn(x)     SerialDebug.print("<< "),SerialDebug.printl(x)
#define debugPrintOut(x)     SerialDebug.print(">> "),SerialDebug.println(x)
#else
#define debugPrint(x)
#define debugPrintln(x)
#define debugPrintOut(x)
#endif

#define NOW millis()
#define IS_TIMEOUT(begin, timeout_ms) ((NOW - begin) > timeout_ms)

enum DataType {
    CMD     = 0,
    DATA    = 1,
};

void init_AtTransport();
void AT_bypass();
int   check_readable();
int   wait_readable(int wait_time_sec);
void flush_serial();
void dumpData(uint8_t *data, uint16_t dataSize);
uint16_t read_string_line(char *buffer, int count, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
uint16_t read_string_until(char *buffer, int count, char *pattern, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
uint16_t read_buffer(uint8_t *buffer, uint16_t count, uint16_t timeout = DEFAULT_TIMEOUT, uint16_t chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
uint16_t read_buffer(char *buffer, uint16_t count, uint16_t timeout = DEFAULT_TIMEOUT, uint16_t chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
void clean_buffer(char* buffer, int count);
void send_byte(uint8_t data);
void send_char(const char c);
void send_cmd(const char* cmd);
void send_cmd(const __FlashStringHelper* cmd);
void send_cmd_P(const char* cmd);
bool wait_for_resp(const char* resp, DataType type, unsigned int timeout_sec = DEFAULT_TIMEOUT, unsigned int chartimeout_ms = DEFAULT_INTERCHAR_TIMEOUT);
bool wait_for_resp_dot(const char* resp, DataType type, unsigned int timeout_sec);
bool check_with_cmd(const char* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT*5);
bool check_with_cmd(const __FlashStringHelper* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);