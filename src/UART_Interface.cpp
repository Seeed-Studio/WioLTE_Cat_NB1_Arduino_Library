/*
    UART_Interface.cpp
    A library for Arduino UART Interface

    Copyright (c) 2017 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Author     : lawliet zou, lambor
    Create Time: April 2017
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

#include <UART_Interface.h>


void init_AtTransport() {
    SerialGSM.begin(115200);
}

void AT_bypass() {
    while (SerialGSM.available()) {
        SerialDebug.write(SerialGSM.read());
    }
    while (SerialDebug.available()) {
        SerialGSM.write(SerialDebug.read());
    }
}

int check_readable() {
    return SerialGSM.available();
}

int wait_readable(int wait_time_sec) {
    unsigned long timerStart = NOW;
    int dataLen = 0;
    timerStart = millis();
    do {
        dataLen = check_readable();
    } while (dataLen == 0 && !IS_TIMEOUT(timerStart, wait_time_sec * 1000u));

    return dataLen;
}

void dumpData(uint8_t* data, uint16_t dataSize) {
    for (uint16_t i = 0; i < dataSize; i++) {
        SerialDebug.write(data[i]);
    }
    SerialDebug.println();
}

void flush_serial() {
    while (check_readable()) {
        debugPrint(SerialGSM.read());
    }
}

uint16_t read_string_line(char* buffer, int count, unsigned int timeout, unsigned int chartimeout) {
    uint16_t i = 0;
    bool is_timeout = false;
    unsigned long timerStart, now;
    timerStart = millis();
    now = 0;
    while (1) {
        while (check_readable()) {
            char c = SerialGSM.read();
            now = millis();
            buffer[i++] = c;
            if ((i >= count) || ('\n' == c) || ('\0' == c)) {
                break;
            }
        }
        if (i >= count) {
            break;
        }
        if ((unsigned long)(millis() - timerStart) > timeout * 1000UL) {
            break;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function. Not DO it if we dont recieve at least one char (now <> 0)
        if (((unsigned long)(millis() - now) > chartimeout) && (now != 0)) {
            break;
        }
    }
    return (uint16_t)(i - 1);
}

uint16_t read_string_until(char* buffer, int count, char* pattern, unsigned int timeout, unsigned int chartimeout) {
    uint16_t i = 0;
    uint8_t sum = 0;
    uint8_t len = strlen(pattern);
    bool is_timeout = false;
    unsigned long timerStart, now;

    timerStart = millis();
    now = 0;
    while (1) {
        if (check_readable()) {
            char c = SerialGSM.read();
            now = millis();
            buffer[i++] = c;
            if (i >= count) {
                break;
            }
            sum = (c == pattern[sum]) ? sum + 1 : 0;
            if (sum == len) {
                break;
            }
        }
        if (i >= count) {
            break;
        }
        if ((unsigned long)(millis() - timerStart) > timeout * 1000UL) {
            break;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function. Not DO it if we dont recieve at least one char (now <> 0)
        if (((unsigned long)(millis() - now) > chartimeout) && (now != 0)) {
            break;
        }
    }
    return (uint16_t)(i - 1);
}

uint16_t read_buffer(uint8_t* buffer, uint16_t count, uint16_t timeout, uint16_t chartimeout) {
    uint16_t i = 0;
    bool is_timeout = false;
    unsigned long timerStart, now;
    timerStart = millis();
    now = 0;
    while (1) {
        if (check_readable()) {
            char c = SerialGSM.read();
            debugPrint(c);
            now = millis();
            buffer[i++] = c;
            if (i >= count) {
                break;
            }
        }
        if (i >= count) {
            break;
        }
        if ((unsigned long)(millis() - timerStart) > timeout * 1000UL) {
            break;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function. Not DO it if we dont recieve at least one char (now <> 0)
        if (((unsigned long)(millis() - now) > chartimeout) && (now != 0)) {
            break;
        }
    }
    return (uint16_t)(i - 1);
}

uint16_t read_buffer(char* buffer, uint16_t count, uint16_t timeout, uint16_t chartimeout) {
    uint16_t i = 0;
    bool is_timeout = false;
    unsigned long timerStart, now;
    timerStart = millis();
    now = 0;
    while (1) {
        if (check_readable()) {
            char c = SerialGSM.read();
            debugPrint(c);
            now = millis();
            buffer[i++] = c;
            if (i >= count) {
                break;
            }
        }
        if (i >= count) {
            break;
        }
        if ((unsigned long)(millis() - timerStart) > timeout * 1000UL) {
            break;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function. Not DO it if we dont recieve at least one char (now <> 0)
        if (((unsigned long)(millis() - now) > chartimeout) && (now != 0)) {
            break;
        }
    }
    return (uint16_t)(i - 1);
}

void clean_buffer(char* buffer, int count) {
    for (int i = 0; i < count; i++) {
        buffer[i] = '\0';
    }
}

//HACERR quitar esta funcion ?
void send_byte(uint8_t data) {
    SerialGSM.write(data);
}

void send_char(const char c) {
    SerialGSM.write(c);
}

void send_cmd(const char* cmd) {
    for (uint16_t i = 0; i < strlen(cmd); i++) {
        send_byte(cmd[i]);
    }
}

void send_cmd(const __FlashStringHelper* cmd) {
    int i = 0;
    const char* ptr = (const char*) cmd;
    while (pgm_read_byte(ptr + i) != '\0') {
        send_byte(pgm_read_byte(ptr + i++)); send_byte(pgm_read_byte(ptr + i++));
    }
}

void send_cmd_P(const char* cmd) {
    while (pgm_read_byte(cmd) != '\0') {
        send_byte(pgm_read_byte(cmd++));
    }
}

bool wait_for_resp(const char* resp, DataType type, unsigned int timeout_sec, unsigned int chartimeout_ms) {
    int len = strlen(resp);
    int sum = 0;
    uint32_t timerStart = millis(), timerPreChar = 0;    //timerPreChar is the time when the previous Char has been read.

    while (1) {
        if (check_readable()) {
            char c = SerialGSM.read();
            debugPrint(c);
            timerPreChar = millis();
            sum = (c == resp[sum]) ? sum + 1 : 0;
            if (sum == len) {
                break;
            }
        }
        if (IS_TIMEOUT(timerStart, timeout_sec * 1000u)) {
            return false;
        }
        //If interchar Timeout => return FALSE. So we can return sooner from this function.  chartimeout_ms
        if (IS_TIMEOUT(timerPreChar, chartimeout_ms) && (timerPreChar != 0)) {
            return false;
        }
    }
    debugPrintln("");
    //If is a CMD, we will finish to read buffer.
    if (type == CMD) {
        flush_serial();
    }
    return true;
}

bool wait_for_resp_dot(const char* resp, DataType type, unsigned int timeout_sec) {
    int len = strlen(resp);
    int sum = 0;
    uint32_t timerStart = millis();
    uint32_t dotShowInterval = 100, lastTimeDotShow = millis();

    while (true) {
        if (check_readable()) {
            char c = SerialGSM.read();
            debugPrint(c);
            sum = (c == resp[sum]) ? sum + 1 : 0;
            if (sum == len) {
                break;
            }
        }
        if (IS_TIMEOUT(timerStart, timeout_sec * 1000u)) {
            return false;
        }

        if ((millis() - lastTimeDotShow) > dotShowInterval) {
            lastTimeDotShow = millis();
            Log(".");
        }


    }
    debugPrintln("");
    //If is a CMD, we will finish to read buffer.
    if (type == CMD) {
        flush_serial();
    }
    return true;
}


bool check_with_cmd(const char* cmd, const char* resp, DataType type, unsigned int timeout, unsigned int chartimeout) {
    send_cmd(cmd);
    return wait_for_resp(resp, type, timeout, chartimeout);
}

// bool check_with_cmd(uint8_t *cmd, uint8_t *resp, DataType type, unsigned int timeout, unsigned int chartimeout)
// {
//     send_cmd(cmd);
//     return wait_for_resp(resp,type,timeout,chartimeout);
// }

//HACERR que tambien la respuesta pueda ser FLASH STRING
bool check_with_cmd(const __FlashStringHelper* cmd, const char* resp, DataType type, unsigned int timeout,
                    unsigned int chartimeout) {
    send_cmd(cmd);
    return wait_for_resp(resp, type, timeout, chartimeout);
}
