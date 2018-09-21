#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/*!
 * \brief The maximum length of a packet
 */
#define MQTT_MAX_PACKET_LENGTH  100
#define MQTT_DEFAULT_KEEP_ALIVE  60


class MQTTPacketInfo;

class MQTT
{
public:
    MQTT(){};
    ~MQTT(){};
    bool setServer(char * server, uint16_t port = 1883);
    bool setClientId(char *clientId);
    bool setAuth(char * userName, char * passwd);
		bool setInactiveTimeout(uint16_t timeout);
		bool setSecureOpt(uint8_t secure, uint8_t use_profile);
		bool clearSession(uint8_t clear);
    
		// will
    bool setWill(char *topic, char *msg, uint8_t qos = 0, uint8_t retain = 0);
    bool clearWill();
		// bool setWillTopic(char *topic);
		// bool setWillMessage(char *message);

		// NVM
		bool saveProfile();
    bool restoreProfile();
    bool deleteProfile();
	
		// MQTT ctrl command
    bool connect(uint16_t timeout_sec = 10u);
		bool disconnect(void);	
    bool publish(const char *topic, const char *msg, uint8_t qos = 0, uint8_t retain = 0);
    // bool publish(const  char *topic, const char *msg);
    bool subscribe(char * topic, uint8_t qos = 0);
		bool unSubscribe(char * topic);
		void readMsg(char *message);
    bool ping(char * server);
    void setPublishHandler(void (*handler)(char *topic, const char *msg, size_t msg_length));
    void setPacketHandler(void (*handler)(char *pckt, size_t len));
    bool loop(void);

    // void setTransport(Sodaq_GSM_Modem * transport);
    // void setKeepAlive(uint16_t x) { _keepAlive = x; }

   
    
    
    // bool availablePacket();
    // bool open();
    // void close(bool switchOff=true);
    // bool isConnected();
    // void setStateClosed();

    // Sets the optional "Diagnostics and Debug" stream.
    // void setDiag(Stream &stream) { _diagStream = &stream; }
    // void setDiag(Stream *stream) { _diagStream = stream; }

private:
    bool isIPAddress(char *server);
    void (*_publishHandler)(const char *topic, const uint8_t *msg, size_t msg_length);
    void (*_packetHandler)(uint8_t *pckt, size_t len);
    uint16_t _keepAlive;
    
    char * _server;
    uint16_t _port;
    char * _clientName;
    char * _password;
    char * _clientId;
};

