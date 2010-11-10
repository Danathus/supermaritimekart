#include <ScenarioComponent.h>

//#include <CCTS_ActorLibrary/ActorRegistry.h>
//#include <CCTS_ActorLibrary/DroppableInventoryItem.h>
//#include <CCTS_ActorLibrary/EnterableActor.h>
//#include <CCTS_ActorLibrary/EnterableActorProxy.h>
//#include <CCTS_ActorLibrary/IdentityAssignmentManager.h>
#include <NetworkBuddy.h>
//#include <CCTS_ActorLibrary/PickupableActor.h>
//#include <CCTS_ActorLibrary/PickupableActorProxy.h>
//#include <CCTS_ActorLibrary/RoleActor.h>
//#include <CCTS_ActorLibrary/RoleActorProxy.h>
#include <DeltaNetworkAdapter/NetworkMessages.h>
//#include <CCTS_Util/RoleMessage.h>
//#include <CCTS_Util/ActorUpdateMessage.h>
#include <DeltaNetworkAdapter/NetworkedUniqueIDMessage.h>
//#include <CCTS_Util/EnterableInventoryUpdateMessage.h>

#include <NetCore/NetworkEngine.h>

#include <SharedActors/ActorLibraryMessages.h>

#include <dtCore/system.h>
#include <dtGame/actorupdatemessage.h>
#include <dtGame/basemessages.h>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////

ScenarioComponent::ScenarioComponent()
   : dtGame::GMComponent("ScenarioComponent")
{
}

void ScenarioComponent::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::TICK_LOCAL)
   {
      const dtGame::TickMessage& tickMsg = static_cast<const dtGame::TickMessage&>(message);
      OnTickLocal(tickMsg.GetDeltaSimTime());
   }
   else if (message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::INFO_JOINED_SERVER)
   {
      const DeltaNetworkAdapter::NetworkedUniqueIDMessage& networkedIDMessage = static_cast<const DeltaNetworkAdapter::NetworkedUniqueIDMessage&>(message);
      const dtGame::MachineInfo* machineInfo = NetworkBuddy::GetRef().GetMachineInfo(networkedIDMessage.GetUniqueId());
      OnPlayerJoinedServer(machineInfo);
   }
   else if (message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::INFO_LEFT_SERVER)
   {
      const DeltaNetworkAdapter::NetworkedUniqueIDMessage& networkedIDMessage = static_cast<const DeltaNetworkAdapter::NetworkedUniqueIDMessage&>(message);
      const dtGame::MachineInfo* machineInfo = NetworkBuddy::GetRef().GetMachineInfo(networkedIDMessage.GetUniqueId());
      OnPlayerLeftServer(machineInfo);
   }
#if 0
   else if (message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::INFO_ROLE_REQUESTED)
   {
      if (NetworkBuddy::GetRef().IsServer())
      {
         const CCTS::RoleMessage& roleMessage = static_cast<const CCTS::RoleMessage&>(message);
         const dtGame::MachineInfo* machineInfo = NetworkBuddy::GetRef().GetMachineInfo(roleMessage.GetUniqueId());
         OnPlayerRequestedRole(machineInfo, roleMessage.GetRole(), roleMessage.GetRoleIdentity());
      }
   }
#endif
   else if (message.GetMessageType() == dtGame::MessageType::INFO_ACTOR_UPDATED)
   {
      const dtGame::ActorUpdateMessage& creationMsg = static_cast<const dtGame::ActorUpdateMessage&>(message);
      //OnPlayerChoseRole(creationMsg.GetAboutActorId(), creationMsg.GetSource());
   }
