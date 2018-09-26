/*
 * ublox_sara_r4_mqtt.h
 * A library for WioLTE Cat NB1
 *
 * Copyright (c) 2018 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : lambor
 * Create Time: September 2018
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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/*!
 * \brief The maximum length of a packet
 */
#define MQTT_MAX_PACKET_LENGTH  100
#define MQTT_DEFAULT_KEEP_ALIVE  60

typedef void (*MQTTClientCallback)(char* topic, char* payload);

class MQTTPacketInfo
{
  public:
    char *_topic;
    char *_msg;
    bool _dup;
    uint8_t _qos;
    bool _retain;
    uint16_t _msg_id;
};

class MQTT
{
public:
    MQTT();
    bool setServer(char * server, uint16_t port = 1883);
    bool setClientId(char *clientId);
    bool setAuth(char * userName, char * passwd);
		bool setInactiveTimeout(uint16_t timeout);
		bool setSecureOpt(uint8_t secure, uint8_t use_profile);
		bool clearSession(uint8_t clear);
    
		// will
    bool setWill(char *topic, char *msg, uint8_t qos = 0, uint8_t retain = 0);
    bool clearWill();

		// non-volatile-memory
		bool saveProfile();
    bool restoreProfile();
    bool deleteProfile();
	
		// MQTT control command
    bool connect(uint16_t timeout_sec = 10u);
		bool disconnect(void);	
    bool publish(const char *topic, const char *msg, uint8_t qos = 0, uint8_t retain = 0);
    bool subscribe(char * topic, uint8_t qos = 0);
		bool unSubscribe(char * topic);
		void onMessage(MQTTClientCallback cb);


    // util
    bool ping(char * server);
    void setPublishHandler(void (*handler)(char *topic, const char *msg, size_t msg_length));
    void setPacketHandler(void (*handler)(char *pckt, size_t len));
    bool loop(void);

    

    void setKeepAlive(uint16_t seconds) 
    { 
      _keepAlive = seconds;
      
    }

   
    
    
    // bool availalePacket();
    // bool open();b
    // void close(bool switchOff=true);
    // bool isConnected();
    // void setStateClosed();

    // Sets the optional "Diagnostics and Debug" stream.
    // void setDiag(Stream &stream) { _diagStream = &stream; }
    // void setDiag(Stream *stream) { _diagStream = stream; }
    
    bool isIPAddress(char *server);

protected:
    uint16_t _keepAlive;
    char * _server;
    uint16_t _port;
    char * _clientName;
    char * _password;
    char * _clientId;
    
private:
  MQTTClientCallback callback;

};

