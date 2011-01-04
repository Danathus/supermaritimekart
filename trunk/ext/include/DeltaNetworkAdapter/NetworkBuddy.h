#ifndef NETWORK_BUDDY__H
#define NETWORK_BUDDY__H

////////////////////////////////////////////////////////////////////////////////

#include <DeltaNetworkAdapter/LibraryExport.h>

#include <DeltaNetworkAdapter/MachineInfoList.h>
#include <DeltaNetworkAdapter/TransmissionProtocol.h>

#include <NetSetGo/NetCore/NodeID.h>

#include <dtCore/refptr.h>

#include <dtGame/gamemanager.h>
#include <dtGame/messagefactory.h>

#include <queue>

namespace net
{
   class Address;
};

namespace dtGame
{
   class MessageType;
   class Message;
}

class NetworkEngineComponent;

namespace DeltaNetworkAdapter
{
   class DELTANETWORKADAPTER_LIBRARY_EXPORT NetworkBuddy
   {
   public:
      static NetworkBuddy& GetRef();
      static void Destroy();

      bool IsServer() const;

      int GetMaxAvailablePacketSize() const;

      // todo: can we phase these out?
      void SetGameManager(dtGame::GameManager* gameManager) { mpGameManager = gameManager; }
      dtGame::GameManager* GetGameManager() { return mpGameManager; }
      const dtGame::GameManager* GetGameManager() const { return mpGameManager; }

      // todo: can we phase these out?
      void SetNetworkComponent(NetworkEngineComponent* netComponent) { mpNetComponent = netComponent; }
      NetworkEngineComponent* GetNetworkComponent() { return mpNetComponent; }
      const NetworkEngineComponent* GetNetworkComponent() const { return mpNetComponent; }

      const dtGame::MachineInfo* GetMachineInfo(net::NodeID nodeID) const;
      const dtGame::MachineInfo* GetMachineInfo(const std::string& uniqueID) const;
      net::NodeID GetNodeID(const dtGame::MachineInfo* machineInfo) const;

      const MachineInfoList& GetConnectedMachineList() const;

      template <typename MessageClassName>
      void CreateMessage(const dtGame::MessageType& msgType, dtCore::RefPtr<MessageClassName>& result)
      {
         GetGameManager()->GetMessageFactory().CreateMessage(msgType, result);
      }

      /**
       * @param destination For this one put 0 for matchmaker/server
       * @return True if dispatched successfully. Note: does not guarantee success of reception.
       */
      bool SendNetworkMessage(dtGame::Message& message, net::NodeID destination, TransmissionProtocol protocol = TRANSPORT_DEFAULT);
      /**
       */
      bool SendNetworkMessage(dtGame::Message& message, const dtGame::MachineInfo& destination, TransmissionProtocol protocol = TRANSPORT_DEFAULT);

      // note: to-date, multi-casting only supports "TRANSPORT_FAST_AND_LOOSE" transfer
      bool MulticastNetworkMessage(const net::Address& address, const dtGame::Message& message);

      // todo: can we get rid of these calls?
      bool SerializeMessage(dtUtil::DataStream& stream, const dtGame::Message* message, size_t maxPayloadSize) const;
      dtCore::RefPtr<dtGame::Message> DeserializeNetworkMessage(net::NodeID senderNodeID, dtUtil::DataStream& stream);

      //void TagMessageForLogging(const dtGame::Message& message);

      // for wrapping things up
      void StartShutdownProcess();
      bool IsShutdownProcessStarted() const { return mIsShuttingDown; }
      bool IsShutdownProcessFinished() const;
      void WaitForShutdownProcessToFinish();

   protected:
      NetworkBuddy();
      ~NetworkBuddy();

   private:
      static NetworkBuddy* mpSelf;
      // todo: can we phase these out?
      dtGame::GameManager* mpGameManager;
      NetworkEngineComponent* mpNetComponent;

      bool mIsShuttingDown;
   };
} // namespace DeltaNetworkAdapter

////////////////////////////////////////////////////////////////////////////////

#endif // NETWORK_BUDDY__H
