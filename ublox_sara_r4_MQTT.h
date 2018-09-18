#pragma once

#include <stddef.h>
#include <stdint.h>

/*!
 * \brief The maximum length of a packet
 */
#define MQTT_MAX_PACKET_LENGTH  100
#define MQTT_DEFAULT_KEEP_ALIVE  60

class MQTTPacketInfo;

class MQTT
{
public:
    MQTT();
    bool setServerByDomain(uint8_t * server, uint16_t port = 1883);
		bool setServerByIP(uint8_t * server, uint16_t port = 1883);
    bool setAuth(uint8_t * userName, uint8_t * passwd);
		bool setInactiveTimeout(uint16_t timeout);
		bool setSecureOpt(uint8_t secure, uint8_t use_profile);
		bool clearSession(uint8_t clear);
    
		// will
		bool setWillTopic(uint8_t *topic);
		bool setWillMessage(uint8_t *message);

		// NVM
		bool set_umqttnv(uint8_t nvMode);
	
		// MQTT ctrl command
		bool connect(void);
		bool disconnect(void);	
    bool publish(uint8_t * topic, uint8_t * msg, uint8_t qos = 0, uint8_t retain = 0);
    bool publish(uint8_t *topic, uint8_t *message);
    bool subscribe(uint8_t * topic, uint8_t qos = 0);
		bool unSubscribe(uint8_t * topic);
		void readMsg(uint8_t *message);

		bool ping(uint8_t * server);
		bool loop(void);

    // void setTransport(Sodaq_GSM_Modem * transport);
    // void setKeepAlive(uint16_t x) { _keepAlive = x; }

   
    // bool ping();
    // void setPublishHandler(void (*handler)(uint8_t *topic, const uint8_t *msg, size_t msg_length));
    // void setPacketHandler(void (*handler)(uint8_t *pckt, size_t len));
    
    // bool availablePacket();
    // bool open();
    // void close(bool switchOff=true);
    // bool isConnected();
    // void setStateClosed();

    // Sets the optional "Diagnostics and Debug" stream.
    // void setDiag(Stream &stream) { _diagStream = &stream; }
    // void setDiag(Stream *stream) { _diagStream = stream; }

private:
    // bool connect();
    // bool disconnect();
    // size_t assemblePublishPacket(uint8_t * pckt, size_t size,
    //         uint8_t * topic, const uint8_t * msg, size_t msg_len, uint8_t qos = 0, uint8_t retain = 1);
    // size_t assembleSubscribePacket(uint8_t * pckt, size_t size,
    //         uint8_t * topic, uint8_t qos = 0);
    // size_t assembleConnectPacket(uint8_t * pckt, size_t size, uint16_t keepAlive);
    // //size_t assembleDisconnectPacket(uint8_t * pckt, size_t size);
    // size_t assemblePingreqPacket(uint8_t * pckt, size_t size);
    // bool dissectPublishPacket(const uint8_t * pckt, size_t len, MQTTPacketInfo &pckt_info);

    // void newPacketIdentifier();

    // uint32_t getRemainingLength(const uint8_t *buf, size_t & nrBytes);
    // uint16_t get_uint16_be(const uint8_t *buf);

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
    uint8_t * _server;
    uint16_t _port;
    uint8_t * _clientName;
    uint8_t * _password;
    // char * _clientId;
    // uint16_t _packetIdentifier;
    // void (*_publishHandler)(uint8_t *topic, const uint8_t *msg, size_t msg_length);
    // void (*_packetHandler)(uint8_t *pckt, size_t len);
    // uint16_t _keepAlive;

    // The (optional) stream to show debug information.
    // Stream* _diagStream;

    // void diagDumpBuffer(const uint8_t * buf, size_t len);
};

/*!
 * \brief This is the default (the only?) instance of the MQTT class
 */
extern MQTT mqtt;

