#include <SMK_NetworkComponent.h>

#include <DeltaNetworkAdapter/NetworkMessages.h>
#include <DeltaNetworkAdapter/machineinfomessage.h>
#include <NetworkBuddy.h>
#include <messages/NetworkMessages.h>
//#include <CCTS_ActorLibrary/IdentityAssignmentManager.h>

#include <dtGame/basemessages.h>
#include <dtUtil/log.h>

////////////////////////////////////////////////////////////////////////////////

SMK_NetworkComponent::SMK_NetworkComponent()
   : NetworkEngineComponent()
{
   //
}

////////////////////////////////////////////////////////////////////////////////

SMK_NetworkComponent::~SMK_NetworkComponent()
{
   //
}

void SMK_NetworkComponent::OnAddedToGM()
{
   NetworkEngineComponent::OnAddedToGM();

   NetworkBuddy::GetRef().SetGameManager(GetGameManager());
   NetworkBuddy::GetRef().SetNetworkComponent(this);
}

void SMK_NetworkComponent::OnRemovedFromGM()
{
   NetworkBuddy::GetRef().StartShutdownProcess();
   NetworkBuddy::GetRef().WaitForShutdownProcessToFinish();

   NetworkEngineComponent::OnRemovedFromGM();
}

void SMK_NetworkComponent::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_CLIENT_CONNECTED)
   {
      const DeltaNetworkAdapter::MachineInfoMessage& machineInfoMessage = static_cast<const DeltaNetworkAdapter::MachineInfoMessage&>(message);

      // if we're the server, we need to tell them what map to load
      if (NetworkBuddy::GetRef().IsServer() &&
         machineInfoMessage.GetUniqueId() != GetGameManager()->GetMachineInfo().GetUniqueId().ToString())
      {
         dtCore::RefPtr<dtGame::MapMessage> mapMessage;
         GetGameManager()->GetMessageFactory().CreateMessage(dtGame::MessageType::COMMAND_LOAD_MAP, mapMessage);
         {
            std::vector<std::string> nameVec;
            nameVec.push_back(GetGameManager()->GetCurrentMap());
            mapMessage->SetMapNames(nameVec);
         }
         mapMessage->SetSource(GetGameManager()->GetMachineInfo());
         mapMessage->SetDestination(NetworkBuddy::GetRef().GetMachineInfo(machineInfoMessage.GetUniqueId()));
         SendNetworkMessage(*mapMessage);
      }
   }
   else if (message.GetMessageType() == dtGame::MessageType::COMMAND_LOAD_MAP)
   {
      const dtGame::MapMessage& mapMessage = static_cast<const dtGame::MapMessage&>(message);
      std::vector<std::string> nameVec;
      mapMessage.GetMapNames(nameVec);
      if (nameVec.size() > 0)
      {
         const std::string& mapName = nameVec[0];
         LOG_INFO("Loading Map: " + mapName);
         GetGameManager()->ChangeMap(mapName);
      }
   }
   else if (message.GetMessageType() == SMK::SMKNetworkMessages::INFO_CLIENT_MAP_LOADED)
   {
      //now send all pertinent game data to the newly connected client
      SendGameDataToClient(&message.GetSource());
   }
   

   // in all cases, we must respect our elder
   NetworkEngineComponent::ProcessMessage(message);
}

void SMK_NetworkComponent::QueueMessage(net::NodeID nodeID, const dtGame::Message* message)
{
#if 0
   // if we're not already sending to the logger, forward this to the logger as well!
   {
      const dtGame::MachineInfo* loggerMachineInfo = CCTS::IdentityAssignmentManager::GetRef().GetMachineInfo(CCTS::IdentityAssignmentManager::GetRef().GetLoggerRole());

      if (loggerMachineInfo != NULL && CCTS::NetworkBuddy::GetRef().GetNodeID(loggerMachineInfo) != nodeID)
      {
         NetworkBuddy::GetRef().TagMessageForLogging(*message);
      }
   }
#endif

   NetworkEngineComponent::QueueMessage(nodeID, message);
}

////////////////////////////////////////////////////////////////////////////////
void SMK_NetworkComponent::AddToNewClientPublishList(dtDAL::BaseActorObject& actorProxy)
{
   mProxiesToSendToNewClients.push_back(&actorProxy);
}

////////////////////////////////////////////////////////////////////////////////
void SMK_NetworkComponent::ClearNewClientPublishList()
{
   mProxiesToSendToNewClients.clear();
}

////////////////////////////////////////////////////////////////////////////////
void SMK_NetworkComponent::SendGameDataToClient(const dtGame::MachineInfo* machineInfo)
{
   LOG_DEBUG("Sending game data to newly connected client");

   ProxyContainer::const_iterator proxyItr = mProxiesToSendToNewClients.begin();
   while (proxyItr != mProxiesToSendToNewClients.end())
   {
      if ((*proxyItr)->IsGameActorProxy())
      {
         dtGame::GameActorProxy* gap = static_cast<dtGame::GameActorProxy*>((*proxyItr).get());
         gap->NotifyFullActorUpdate(); //creates a msg and sends all Properties in it.
      }

      ++proxyItr;
   }
}

////////////////////////////////////////////////////////////////////////////////
