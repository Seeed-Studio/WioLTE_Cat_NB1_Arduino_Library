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
    bool setAuth(char * userName, char * passwd);
		bool setInactiveTimeout(uint16_t timeout);
		bool setSecureOpt(uint8_t secure, uint8_t use_profile);
		bool clearSession(uint8_t clear);
    
		// will
		bool setWillTopic(char *topic);
		bool setWillMessage(char *message);

		// NVM
		bool set_umqttnv(uint8_t nvMode);
	
		// MQTT ctrl command
    bool connect(uint16_t timeout_sec = 5);
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
    // bool connect();
    // bool disconnect();
    // size_t assemblePublishPacket(char * pckt, size_t size,
    //         char * topic, const char * msg, size_t msg_len, char qos = 0, char retain = 1);
    // size_t assembleSubscribePacket(char * pckt, size_t size,
    //         char * topic, char qos = 0);
    // size_t assembleConnectPacket(char * pckt, size_t size, uint16_t keepAlive);
    // //size_t assembleDisconnectPacket(char * pckt, size_t size);
    // size_t assemblePingreqPacket(char * pckt, size_t size);
    // bool dissectPublishPacket(const char * pckt, size_t len, MQTTPacketInfo &pckt_info);

    // void newPacketIdentifier();

    // uint32_t getRemainingLength(const char *buf, size_t & nrBytes);
    // uint16_t get_uint16_be(const char *buf);

  //   enum ControlPacketType_e {
  //       CPT_CONNECT = 1,
  //       CPT_CONNACK = 2,
  //       CPT_PUBLISH = 3,
  //       CPT_PUBACK = 4,
  //       CPT_PUBREC = 5,
  //       CPT_PUBREL = 6,
  //       CPT_PUBCOMP = 7,
	// CPT_SUBSCRIBE = 8,
  //       CPT_SUBACK = 9,
  //       CPT_UNSUBSCRIBE = 10,
  //       CPT_UNSUBACK = 11,
  //       CPT_PINGREQ = 12,
  //       CPT_PINGRESP = 13,
  //       CPT_DISCONNECT = 14,
  //   };
  //   enum State_e {
  //       ST_UNKNOWN,
  //       ST_TCP_OPEN,
  //       ST_MQTT_CONNECTED,
  //       ST_MQTT_DISCONNECTED,
  //       ST_TCP_CLOSED,
  //   };
    // enum State_e _state;
    // Sodaq_GSM_Modem * _transport;
    char * _server;
    uint16_t _port;
    char * _clientName;
    char * _password;
    // char * _clientId;
    // uint16_t _packetIdentifier;
    // void (*_publishHandler)(char *topic, const char *msg, size_t msg_length);
    // void (*_packetHandler)(char *pckt, size_t len);
    // uint16_t _keepAlive;

    // The (optional) stream to show debug information.
    // Stream* _diagStream;

    // void diagDumpBuffer(const char * buf, size_t len);
};

/*!
 * \brief This is the default (the only?) instance of the MQTT class
 */
// extern MQTT mqtt;

