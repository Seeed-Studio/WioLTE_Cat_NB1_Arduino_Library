#include <Arduino.h>
#include <ublox_sara_r4_udp.h>
#include <ublox_sara_r4.h>


/* Constructor */
Ublox_sara_r4_udp::Ublox_sara_r4_udp() {}

/* Start udp socket, listening at local port PORT */
uint8_t Ublox_sara_r4_udp::begin(uint16_t port) {   

    // Create sock
    uint8_t sock = createSocket(UDP, port);
    if(sock == -1) {
        return 0;
    }
    _sock = sock;
    _port = port;
    _remaining = 0;
    return 1;
}

void Ublox_sara_r4_udp::stop()
{
    // close sock
    sockClose(_sock);
}

int Ublox_sara_r4_udp::beginPacket(char *host, uint16_t port)
{
    if(-1 != _sock) {
        if(!sockConnect(_sock, host, port)) {
            return 0;
        }
    }
    
    return 1:
}

int Ublox_sara_r4_udp::endPacket()
{
  
}

void Ublox_sara_r4_udp::write(uint8_t byte)
{
  return socketWrite(byte);
}

void Ublox_sara_r4_udp::write(const uint8_t *buffer, uint16_t size)
{
  socketWrite(buffer);
}


int Ublox_sara_r4_udp::parsePacket()
{
  // discard any remaining bytes in the last packet
  while (_remaining) {
    // could this fail (loop endlessly) if _remaining > 0 and recv in read fails?
    // should only occur if recv fails after telling us the data is there, lets
    // hope the w5100 always behaves :)
    read();
  }

  if (recvAvailable(_sock) > 0)
  {
    //HACK - hand-parse the UDP packet using TCP recv method
    uint8_t tmpBuf[8];
    int ret =0; 
    //read 8 header bytes and get IP and port from it
    ret = recv(_sock,tmpBuf,8);
    if (ret > 0)
    {
      _remoteIP = tmpBuf;
      _remotePort = tmpBuf[4];
      _remotePort = (_remotePort << 8) + tmpBuf[5];
      _remaining = tmpBuf[6];
      _remaining = (_remaining << 8) + tmpBuf[7];

      // When we get here, any remaining bytes are the data
      ret = _remaining;
    }
    return ret;
  }
  // There aren't any packets available
  return 0;
}

int Ublox_sara_r4_udp::read()
{
  uint8_t byte;

  if ((_remaining > 0) && (recv(_sock, &byte, 1) > 0))
  {
    // We read things without any problems
    _remaining--;
    return byte;
  }

  // If we get here, there's no data available
  return -1;
}

int Ublox_sara_r4_udp::read(unsigned char* buffer, uint16_t len)
{

  if (_remaining > 0)
  {

    int got;

    if (_remaining <= len)
    {
      // data should fit in the buffer
      got = recv(_sock, buffer, _remaining);
    }
    else
    {
      // too much data for the buffer, 
      // grab as much as will fit
      got = recv(_sock, buffer, len);
    }

    if (got > 0)
    {
      _remaining -= got;
      return got;
    }

  }

  // If we get here, there's no data available or recv failed
  return -1;

}

int Ublox_sara_r4_udp::peek()
{
  uint8_t b;
  // Unlike recv, peek doesn't check to see if there's any data available, so we must.
  // If the user hasn't called parsePacket yet then return nothing otherwise they
  // may get the UDP header
  if (!_remaining)
    return -1;
  ::peek(_sock, &b);
  return b;
}

void Ublox_sara_r4_udp::flush()
{
  // TODO: we should wait for TX buffer to be emptied
}