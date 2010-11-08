#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include <list>

#include <NetCore/NetCoreExport.h>

namespace net {

////////////////////////////////////////////////////////////////////////////////

// packet queue to store information about sent and received packets sorted in sequence order
//  + we define ordering using the "sequence_more_recent" function, this works provided there is a large gap when sequence wrap occurs

struct NETCORE_EXPORT PacketData
{
   unsigned int mSequence; // packet sequence number
   float mTime;            // time offset since packet was sent or received (depending on context)
   int mSize;              // packet size in bytes

   bool operator==(const PacketData& packetData) const;
   bool operator<(const PacketData& packetData) const;
};

class NETCORE_EXPORT PacketQueue : public std::list<PacketData>
{
public:
   bool exists(unsigned int sequence);
   void insert_sorted(const PacketData& p, unsigned int max_sequence);
   void verify_sorted(unsigned int max_sequence);
};

////////////////////////////////////////////////////////////////////////////////

} // namespace net

#endif // PACKETQUEUE_H
