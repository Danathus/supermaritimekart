#ifndef GUARANTEED_DELIVERY_SYSTEM__H
#define GUARANTEED_DELIVERY_SYSTEM__H

////////////////////////////////////////////////////////////////////////////////

#include <map>

#include <NetCore/NetCoreExport.h>

#include <NetCore/ReliabilitySystem.h>

namespace dtUtil
{
   class Log;
}
////////////////////////////////////////////////////////////////////////////////

class NETCORE_EXPORT GuaranteedDeliverySystem
{
public:
   GuaranteedDeliverySystem(const net::ReliabilitySystem& reliabilitySystem);

   void Reset();
   static size_t GetHeaderSize() { return GuaranteedPacket::GetHeaderSize(); }

   size_t GetPendingSendQueueSize() const { return mPendingSendQueue.size(); }
   size_t GetPendingRecvQueueSize() const { return mPendingRecvQueue.size(); }

   // these are the main entry points for the user(s) of the system ////////////
   void QueueOutgoingPacket(const char* packet, size_t length);
   // note: if packet is passed in as NULL or length is passed in as 0
   //   then malloc() will be called to create new space for the packet
   bool DequeueReceivedPacket(int nodeID, char*& packet, size_t& length);
   // also note: the nodeID is only being passed in for debugging purposes

   // these are called when it's time to literally transmit or receive a packet
   //   note: currently make sure this is called before reliability system's sequence number is incremented
   size_t SerializePacket(char* packet, size_t maxLength);
   size_t DeserializePacket(const char* packet, size_t length);

   // these are the main update pumps for basic operation //////////////////////

   // note: don't let the corresponding Reliability System update twice
   //   between sequential Guaranteed Delivery System updates
   void Update();

protected:
   struct GuaranteedPacket
   {
      unsigned int mGuaranteedSequence;
      const char* mData;
      size_t mLength;

      size_t Serialize(char* packet) const;
      size_t Deserialize(const char* packet, size_t packetLength);
      static size_t GetHeaderSize();
      size_t GetSize() const;
   };
   struct IssuedGuaranteedPacket
   {
      GuaranteedPacket guaranteedPacket;

      // at this point it has also been assigned a reliability sequence number
      unsigned int mReliabilitySequence;
   };

   std::list<IssuedGuaranteedPacket>::iterator FindPendingAckPacket(unsigned int reliabilitySequence);
   bool FindInPacketList(const std::list<GuaranteedPacket>& packetList, const GuaranteedPacket& packet) const;
   void InsertSorted(std::list<GuaranteedPacket>& packetList, const GuaranteedPacket& packet);

private:
   const net::ReliabilitySystem& mReliabilitySystem;
   unsigned int mLocalGuaranteedSequenceNumber;  // local sequence number for most recently sent packet
   unsigned int mRemoteGuaranteedSequenceNumber; // remote sequence number for next received packet to be dequeued

   // for sending
   std::list<GuaranteedPacket>       mPendingSendQueue;
   std::list<IssuedGuaranteedPacket> mPendingAckQueue;

   // for receiving
   std::list<GuaranteedPacket>       mPendingRecvQueue;

   // for debug reporting
   bool mGuaranteedNumberMismatchReported;

   dtUtil::Log* mLogger;
};

////////////////////////////////////////////////////////////////////////////////

#endif // GUARANTEED_DELIVERY_SYSTEM__H
