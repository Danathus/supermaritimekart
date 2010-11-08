#ifndef PACKET_PROCESSOR__H
#define PACKET_PROCESSOR__H

////////////////////////////////////////////////////////////////////////////////

#include <map>

#include <NetCore/Address.h>

namespace net
{
   class PacketParser;
}

////////////////////////////////////////////////////////////////////////////////

namespace net {

   class NETCORE_EXPORT PacketProcessor
   {
   public:
      typedef int ProtocolID;

      void RegisterParser(ProtocolID protocolID, PacketParser* parser);
      void RemoveParser(ProtocolID protocolID); // note: does no de-allocations
      PacketParser* GetParser(ProtocolID protocolID);
      const PacketParser* GetParser(ProtocolID protocolID) const;

      bool ProcessPacket(const Address& sender, const unsigned char data[], size_t size) const;

   protected:
   private:
      typedef std::map<ProtocolID, PacketParser*> ProtocolToParserMap;
      ProtocolToParserMap mProtocolToParserMap;
   };

} // namespace net

////////////////////////////////////////////////////////////////////////////////

#endif // PACKET_PROCESSOR__H
