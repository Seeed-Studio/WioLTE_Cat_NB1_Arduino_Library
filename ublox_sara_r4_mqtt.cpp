
/*
 * ublox_sara_r4_mqtt.cpp
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

#include <Arduino.h>

#include <ublox_sara_r4.h>
#include <UART_Interface.h>
#include <ublox_sara_r4_mqtt.h>


MQTT::MQTT()
{
	callback = NULL;
}

bool MQTT::setServer(char * server, uint16_t port)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	_server = server;
	_port = port;

	if(isIPAddress(server)) 
	{
		sprintf(txBuf, "AT+UMQTT=3,\"%s\",%d"CRLF, server, port);
		retVal = check_with_cmd(txBuf, "+UMQTT: 3,1", CMD);
	}
	else 
	{
		sprintf(txBuf, "AT+UMQTT=2,\"%s\",%d"CRLF, server, port);
		retVal = check_with_cmd(txBuf, "+UMQTT: 2,1", CMD);
	}				
	return retVal;				
}

bool setClientId(char *clientId)
{
	//AT+UMQTT=0,<client_id>
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTT=0,\"%s\""CRLF, clientId);
	retVal = check_with_cmd(txBuf, "+UMQTT: 0,1", CMD);	
	return retVal;

}

bool MQTT::setAuth(char *userName, char *passwd)
{
	// AT+UMQTT=4,<username>, <password>
	bool retVal = false;
	char txBuf[64] = {'\0'};

	_clientName = userName;
	_password = passwd;

	sprintf(txBuf, "AT+UMQTT=4,\"%s\",\"%s\""CRLF, userName, passwd);
	retVal = check_with_cmd(txBuf, "+UMQTT: 4,1", CMD);
	return retVal;

}

bool MQTT::setInactiveTimeout(uint16_t timeout)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTT=10,%d"CRLF, timeout);
	retVal = check_with_cmd(txBuf, "+UMQTT: 10,1", CMD);
	return retVal;
}


bool MQTT::clearSession(uint8_t clear)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTT=12,%d"CRLF, clear);
	retVal = check_with_cmd(txBuf, "+UMQTT: 12,1", CMD);
	return retVal;
}

bool MQTT::setWill(char *topic, char *msg, uint8_t qos, uint8_t retain)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTTWTOPIC=%d,%d,%s"CRLF, qos, retain, topic);
	retVal = check_with_cmd(txBuf, "+UMQTTWTOPIC: 1", CMD);
	if(!retVal) {
		debugPrintln(PRE_FIX_ERR"Set will topic error");
		return false;
	} 

	sprintf(txBuf, "AT+UMQTTWMSG=\"%s\""CRLF, msg);
	retVal = check_with_cmd(txBuf, "+UMQTTWMSG: 1", CMD);
	if(!retVal) {
		debugPrintln(PRE_FIX_ERR"Set will msg error");
		return false;
	}

	return true;
}

bool MQTT::clearWill(void)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTTWTOPIC=%d,%d,%s"CRLF, 0, 0, "");
	retVal = check_with_cmd(txBuf, "+UMQTTWTOPIC: 1", CMD);

	sprintf(txBuf, "AT+UMQTTWMSG=\"%s\""CRLF, "");
	retVal &= check_with_cmd(txBuf, "+UMQTTWMSG: 1", CMD);

	return retVal;
}

bool MQTT::saveProfile()
{
	//AT+UMQTTNV=2
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTNV=2"CRLF, "+UMQTTNV: 2,1", CMD);
	return retVal;
}
bool restoreProfile()
{
	//AT+UMQTTNV=1
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTNV=1"CRLF, "+UMQTTNV: 1,1", CMD);
	return retVal;
}

bool deleteProfile()
{
	//AT+UMQTTNV=0
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTNV=0"CRLF, "+UMQTTNV: 0,1", CMD);
	return retVal;
}

bool MQTT::disconnect(void)
{
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTC=0"CRLF, "+UMQTTC: 0", CMD);
	return retVal;
}

bool MQTT::connect(uint16_t timeout_sec)
{
	bool retVal = false;

	send_cmd("AT+UMQTTC=1"CRLF);	
	retVal = wait_for_resp_dot("+UMQTTC: 1,1", CMD, timeout_sec);

	return retVal;
}

bool MQTT::publish(const char *topic, const char *msg, uint8_t qos, uint8_t retain)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};
	sprintf(txBuf, "AT+UMQTTC=2,%d,%d,\"%s\",\"%s\""CRLF, qos, retain, topic, msg);
	retVal = check_with_cmd(txBuf, "+UMQTTC: 2,1", CMD);		
	return retVal;
}

bool MQTT::subscribe(char *topic, uint8_t qos)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};
	sprintf(txBuf, "AT+UMQTTC=4,%d,\"%s\""CRLF, qos, topic);
	retVal = check_with_cmd(txBuf, "+UMQTTC: 4,1", CMD);
	sprintf(txBuf, "MQTT set sublish Topic \"%s\"", topic);
	Log_info(txBuf);
	return retVal;
}

bool MQTT::unSubscribe(char *topic)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};
	sprintf(txBuf, "AT+UMQTTC=5,\"%s\""CRLF, topic);
	retVal = check_with_cmd(txBuf, "+UMQTTC: 5,1", CMD);
	return retVal;
}

bool MQTT::ping(char * server)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};
	sprintf(txBuf, "AT+UMQTTC=8,\"%s\""CRLF, server);
	retVal = check_with_cmd(txBuf, "+UMQTTC: 8,1", CMD);
	return retVal;
}

void MQTT::onMessage(MQTTClientCallback cb) {	
	callback = cb;
}

bool MQTT::loop(void)
{
	// Is there a packet?
	bool status;
	uint16_t pckt_size;
	uint16_t pckt_ix;
	uint8_t msgCnt = 0;
	char *p;

	if (check_readable() > 0) 
	{
		char mqtt_packets[256] = {'\0'};		
		char topic[128] = {'\0'};
		char msg[128] = {'\0'};

		pckt_size = read_buffer(mqtt_packets, sizeof(mqtt_packets));
		if(NULL != (p = strstr(mqtt_packets, "+UUMQTTCM: 6")))
		{
			if(1 == sscanf(p, "+UUMQTTCM: 6,%d", &msgCnt))
			{				
				// Log_info("Recv message count: ");
				// Log(msgCnt);
				// Log(CRLF);

				clean_buffer(mqtt_packets, sizeof mqtt_packets);
				send_cmd("AT+UMQTTC=6"CRLF);
				read_buffer(mqtt_packets, sizeof mqtt_packets);

				if(NULL != (p = strstr(mqtt_packets, "+UUMQTTCM: 6,1")))
				{
					p = strstr(p, "Topic:");
					if(2 == sscanf(p, "Topic:%s"CRLF"Msg:%s", topic, msg))
					{
						if(NULL != callback)
						{
							callback((char*)topic, (char*)msg);
						}						
					}
				}
			}
		}
		if (pckt_size > 0) 
		{
				/* Log out anything received from model */	
				// Log("[MQTT] received packet with length ");
				// Log(strlen(mqtt_packets));
				// Logln(":");
				// dumpData((uint8_t*)mqtt_packets, strlen(mqtt_packets));
		}
		
		return true;
	}

	return false;
}

bool MQTT::isIPAddress(char *server)
{
	int a;
	return (4 == sscanf(server, IP_FORMAT, &a, &a, &a, &a));
}