#pragma once
#include <Arduino.h>

#define UDP_TX_PACKET_MAX_SIZE 24
#define MAX_SOCK_NUM 7

class Ublox_sara_r4_udp : public Ublox_sara_r4 {
private:
  uint16_t _port; // local port to listen on
  uint32_t _remoteIP; // remote IP address for the incoming packet whilst it's being processed
  uint16_t _remotePort; // remote port for the incoming packet whilst it's being processed
  uint16_t _offset; // offset into the packet being sent

protected:
  uint8_t _sock;  // socket ID 
  uint16_t _remaining; // remaining bytes of incoming packet yet to be processed

public:
  Ublox_sara_r4_udp();  // Constructor
  virtual uint8_t begin(uint16_t);	// initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
  virtual uint8_t beginMulticast(IPAddress, uint16_t);	// initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
  virtual void stop();  // Finish with the UDP socket
  
  // Start building up a packet to send to the remote host specific in host and port
  // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
  virtual int beginPacket(const char *host, uint16_t port);
  // Finish off this packet and send it
  // Returns 1 if the packet was sent successfully, 0 if there was an error
  virtual int endPacket();
  // Write a single byte into the packet
  virtual void write(uint8_t);
  // Write size bytes from buffer into the packet
  virtual void write(const uint8_t *buffer, uint16_t size);

  // Start processing the next available incoming packet
  // Returns the size of the packet in bytes, or 0 if no packets are available
  virtual int parsePacket();
  // Number of bytes remaining in the current packet
  virtual int available();
  // Read a single byte from the current packet
  virtual int read();
  // Read up to len bytes from the current packet and place them into buffer
  // Returns the number of bytes read, or 0 if none are available
  virtual int read(unsigned char* buffer, uint16_t len);
  // Read up to len characters from the current packet and place them into buffer
  // Returns the number of characters read, or 0 if none are available
  virtual int read(char* buffer, uint16_t len) { return read((unsigned char*)buffer, len); };
  // Return the next byte from the current packet without moving on to the next byte
  virtual int peek();
  virtual void flush();	// Finish reading the current packet

  // Return the IP address of the host who sent the current incoming packet
  virtual IPAddress remoteIP() { return _remoteIP; };
  // Return the port of the host who sent the current incoming packet
  virtual uint16_t remotePort() { return _remotePort; };
};