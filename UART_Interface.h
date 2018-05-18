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

#ifndef __UART_INTERFACE_H__
#define __UART_INTERFACE_H__

#include <Arduino.h>
#include <config.h>

#define DEFAULT_TIMEOUT              5   //seconds
#define DEFAULT_INTERCHAR_TIMEOUT 3000   //miliseconds

enum DataType {
    CMD     = 0,
    DATA    = 1,
};

void  serialPort_init();
void  AT_bypass();
int   check_readable();
int   wait_readable(int wait_time);
void  flush_serial();
uint16_t read_string_line(char *buffer, int count, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
uint16_t read_string_until(char *buffer, int count, char *pattern, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
uint16_t  read_buffer(char* buffer,int count,  unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
void  clean_buffer(char* buffer, int count);
void  send_byte(uint8_t data);
void  send_char(const char c);
void  send_cmd(const char* cmd);
void  send_cmd(const __FlashStringHelper* cmd);
void  send_cmd_P(const char* cmd);
boolean  Test_AT(void);
void  send_End_Mark(void);
boolean wait_for_resp(const char* resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
boolean  check_with_cmd(const char* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT*5);
boolean  check_with_cmd(const __FlashStringHelper* cmd, const char *resp, DataType type, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);

#endif
