// this class serves as an adapter between Delta and the new NetCore system.
// It wraps the NetworkEngine and provides an easy way to send and receive
// dtGame::Messages.

#ifndef NETWORK_ENGINE_COMPONENT__H
#define NETWORK_ENGINE_COMPONENT__H

////////////////////////////////////////////////////////////////////////////////

#include <dtGame/gmcomponent.h>

#include <DeltaNetworkAdapter/LibraryExport.h>
#include <DeltaNetworkAdapter/GameMessageSerializer.h>
#include <DeltaNetworkAdapter/MachineInfoList.h>
#include <DeltaNetworkAdapter/NetworkMessageQueues.h>

////////////////////////////////////////////////////////////////////////////////

namespace DeltaNetworkAdapter
{
   class MachineInfoMessage;
}

class DELTANETWORKADAPTER_LIBRARY_EXPORT NetworkEngineComponent : public dtGame::GMComponent
{
public:
   static const float kDefaultNetworkTimeoutDuration;

   enum DestinationType
   {
      DESTINATION,
      ALL_CLIENTS,
      ALL_NOT_CLIENTS
   };

   NetworkEngineComponent();

   void SendNetworkMessage(const dtGame::Message& message, const DestinationType destinationType = DESTINATION);

   // this is needed to check if we're all finished shutting off the network pipe
   bool AllOutgoingQueuesEmpty() const;

   /**
    * Retrieves a dtGame::MachineInfo* from the stored connections
    * If no matching connection is found, NULL is returned
    * @param dtCore::UniqueId of the MachineInfo
    * @return Pointer to the dtGame::MachineInfo* or NULL
    */
   const dtGame::MachineInfo* GetMachineInfo(const std::string& uniqueId) const;
   const dtGame::MachineInfo* GetMachineInfo(net::NodeID nodeID) const;
   net::NodeID GetNodeID(const dtGame::MachineInfo* machineInfo) const;

   const MachineInfoList& GetConnectedMachineList() const { return mConnectedMachines; }

   // todo: can we make these non-public?
   bool SerializeMessage(dtUtil::DataStream& stream, const dtGame::Message* message, size_t maxPayloadSize) const;
   dtCore::RefPtr<dtGame::Message> DeserializeNetworkMessage(net::NodeID senderNodeID, dtUtil::DataStream& stream);

   // implementations of virtual methods
   void OnAddedToGM();
   void OnRemovedFromGM();
   void ProcessMessage(const dtGame::Message& message);
   void DispatchNetworkMessage(const dtGame::Message& message);

protected:
   ~NetworkEngineComponent();

   virtual void QueueMessage(net::NodeID nodeID, const dtGame::Message* message);

private:
   void OnTickLocal(const float deltaSimTime);
   void OnClientConnected(const DeltaNetworkAdapter::MachineInfoMessage& machineInfoMsg);

   void UpdateLocalClientMachineInfo(const DeltaNetworkAdapter::MachineInfoMessage& machineInfoMsg);
   void ServerForwardClientConnection(const DeltaNetworkAdapter::MachineInfoMessage& machineInfoMsg);

   void UpdateMachineInfoList();
   void UpdateNetworkMessages(const float deltaSimTime);
   void ForwardReceivedNetworkMessage(net::NodeID senderNodeID, dtGame::Message* message);

   GameMessageSerializer mGameMessageSerializer;
   NetworkMessageQueues mNetworkMessageQueues;

   // a vector containing information of all connected machines
   MachineInfoList mConnectedMachines;
   bool mSentMachineInfo;
};

////////////////////////////////////////////////////////////////////////////////

#endif // NETWORK_ENGINE_COMPONENT__H