#if 0
   else if (message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::ACTION_ADD_INVENTORY_TO_ENTERABLE)
   {
      const CCTS::EnterableInventoryUpdateMessage& invMessage = static_cast<const CCTS::EnterableInventoryUpdateMessage&>(message);
      const dtGame::MachineInfo* machineInfo = NetworkBuddy::GetRef().GetMachineInfo(invMessage.GetUniqueId());
      OnEnterableActorAddInventory(machineInfo, invMessage.GetEnterableUniqueId(), invMessage.GetInventoryGroupIndex(), invMessage.GetDroppableCount(), invMessage.GetInventoryItemType());
   }
   else if (message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::ACTION_REMOVE_INVENTORY_FROM_ENTERABLE)
   {
      const CCTS::EnterableInventoryUpdateMessage& invMessage = static_cast<const CCTS::EnterableInventoryUpdateMessage&>(message);
      const dtGame::MachineInfo* machineInfo = NetworkBuddy::GetRef().GetMachineInfo(invMessage.GetUniqueId());
      OnEnterableActorRemoveInventory(machineInfo, invMessage.GetEnterableUniqueId(), invMessage.GetInventoryGroupIndex(), invMessage.GetDroppableCount(), invMessage.GetInventoryItemType());
   }
   else if (message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::ACTION_REMOVE_INVENTORY_ORIGIN)
   {
      if (CCTS::NetworkBuddy::GetRef().IsServer())
      {
         const ConfigurationMessage<std::string>& originMessage = static_cast<const ConfigurationMessage<std::string>&>(message);
         ServerRemoveInventoryFromOriginMap(&originMessage.GetSource(), originMessage.GetConfigValue());
      }
   }
#endif
   else if (message.GetMessageType() == dtGame::MessageType::INFO_TIME_CHANGED)
   {
      const dtGame::TimeChangeMessage& timeChangeMessage = static_cast<const dtGame::TimeChangeMessage&>(message);
      //
      printf("received time change message:\n");
      printf("\tsimtime: %g\n", timeChangeMessage.GetSimulationTime());
      printf("\tsimclocktime: %g\n", timeChangeMessage.GetSimulationClockTime());
      //
      // should there be a default message processor that handles this?
      dtCore::System::GetInstance().SetSimulationTime(timeChangeMessage.GetSimulationTime());
      dtCore::System::GetInstance().SetSimulationClockTime(timeChangeMessage.GetSimulationClockTime());
   }
#if 0
   else if(message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::ACTION_CCTS_WARP_ACTOR)
   {
      const CCTS::ActorUpdateMessage& updateMessage = static_cast<const CCTS::ActorUpdateMessage&>(message);
      OnWarpActor(updateMessage.GetUniqueId(), updateMessage.GetTranslation(), updateMessage.GetRotation());
   }
   else if(message.GetMessageType() == DeltaNetworkAdapter::NetworkMessageType::ACTION_EXTINGUISH_FIRE)
   {
      const ConfigurationMessage<std::string>& extinguishMessage = static_cast<const ConfigurationMessage<std::string>&>(message);
      OnExtinguishFire(extinguishMessage.GetConfigValue());
   }
