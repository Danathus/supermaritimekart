#ifndef PACKET_PARSER__H
#define PACKET_PARSER__H

#include <NetCore/Address.h>

namespace net {

////////////////////////////////////////////////////////////////////////////////

class PacketParser
{
public:
   virtual ~PacketParser(){}

   virtual bool ParsePacket(const Address& sender, const unsigned char data[], size_t size) const = 0;

protected:
private:
};

////////////////////////////////////////////////////////////////////////////////

} // namespace net

#endif // PACKET_PARSER__H
