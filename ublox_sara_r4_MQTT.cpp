#include <Arduino.h>

#include <ublox_sara_r4.h>
#include <UART_Interface.h>

#include <ublox_sara_r4_mqtt.h>

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

bool MQTT::setServer(char * server, uint16_t port)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	_server = server;
	_port = port;

	if(isIPAddress(server)) 
	{
		sprintf(txBuf, "AT+UMQTT=3,\"%s\",%d\r\n"CRLF, server, port);
		retVal = check_with_cmd(txBuf, "+UMQTT: 3,1", CMD);
	}
	else 
	{
		sprintf(txBuf, "AT+UMQTT=2,\"%s\",%d"CRLF, server, port);
		retVal = check_with_cmd(txBuf, "+UMQTT: 2,1", CMD);
	}				
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

bool MQTT::setWillTopic(char *topic)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTTWTOPIC=0,0,%s"CRLF, topic);
	retVal = check_with_cmd(txBuf, "+UMQTTWTOPIC: 1", CMD);
	return retVal;
}

bool MQTT::setWillMessage(char *message)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};

	sprintf(txBuf, "AT+UMQTTWMSG=\"%s\""CRLF, message);
	retVal = check_with_cmd(txBuf, "+UMQTTWMSG: 1", CMD);
	return retVal;
}


bool MQTT::set_umqttnv(uint8_t nvMode)
{

}

bool  MQTT::disconnect(void)
{
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTC=0"CRLF, "+UMQTTC: 0,1", CMD);
	return retVal;
}

bool MQTT::connect(uint16_t timeout_sec)
{
	bool retVal = false;
	uint8_t _retry = retry;
	retVal = check_with_cmd("AT+UMQTTC=1"CRLF, "+UMQTTC: 1,1", CMD, timeout_sec);
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
		MQTTPacketInfo pckt_info;

		pckt_size = read_buffer(mqtt_packets, sizeof(mqtt_packets));
		if(NULL != (p = strstr(mqtt_packets, "+UUMQTTCM: 6")))
		{
			if(1 == sscanf(p, "+UUMQTTCM: 6,%d", &msgCnt))
			{
				Log_info("Recv message count: ");
				Log(msgCnt);
				Log(CRLF);

				clean_buffer(mqtt_packets, sizeof mqtt_packets);
				send_cmd("AT+UMQTTC=6"CRLF);
				read_buffer(mqtt_packets, sizeof mqtt_packets);

				if(NULL != (p = strstr(mqtt_packets, "+UUMQTTCM: 6,1")))
				{
					p = strstr(p, "Topic:");
					if(2 == sscanf(p, "Topic:%s"CRLF"Msg:%s", topic, msg))
					{
						memcpy(pckt_info._topic, topic, strlen(topic));
						memcpy(pckt_info._msg, msg, strlen(msg));						
					}
				}

			}
		}
		if (pckt_size > 0) 
		{
				// TODO

				Log_info("received packet:");
				dumpData((uint8_t*)mqtt_packets, pckt_size);

// 				// Notice that there can be multiple MQTT packet
// 				pckt_ix = 0;
// 				while (pckt_ix < pckt_size) {
// 						switch ((mqtt_packets[pckt_ix] >> 4) & 0xF) {
// 						case CPT_PUBLISH:

// 								memset(&pckt_info, 0, sizeof(pckt_info));
// 								pckt_info._topic = topic;
// 								pckt_info._topic_size = sizeof(topic);
// 								pckt_info._msg = msg;
// 								pckt_info._msg_size = sizeof(msg);

// 								status = dissectPublishPacket(&mqtt_packets[pckt_ix], pckt_size - pckt_ix, pckt_info);
// 								if (status) {
// 										if (pckt_info._qos == 0) {
// 												// Nothing else to do
// 										} else if (pckt_info._qos == 1) {
// 												// TODO
// 												// Send PUBACK
// 										} else if (pckt_info._qos == 2) {
// 												// TODO
// 												// Send PUBREC
// 										} else {
// 												// Shouldn't happen
// 										}

// 										if (_publishHandler) {
// 												_publishHandler(topic, msg, pckt_info._msg_truncated_length);
// 										}
// 										pckt_ix += pckt_info._pckt_length;
// 								}
// 								else {
// 										// TODO
// 										// We don't know if we can trust the computed length
// 										// For now skip the rest
// 										pckt_ix = pckt_size;
// 								}
// 								break;
// 						default:
// 								if (_packetHandler) {
// 										_packetHandler(&mqtt_packets[pckt_ix], pckt_size - pckt_ix);
// 								}
// 								// TODO
// 								// We don't know the packet size. There can be multiple
// 								// For now skip the rest
// 								pckt_ix = pckt_size;
// 								break;
// 						}
// 				}
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