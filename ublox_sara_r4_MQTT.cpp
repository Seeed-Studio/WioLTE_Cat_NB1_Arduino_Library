#include <Arduino.h>
#include <ublox_sara_r4_mqtt.h>
#include <UART_Interface.h>

class MQTTPacketInfo
{
public:
    uint16_t _pckt_length;

    char *_topic;
    uint16_t _topic_size;
    uint16_t _topic_length;

    char *_msg;
    uint16_t _msg_size;
    uint16_t _msg_length;
    uint16_t _msg_truncated_length;

    bool _dup;
    uint8_t _qos;
    bool _retain;

    uint16_t _msg_id;
};

bool MQTT::setServerByDomain(char * server, uint16_t port)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	_server = server;
  _port = port;

	sprintf(sendBuf, "AT+UMQTT=2,\"%s\",%d", server, port);
	retVal = check_with_cmd(sendBuf, "+UMQTT: 2,1", CMD);
	return retVal;				
}

bool MQTT::setServerByIP(char * server, uint16_t port)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	sprintf(sendBuf, "AT+UMQTT=3,\"%s\",%d", server, port);
	retVal = check_with_cmd(sendBuf, "+UMQTT: 3,1", CMD);
	return retVal;				
}

bool MQTT::setAuth(char *userName, char *passwd)
{
	// AT+UMQTT=4,<username>, <password>
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	_clientName = userName;
	_password = passwd;

	sprintf(sendBuf, "AT+UMQTT=4,\"%s\",\"%s\"", userName, passwd);
	retVal = check_with_cmd(sendBuf, "+UMQTT: 4,1", CMD);
	return retVal;

}

bool MQTT::setInactiveTimeout(uint16_t timeout)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	sprintf(sendBuf, "AT+UMQTT=10,%d", timeout);
	retVal = check_with_cmd(sendBuf, "+UMQTT: 10,1", CMD);
	return retVal;
}


bool MQTT::clearSession(uint8_t clear)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	sprintf(sendBuf, "AT+UMQTT=12,%d", clear);
	retVal = check_with_cmd(sendBuf, "+UMQTT: 12,1", CMD);
	return retVal;
}

bool MQTT::setWillTopic(char *topic)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	sprintf(sendBuf, "AT+UMQTTWTOPIC=0,0,%d", topic);
	retVal = check_with_cmd(sendBuf, "+UMQTTWTOPIC: 1", CMD);
	return retVal;
}

bool MQTT::setWillMessage(char *message)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};

	sprintf(sendBuf, "AT+UMQTTWMSG=\"%s\"", message);
	retVal = check_with_cmd(sendBuf, "+UMQTTWMSG: 1", CMD);
	return retVal;
}


bool MQTT::set_umqttnv(uint8_t nvMode)
{

}

bool  MQTT::disconnect(void)
{
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTC=0", "+UMQTTC: 0,1", CMD);
	return retVal;
}

bool MQTT::connect(void)
{
	bool retVal = false;
	retVal = check_with_cmd("AT+UMQTTC=1", "+UMQTTC: 1,1", CMD);
	return retVal;
}

// bool MQTT::publish(const char *topic, const char *msg)
// {
// 	bool retVal = false;
// 	char txBuf[64] = {'\0'};

// 	sprintf(txBuf, "AT+UMQTTC=2,0,0,\"%s\",\"%s\"", topic, msg);
// 	retVal = check_with_cmd(txBuf, "+UMQTTC: 2,1", CMD);
// 	return retVal;
// }

bool MQTT::publish(const char *topic, const char *msg, uint8_t qos, uint8_t retain)
{
	bool retVal = false;
	char txBuf[64] = {'\0'};
	sprintf(txBuf, "AT+UMQTTC=2,%d,%d,\"%s\",\"%s\"", qos, retain, topic, msg);
	retVal = check_with_cmd(txBuf, "+UMQTTC: 2,1", CMD);
	return retVal;
}

bool MQTT::subscribe(char *topic, uint8_t qos)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};
	sprintf(sendBuf, "AT+UMQTTC=4,%d,\"%s\"", qos, topic);
	retVal = check_with_cmd(sendBuf, "+UMQTTC: 4,1", CMD);
	return retVal;
}

bool MQTT::unSubscribe(char *topic)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};
	sprintf(sendBuf, "AT+UMQTTC=5,\"%s\"", topic);
	retVal = check_with_cmd(sendBuf, "+UMQTTC: 5,1", CMD);
	return retVal;
}

bool MQTT::ping(uint8_t * server)
{
	bool retVal = false;
	char sendBuf[64] = {'\0'};
	sprintf(sendBuf, "AT+UMQTTC=8,\"%s\"", server);
	retVal = check_with_cmd(sendBuf, "+UMQTTC: 8,1", CMD);
	return retVal;
}

bool MQTT::loop()
{
	// Is there a packet?
	bool status;
	uint16_t pckt_size;
	uint16_t pckt_ix;
	pckt_size = check_readable();
	if (pckt_size > 0) 
	{
		char mqtt_packets[256] = {'\0'};
		// char topic[128] = {'\0'};;
		// char msg[128] = {'\0'};
		MQTTPacketInfo pckt_info;

		pckt_size = read_buffer(mqtt_packets, sizeof(mqtt_packets));
		if (pckt_size > 0) 
		{
				// TODO

				Log_info("received packet:");
				dumpData(mqtt_packets, pckt_size);

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