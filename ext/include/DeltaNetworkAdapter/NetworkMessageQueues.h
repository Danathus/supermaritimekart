#ifndef NETWORK_MESSAGE_QUEUES__H
#define NETWORK_MESSAGE_QUEUES__H

////////////////////////////////////////////////////////////////////////////////

#include <queue>
#include <vector>

#include <dtCore/refptr.h>
#include <dtGame/message.h>

#include <NetSetGo/NetCore/NodeID.h>

#include <DeltaNetworkAdapter/LibraryExport.h>

class GameMessageSerializer;

////////////////////////////////////////////////////////////////////////////////

class DELTANETWORKADAPTER_LIBRARY_EXPORT NetworkMessageQueues
{
public:
   NetworkMessageQueues(GameMessageSerializer& gameMessageSerializer);
   ~NetworkMessageQueues();

   void Update(const float deltaSimTime);

   // message queue handling
   size_t GetNumQueuedMessages(net::NodeID nodeID) const;
   void QueueMessage(net::NodeID nodeID, const dtGame::Message* message);

protected:
   // sendAnyway means, send a packet with no payload even if there's no payload to send
   void SendQueuedMessages(dtUtil::DataStream& stream, net::NodeID nodeID, bool sendAnyway = true);

private:
   typedef std::queue< dtCore::RefPtr<const dtGame::Message> > MessageQueue;
   std::vector<MessageQueue> mMessageQueues; // index by node ID, grow as needed

   GameMessageSerializer& mGameMessageSerializer;
};

////////////////////////////////////////////////////////////////////////////////

#endif // NETWORK_MESSAGE_QUEUES__H