#endif
   else
   {
      dtGame::GMComponent::ProcessMessage(message);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::OnTickLocal(const float deltaSimTime)
{
   CheckForPlayerDisconnects();
}

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::OnPlayerJoinedServer(const dtGame::MachineInfo* joinerMachineInfo)
{
   //ForwardSelectedRoleToNewPlayer(joinerMachineInfo);

   if (NetworkBuddy::GetRef().IsServer())
   {
      ServerForwardGameTimeToNewPlayer(joinerMachineInfo);
      ServerForwardPlayerJoinedServer(joinerMachineInfo);
      ServerForwardGameInfoToNewPlayer(joinerMachineInfo);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::OnPlayerLeftServer(const dtGame::MachineInfo* quitterMachineInfo)
{
   //ClearQuitterIdentity(quitterMachineInfo);

   // If we're the server, then pass this message on
   if (NetworkBuddy::GetRef().IsServer())
   {
      ServerForwardPlayerLeftServer(quitterMachineInfo);
      //ServerReplaceLostInventoryItems(quitterMachineInfo);
   }
}

#if 0
////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::OnPlayerRequestedRole(
   const dtGame::MachineInfo* machineInfo, const std::string& roleID, const std::string& roleIdentity)
{
   // If that role isn't assigned to someone else
   if (!CCTS::IdentityAssignmentManager::GetRef().IsRoleFilled(roleID))
   {
      // Accept role selection
      dtCore::RefPtr<dtGame::Message> msg;
      CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::INFO_ROLE_ACCEPTED, msg);
      CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*msg, *machineInfo);
   }
   else
   {
      // Reject role selection
      dtCore::RefPtr<dtGame::Message> msg;
      CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::INFO_ROLE_REJECTED, msg);
      CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*msg, *machineInfo);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::OnPlayerChoseRole(const dtCore::UniqueId& roleID, const dtGame::MachineInfo& sender)
{
   // Setup our role.  Note that currently this is on ACTOR_UPDATE, so we have to make
   // sure we only do this once
   if (CCTS::IdentityAssignmentManager::GetRef().GetIdentity(&sender) == NULL)
   {
      SetupLocalRoleActor(roleID, &sender);
      UpdateContactListUI();
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::OnEnterableActorAddInventory(const dtGame::MachineInfo* machineInfo,
   const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType)
{
   UpdateEnterableInventory(enterableActorId, groupIndex, droppableCount, inventoryType, true);

   // If we're the server, then pass this message on
   if (NetworkBuddy::GetRef().IsServer())
   {
      ServerForwardEnterableInventoryUpdate(machineInfo, enterableActorId, groupIndex, droppableCount, inventoryType, true);
      ServerUpdatePortableInventoryMap(machineInfo, enterableActorId, groupIndex, droppableCount, inventoryType, true);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::OnEnterableActorRemoveInventory(const dtGame::MachineInfo* machineInfo,
   const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType)
{
   UpdateEnterableInventory(enterableActorId, groupIndex, droppableCount, inventoryType, false);

   // If we're the server, then pass this message on
   if (NetworkBuddy::GetRef().IsServer())
   {
      ServerForwardEnterableInventoryUpdate(machineInfo, enterableActorId, groupIndex, droppableCount, inventoryType, false);
      ServerUpdatePortableInventoryMap(machineInfo, enterableActorId, groupIndex, droppableCount, inventoryType, false);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::OnWarpActor(const std::string& warpedActorID,
   osg::Vec3 newTranslation, osg::Vec3 newRotation)
{
   // Forward this to every player in order
   if (CCTS::NetworkBuddy::GetRef().IsServer())
   {
      WarpLocalEnterableActor(dtCore::UniqueId(warpedActorID), newTranslation, newRotation);
      ServerForwardWarpActorMessage(warpedActorID, newTranslation, newRotation);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::OnExtinguishFire(const std::string& fireUniqueId)
{
   // Forward this to every player in order
   if (CCTS::NetworkBuddy::GetRef().IsServer())
   {
      ServerForwardFireExtinguishMessage(fireUniqueId);
   }
}
#endif

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::CheckForPlayerDisconnects()
{
   // See if anyone just disconnected
   for (net::NodeID nodeIndex = 1; nodeIndex < net::NetworkEngine::GetRef().GetNode().GetNumNodesReserved(); ++nodeIndex)
   {
      if (net::NetworkEngine::GetRef().GetNode().NodeJustDisconnected(nodeIndex))
      {
         dtCore::RefPtr<DeltaNetworkAdapter::NetworkedUniqueIDMessage> playerDisconnectMessage;
         GetGameManager()->GetMessageFactory().CreateMessage(DeltaNetworkAdapter::NetworkMessageType::INFO_LEFT_SERVER, playerDisconnectMessage);
         const dtGame::MachineInfo* machineInfo = NetworkBuddy::GetRef().GetMachineInfo(nodeIndex);
         assert(machineInfo);
         playerDisconnectMessage->SetUniqueId(machineInfo->GetUniqueId().ToString());
         GetGameManager()->SendMessage(*playerDisconnectMessage);
      }
   }
}

#if 0
////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ForwardSelectedRoleToNewPlayer(const dtGame::MachineInfo* joinerMachineInfo)
{
   // Make sure if we have a role, that the new player gets a full update of it
   CCTS::RoleActor* self = const_cast<CCTS::RoleActor*>(CCTS::IdentityAssignmentManager::GetRef().GetIdentity(
      &GetGameManager()->GetMachineInfo()));
   if (self != NULL)
   {
      dtCore::RefPtr<dtGame::ActorUpdateMessage> updateMsg = static_cast<dtGame::ActorUpdateMessage *>
         (GetGameManager()->GetMessageFactory().CreateMessage(dtGame::MessageType::INFO_ACTOR_UPDATED).get());
      self->GetGameActorProxy().PopulateActorUpdate(*updateMsg);
      CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*updateMsg, *joinerMachineInfo);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::UpdateContactListUI()
{
   // Since a new role has been chosen, make sure all the UI updates to match
   dtGame::GameManager* gameManager = GetGameManager();
   dtCore::RefPtr<dtGame::Message> msg;
   gameManager->GetMessageFactory().CreateMessage(UtilMessageType::ACTION_UPDATE_CONTACTS, msg);
   gameManager->SendMessage(*msg);
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ClearQuitterIdentity(const dtGame::MachineInfo* quitterMachineInfo)
{
   // Clear out local role information
   CCTS::RoleActor* existingRole = const_cast<CCTS::RoleActor*>(
      CCTS::IdentityAssignmentManager::GetRef().GetIdentity(quitterMachineInfo));
   if (existingRole != NULL)
   {
      existingRole->SetIdentityName("");
      dtGame::GameActorProxy* prototypeProxy = NULL;
      GetGameManager()->FindPrototypeByName(existingRole->GetPrototypeName(), prototypeProxy);
      if (prototypeProxy != NULL) // The logger has no prototype, so make sure we don't crash here when he leaves
      {
         CCTS::IdentityAssignmentManager::GetRef().UnsetRoleFilled(prototypeProxy->GetActor()->GetUniqueId().ToString());

         ReplaceContactListID(existingRole, false);
      }

      // Delete our copy of the role
      GetGameManager()->DeleteActor(existingRole->GetGameActorProxy());
   }
   CCTS::IdentityAssignmentManager::GetRef().UnassignIdentity(quitterMachineInfo);
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::SetupLocalRoleActor(const dtCore::UniqueId& roleID, const dtGame::MachineInfo* newPlayerMachineInfo)
{
   // find the role actor in the map
   CCTS::RoleActorProxy* roleActorProxy = NULL;
   CCTS::RoleActor* roleActor = NULL;
   GetGameManager()->FindActorById(dtCore::UniqueId(roleID), roleActorProxy);
   if (roleActorProxy != NULL)
   {
      roleActorProxy->GetActor(roleActor);
      assert(roleActor);

      if (roleActor->IsRemote())
      {
         ReplaceContactListID(roleActor, true);

         // Update Identity Assignment Manager
         CCTS::IdentityAssignmentManager::GetRef().AssignIdentity(newPlayerMachineInfo, roleActor);
      }
   }
}
#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ReplaceContactListID(CCTS::RoleActor* role, bool isJoining)
{
   // Get new guy's prototypeID
   dtGame::GameActorProxy* prototypeProxy = NULL;
   GetGameManager()->FindPrototypeByName(role->GetPrototypeName(), prototypeProxy);
   if (prototypeProxy) // note: this will be false for the logger...
   {
      dtCore::UniqueId protoTypeUniqueId = prototypeProxy->GetActor()->GetUniqueId();

      // Update our contact list to point to the new guy if he's in there
      CCTS::RoleActor* self = const_cast<CCTS::RoleActor*>(IdentityAssignmentManager::GetRef().GetIdentity(
         &GetGameManager()->GetMachineInfo()));
      if (self != NULL)
      {
         if (isJoining)
         {
            ReplaceLocalContactList(protoTypeUniqueId, role->GetUniqueId(), self);
         }
         else
         {
            ReplaceLocalContactList(role->GetUniqueId(), protoTypeUniqueId, self);
         }
      }
      else // We haven't picked a role yet
      {
         if (isJoining)
         {
            ReplacePrototypesContactLists(protoTypeUniqueId, role->GetUniqueId());
         }
         else
         {
            ReplacePrototypesContactLists(role->GetUniqueId(), protoTypeUniqueId);
         }
      }
   }
}
#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ReplaceLocalContactList(dtCore::UniqueId currentID, dtCore::UniqueId newID, CCTS::RoleActor* self)
{
   // If it's in our list, replace it with the actor's new UniqueID
   CCTS::RoleActor::ContactListIds& contactList = self->GetContactListIds();
   for (size_t contactIndex = 0; contactIndex < contactList.size(); ++contactIndex)
   {
      if (contactList[contactIndex] == currentID)
      {
         contactList[contactIndex] = newID;
      }
   }
   self->SetContactList(contactList);
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ReplacePrototypesContactLists(dtCore::UniqueId currentID, dtCore::UniqueId newID)
{
   // Update all local RoleActor prototype's contact lists
   std::vector<dtDAL::ActorProxy*> prototypeRoleProxies;
   GetGameManager()->FindPrototypesByActorType(*CCTS::ActorRegistry::ROLE_ACTOR_TYPE, prototypeRoleProxies);
   for (size_t roleIndex = 0; roleIndex < prototypeRoleProxies.size(); ++roleIndex)
   {
      CCTS::RoleActor* prototypeRole = dynamic_cast<CCTS::RoleActor*>(prototypeRoleProxies[roleIndex]->GetActor());
      CCTS::RoleActor::ContactListIds& contactList = prototypeRole->GetContactListIds();
      for (size_t contactIndex = 0; contactIndex < contactList.size(); ++contactIndex)
      {
         if (contactList[contactIndex] == currentID)
         {
            contactList[contactIndex] = newID;
         }
      }
      prototypeRole->SetContactList(contactList);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::UpdateEnterableInventory(const std::string& enterableActorId,
   int groupIndex, int droppableCount, const std::string& inventoryType, bool added)
{
   CCTS::EnterableActorProxy* enterableActorProxy = dynamic_cast<CCTS::EnterableActorProxy*>(
      GetGameManager()->FindActorById(dtCore::UniqueId(enterableActorId)));
   CCTS::EnterableActor* enterableActor = dynamic_cast<CCTS::EnterableActor*>(enterableActorProxy->GetActor());;
   if (enterableActor != NULL)
   {
      if (added)
      {
         enterableActor->AddInventoryItemToGroup(groupIndex, droppableCount, inventoryType);
      }
      else
      {
         enterableActor->RemoveInventoryItemFromGroup(groupIndex, inventoryType);
      }
      // If this is an EnterableActor we're inside, then update our inventory items
      CCTS::RoleActor* self = const_cast<CCTS::RoleActor*>(
         CCTS::IdentityAssignmentManager::GetRef().GetIdentity(&GetGameManager()->GetMachineInfo()));
      if (self != NULL && self->GetOccupiedEnterable() == enterableActor)
      {
         enterableActor->RefreshInventoryItemButtons();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::WarpLocalEnterableActor(dtCore::UniqueId warpedActorID,
   osg::Vec3 newTranslation, osg::Vec3 newRotation)
{
   dtDAL::ActorProxy* warpedActorProxy = GetGameManager()->FindActorById(warpedActorID);
   CCTS::EnterableActor* warpedEnterable = dynamic_cast<CCTS::EnterableActor*>(warpedActorProxy->GetActor());
   if (warpedEnterable != NULL)
   {
      dtCore::Transform targetTransform;
      warpedEnterable->GetTransform(targetTransform);
      newTranslation.z() -= 0.25f;
      targetTransform.SetTranslation(newTranslation);
      warpedEnterable->SetTransform(targetTransform);
   }
}
#endif

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::ServerForwardGameTimeToNewPlayer(const dtGame::MachineInfo* joinerMachineInfo)
{
   dtCore::RefPtr<dtGame::TimeChangeMessage> timeChangeMessage;
   GetGameManager()->GetMessageFactory().CreateMessage(dtGame::MessageType::INFO_TIME_CHANGED, timeChangeMessage);
   timeChangeMessage->SetSimulationTime(GetGameManager()->GetSimulationTime());
   timeChangeMessage->SetSimulationClockTime(GetGameManager()->GetSimulationClockTime());
   NetworkBuddy::GetRef().SendNetworkMessage(*timeChangeMessage, *joinerMachineInfo);
}

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::ServerForwardPlayerJoinedServer(const dtGame::MachineInfo* joinerMachineInfo)
{
   dtCore::RefPtr<DeltaNetworkAdapter::NetworkedUniqueIDMessage> msg;
   NetworkBuddy::GetRef().CreateMessage(DeltaNetworkAdapter::NetworkMessageType::INFO_JOINED_SERVER, msg);
   msg->SetUniqueId(joinerMachineInfo->GetUniqueId().ToString());

   const MachineInfoList& connectedMachines = NetworkBuddy::GetRef().GetConnectedMachineList();

   for (size_t connectedMachineIndex = 0; connectedMachineIndex < connectedMachines.size(); ++connectedMachineIndex)
   {
      if (!connectedMachines[connectedMachineIndex].valid() || *connectedMachines[connectedMachineIndex] == GetGameManager()->GetMachineInfo() ||
         joinerMachineInfo == connectedMachines[connectedMachineIndex])
      {
         continue;
      }
      NetworkBuddy::GetRef().SendNetworkMessage(*msg, *connectedMachines[connectedMachineIndex]);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::ServerForwardPlayerLeftServer(const dtGame::MachineInfo* quitterMachineInfo)
{
   if (!NetworkBuddy::GetRef().IsServer()) { return; }

   dtCore::RefPtr<DeltaNetworkAdapter::NetworkedUniqueIDMessage> msg;
   NetworkBuddy::GetRef().CreateMessage(DeltaNetworkAdapter::NetworkMessageType::INFO_LEFT_SERVER, msg);
   msg->SetUniqueId(quitterMachineInfo->GetUniqueId().ToString());

   const MachineInfoList& connectedMachineList = NetworkBuddy::GetRef().GetConnectedMachineList();
   for (size_t i = 0; i < connectedMachineList.size(); ++i)
   {
      if (!connectedMachineList[i].valid()) { continue; }
      if (connectedMachineList[i]->GetUniqueId() == GetGameManager()->GetMachineInfo().GetUniqueId()) { continue; }
      if (connectedMachineList[i]->GetUniqueId() == quitterMachineInfo->GetUniqueId()) { continue; }
      // got past all checks -- send it on
      NetworkBuddy::GetRef().SendNetworkMessage(*msg, *connectedMachineList[i]);
   }
}

#if 0
////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerReplaceLostInventoryItems(const dtGame::MachineInfo* quitterMachineInfo)
{
   std::vector<PortableItem>& removedItemList = mRemovedPortableItemMap[quitterMachineInfo->GetUniqueId().ToString()];
   for (size_t itemIndex = 0; itemIndex < removedItemList.size(); ++itemIndex)
   {
      OnEnterableActorAddInventory(quitterMachineInfo, removedItemList[itemIndex].originEnterableId,
         removedItemList[itemIndex].originGroupIndex, 1, removedItemList[itemIndex].inventoryType);
   }
}
#endif

////////////////////////////////////////////////////////////////////////////////
void ScenarioComponent::ServerForwardGameInfoToNewPlayer(const dtGame::MachineInfo* newPlayerMachineInfo)
{
   // Send EnterableActors' current inventory lists information
   std::vector<dtDAL::ActorProxy*> actorProxies;
   //GetGameManager()->FindActorsByType(*CCTS::ActorRegistry::OFFICE_ACTOR_TYPE, actorProxies);
   //ServerForwardEnterableActorInfoToNewPlayer(actorProxies, newPlayerMachineInfo);
   //GetGameManager()->FindActorsByType(*CCTS::ActorRegistry::VEHICLE_ACTOR_TYPE, actorProxies);
   //ServerForwardEnterableActorInfoToNewPlayer(actorProxies, newPlayerMachineInfo);

   // Notify new player of all pickupable actors
   //GetGameManager()->FindActorsByType(*CCTS::ActorRegistry::PICKUPABLE_ACTOR_TYPE, actorProxies);
   //ServerForwardPickupableInfoToNewPlayer(actorProxies, newPlayerMachineInfo);

   // Notify new player of the state of all FireActors
   //GetGameManager()->FindActorsByType(*CCTS::ActorRegistry::FIRE_ACTOR_TYPE, actorProxies);
   //ServerForwardFireActorStatusToNewPlayer(actorProxies, newPlayerMachineInfo);
}

#if 0
////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerForwardEnterableActorInfoToNewPlayer(
   std::vector<dtDAL::ActorProxy*> enterableProxies, const dtGame::MachineInfo* newPlayerMachineInfo)
{
   // Tell new player about the state of EnterableActor's inventory items
   for (size_t enterableIndex = 0; enterableIndex < enterableProxies.size(); ++enterableIndex)
   {
      CCTS::EnterableActor* enterableActor = dynamic_cast<CCTS::EnterableActor*>(enterableProxies[enterableIndex]->GetActor());
      if (enterableActor != NULL)
      {
         std::vector< dtCore::RefPtr<CCTS::InventoryGroup> > itemGroups = enterableActor->GetAllInventoryGroups();
         for (size_t invGroupIndex = 0; invGroupIndex < itemGroups.size(); ++invGroupIndex)
         {
            CCTS::InventoryGroup::ItemContainer inventoryGroup = itemGroups[invGroupIndex]->GetAllItems();
            for (size_t itemIndex = 0; itemIndex < inventoryGroup.size(); ++itemIndex)
            {
               int droppableCount = 1;
               CCTS::DroppableInventoryItem* droppable = dynamic_cast<CCTS::DroppableInventoryItem*>(inventoryGroup[itemIndex].get());
               if (droppable != NULL)
               {
                  droppableCount = droppable->GetItemCount();
               }
               dtCore::RefPtr<CCTS::EnterableInventoryUpdateMessage> inventoryUpdateMessage;
               CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::ACTION_ADD_INVENTORY_TO_ENTERABLE, inventoryUpdateMessage);
               inventoryUpdateMessage->SetUniqueId(GetGameManager()->GetMachineInfo().GetUniqueId().ToString());
               inventoryUpdateMessage->SetEnterableUniqueId(enterableActor->GetUniqueId().ToString());
               inventoryUpdateMessage->SetInventoryGroupIndex(invGroupIndex);
               inventoryUpdateMessage->SetDroppableCount(droppableCount);
               inventoryUpdateMessage->SetInventoryItemType(inventoryGroup[itemIndex]->GetType());
               CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*inventoryUpdateMessage, *newPlayerMachineInfo);
            }
         }

         // Forward the EnterableActor's location
         dtCore::Transform currentTransform;
         enterableActor->GetTransform(currentTransform);
         osg::Vec3 rotation;
         currentTransform.GetRotation(rotation);
         dtCore::RefPtr<CCTS::ActorUpdateMessage> warpMessage;
         CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::ACTION_CCTS_WARP_ACTOR, warpMessage);
         warpMessage->SetUniqueId(enterableActor->GetUniqueId().ToString());
         warpMessage->SetTranslation(currentTransform.GetTranslation());
         warpMessage->SetRotation(rotation);
         CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*warpMessage, *newPlayerMachineInfo);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerForwardPickupableInfoToNewPlayer(
   std::vector<dtDAL::ActorProxy*> pickupableProxies, const dtGame::MachineInfo* newPlayerMachineInfo)
{
   // Tell new player about the state of EnterableActor's inventory items
   for (size_t pickupableIndex = 0; pickupableIndex < pickupableProxies.size(); ++pickupableIndex)
   {
      dtGame::GameActorProxy* pickupableProxy = dynamic_cast<dtGame::GameActorProxy*>(pickupableProxies[pickupableIndex]);
      if (pickupableProxy != NULL)
      {
         dtCore::RefPtr<dtGame::ActorUpdateMessage> updateMsg = static_cast<dtGame::ActorUpdateMessage *>
            (GetGameManager()->GetMessageFactory().CreateMessage(dtGame::MessageType::INFO_ACTOR_UPDATED).get());
         pickupableProxy->PopulateActorUpdate(*updateMsg);
         CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*updateMsg, *newPlayerMachineInfo);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerForwardFireActorStatusToNewPlayer(std::vector<dtDAL::ActorProxy*> fireProxies, const dtGame::MachineInfo* newPlayerMachineInfo)
{
   // Tell new player about the state of EnterableActor's inventory items
   for (size_t fireIndex = 0; fireIndex < fireProxies.size(); ++fireIndex)
   {
      dtGame::GameActorProxy* fireProxy = dynamic_cast<dtGame::GameActorProxy*>(fireProxies[fireIndex]);
      if (fireProxy != NULL)
      {
         std::vector<dtUtil::RefString> extinguishedProperty;
         extinguishedProperty.push_back("IsActive");
         extinguishedProperty.push_back("Extinguished");
         dtCore::RefPtr<dtGame::ActorUpdateMessage> updateMsg = static_cast<dtGame::ActorUpdateMessage *>
            (GetGameManager()->GetMessageFactory().CreateMessage(dtGame::MessageType::INFO_ACTOR_UPDATED).get());
         fireProxy->PopulateActorUpdate(*updateMsg, extinguishedProperty);
         CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*updateMsg, *newPlayerMachineInfo);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerForwardEnterableInventoryUpdate(const dtGame::MachineInfo* machineInfo,
   const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType, bool added)
{
   dtCore::RefPtr<CCTS::EnterableInventoryUpdateMessage> inventoryUpdateMessage;
   if (added)
   {
      CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::ACTION_ADD_INVENTORY_TO_ENTERABLE, inventoryUpdateMessage);
   }
   else
   {
      CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::ACTION_REMOVE_INVENTORY_FROM_ENTERABLE, inventoryUpdateMessage);
   }
   inventoryUpdateMessage->SetEnterableUniqueId(enterableActorId);
   inventoryUpdateMessage->SetInventoryGroupIndex(groupIndex);
   inventoryUpdateMessage->SetDroppableCount(droppableCount);
   inventoryUpdateMessage->SetInventoryItemType(inventoryType);

   const MachineInfoList& connectedMachineList = NetworkBuddy::GetRef().GetConnectedMachineList();
   for (size_t i = 0; i < connectedMachineList.size(); ++i)
   {
      if (!connectedMachineList[i].valid()) { continue; }
      if (connectedMachineList[i]->GetUniqueId() == GetGameManager()->GetMachineInfo().GetUniqueId()) { continue; }
      if (connectedMachineList[i]->GetUniqueId() == machineInfo->GetUniqueId()) { continue; }
      if (CCTS::IdentityAssignmentManager::GetRef().GetIdentity(connectedMachineList[i]) == NULL) { continue; }
      // got past all checks -- send it on
      CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*inventoryUpdateMessage, *connectedMachineList[i]);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerUpdatePortableInventoryMap(const dtGame::MachineInfo* machineInfo,
   const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType, bool added)
{
   if (added)
   {
      // Remove an instance for each droppable item we are losing
      while(droppableCount-- > 0)
      {
         // Remove from map of removed portable items
         std::vector<PortableItem>& removedItemList = mRemovedPortableItemMap[machineInfo->GetUniqueId().ToString()];
         for (size_t itemIndex = 0; itemIndex < removedItemList.size(); ++itemIndex)
         {
            if (removedItemList[itemIndex].inventoryType == inventoryType)
            {
               removedItemList.erase(removedItemList.begin() + itemIndex);
               break;
            }
         }
      }
   }
   else
   {
      // Add a new instance for each droppable item we have
      while(droppableCount-- > 0)
      {
         // Add to map of removed portable items
         PortableItem item;
         item.originEnterableId = enterableActorId;
         item.originGroupIndex = groupIndex;
         item.inventoryType = inventoryType;
         mRemovedPortableItemMap[machineInfo->GetUniqueId().ToString()].push_back(item);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerRemoveInventoryFromOriginMap(
   const dtGame::MachineInfo* machineInfo, const std::string& inventoryType)
{
   std::vector<PortableItem>& removedItemList = mRemovedPortableItemMap[machineInfo->GetUniqueId().ToString()];
   for (size_t itemIndex = 0; itemIndex < removedItemList.size(); ++itemIndex)
   {
      if (removedItemList[itemIndex].inventoryType == inventoryType)
      {
         removedItemList.erase(removedItemList.begin() + itemIndex);
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerForwardWarpActorMessage(const std::string& warpedActorID,
   osg::Vec3 newTranslation, osg::Vec3 newRotation)
{
   dtCore::RefPtr<CCTS::ActorUpdateMessage> warpMessage;
   CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::ACTION_CCTS_WARP_ACTOR, warpMessage);
   warpMessage->SetUniqueId(warpedActorID);
   warpMessage->SetTranslation(newTranslation);
   warpMessage->SetRotation(newRotation);
   const MachineInfoList& connectedMachineList = CCTS::NetworkBuddy::GetRef().GetConnectedMachineList();
   for (size_t i = 0; i < connectedMachineList.size(); ++i)
   {
      // Make sure the connection is valid
      if (!connectedMachineList[i].valid()) { continue; }
      // Don't send it to ourself
      if (connectedMachineList[i]->GetUniqueId() == GetGameManager()->GetMachineInfo().GetUniqueId()) { continue; }
      // got past all checks -- send it on
      CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*warpMessage, *connectedMachineList[i]);
   }
}

////////////////////////////////////////////////////////////////////////////////
void CCTS::ScenarioComponent::ServerForwardFireExtinguishMessage(const std::string& fireUniqueId)
{
   dtCore::RefPtr< ConfigurationMessage<std::string> > extinguishMessage;
   CCTS::NetworkBuddy::GetRef().CreateMessage(CCTS::UtilMessageType::ACTION_EXTINGUISH_FIRE, extinguishMessage);
   extinguishMessage->SetConfigValue(fireUniqueId);
   const MachineInfoList& connectedMachineList = CCTS::NetworkBuddy::GetRef().GetConnectedMachineList();
   for (size_t i = 0; i < connectedMachineList.size(); ++i)
   {
      // Make sure the connection is valid
      if (!connectedMachineList[i].valid()) { continue; }
      // Don't send it to ourself
      if (connectedMachineList[i]->GetUniqueId() == GetGameManager()->GetMachineInfo().GetUniqueId()) { continue; }
      // got past all checks -- send it on
      CCTS::NetworkBuddy::GetRef().SendNetworkMessage(*extinguishMessage, *connectedMachineList[i]);
   }
}
#endif

////////////////////////////////////////////////////////////////////////////////
