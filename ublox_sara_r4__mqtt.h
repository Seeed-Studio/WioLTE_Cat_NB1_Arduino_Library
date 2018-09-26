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

