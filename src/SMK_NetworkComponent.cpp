#include <SMK_NetworkComponent.h>
#include <network/NetworkBuddy.h>

#include <DeltaNetworkAdapter/NetworkMessages.h>
#include <DeltaNetworkAdapter/machineinfomessage.h>
#include <messages/NetworkMessages.h>
//#include <CCTS_ActorLibrary/IdentityAssignmentManager.h>
#include <actors/PickUpItemHandle.h>

#include <dtGame/basemessages.h>
#include <dtUtil/log.h>

static const double SECONDS_BEFORE_SENDING_GAME_STATE = 1.0;

////////////////////////////////////////////////////////////////////////////////

SMK_NetworkComponent::SMK_NetworkComponent()
   : NetworkEngineComponent()
   , mTimeLeftBeforeUpdatingClients(SECONDS_BEFORE_SENDING_GAME_STATE)
   , mNeedToUpdateClients(false)
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
   if (NetworkBuddy::GetRef().IsServer())
   {
      if (message.GetMessageType() == dtGame::MessageType::INFO_CLIENT_CONNECTED)
      {
         const DeltaNetworkAdapter::MachineInfoMessage& machineInfoMessage = static_cast<const DeltaNetworkAdapter::MachineInfoMessage&>(message);
         mNeedToUpdateClients = true;  
         mTimeLeftBeforeUpdatingClients = SECONDS_BEFORE_SENDING_GAME_STATE;
      }
      else if (message.GetMessageType() == SMK::SMKNetworkMessages::REQUEST_PICKUP_PICKUP)
      {      
         HandleThePickUpRequest(message);
      }
      else if (message.GetMessageType() == dtGame::MessageType::TICK_LOCAL)
      {
         if (mNeedToUpdateClients)
         {
           const dtGame::TickMessage& tickMsg = static_cast<const dtGame::TickMessage&>(message);

            mTimeLeftBeforeUpdatingClients -= tickMsg.GetDeltaRealTime();
            if (mTimeLeftBeforeUpdatingClients <= 0.0)
            {
               //now send all pertinent game data to the newly connected client
               //Their map should have been loaded by this point.
               SendGameDataToClient();
               mNeedToUpdateClients = false;
            }
         }         
      }
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
void SMK_NetworkComponent::SendGameDataToClient()
{   
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
void SMK_NetworkComponent::HandleThePickUpRequest(const dtGame::Message& message)
{
   const dtCore::UniqueId& pickupUID = message.GetAboutActorId();

   SMK::PickUpItemBaseProxy* pickupProxy(NULL);

   GetGameManager()->FindActorById(pickupUID, pickupProxy);
   if (pickupProxy)
   {
      SMK::PickUpItemHandle* pickup(NULL);
      pickupProxy->GetActor(pickup);

      if (pickup->GetActive() == true)
      {
         //send a msg to the player who got this pickup.
         dtCore::RefPtr<dtGame::MachineInfoMessage> msg;
         GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::INFO_PICKUP_ITEM_ACQUIRED, msg);
         msg->SetAboutActorId(message.GetSendingActorId());
         msg->SetUniqueId(pickupUID.ToString());
         GetGameManager()->SendNetworkMessage(*msg);
         GetGameManager()->SendMessage(*msg);

         //Looks like a valid pickup attempt.  Mark it inactive and tell the world
         pickup->SetActive(false);
         pickup->SetCollisionDetection(false);

         //tell the world this pickup is now inactive
         std::vector<dtUtil::RefString> propsToSend;
         propsToSend.push_back("IsActive"); //DeltaDrawable
         propsToSend.push_back(dtDAL::TransformableActorProxy::PROPERTY_ENABLE_COLLISION);
         pickupProxy->NotifyPartialActorUpdate(propsToSend);
      }      
   }   
}

////////////////////////////////////////////////////////////////////////////////
