#ifndef SCENARIO_COMPONENT__H
#define SCENARIO_COMPONENT__H

////////////////////////////////////////////////////////////////////////////////

#include <dtGame/gmcomponent.h>

//#include <CCTS_ActorLibrary/ActorLibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

//namespace CCTS
//{
   //class RoleActor;

   class ScenarioComponent : public dtGame::GMComponent
   {
   public:
      ScenarioComponent();

      // implementations of virtual methods
      void ProcessMessage(const dtGame::Message& message);

   protected:
   private:
      // Message processing functions
      void OnTickLocal(const float deltaSimTime);
      void OnPlayerJoinedServer(const dtGame::MachineInfo* joinerMachineInfo);
      void OnPlayerLeftServer(const dtGame::MachineInfo* quitterMachineInfo);
      //void OnPlayerRequestedRole(const dtGame::MachineInfo* machineInfo, const std::string& roleID, const std::string& roleIdentity);
      //void OnPlayerChoseRole(const dtCore::UniqueId& roleID, const dtGame::MachineInfo& sender);
      //void OnEnterableActorAddInventory(const dtGame::MachineInfo* machineInfo, const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType);
      //void OnEnterableActorRemoveInventory(const dtGame::MachineInfo* machineInfo, const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType);
      //void OnWarpActor(const std::string& warpedActorID, osg::Vec3 newTranslation, osg::Vec3 newRotation);
      //void OnExtinguishFire(const std::string& fireUniqueId);

      // Update local actor functions
      void CheckForPlayerDisconnects();
      //void ForwardSelectedRoleToNewPlayer(const dtGame::MachineInfo* joinerMachineInfo);
      //void UpdateContactListUI();
      //void ClearQuitterIdentity(const dtGame::MachineInfo* quitterMachineInfo);
      //void SetupLocalRoleActor(const dtCore::UniqueId& roleID, const dtGame::MachineInfo* newPlayerMachineInfo);
      //void ReplaceContactListID(CCTS::RoleActor* role, bool isJoining);
      //void ReplaceLocalContactList(dtCore::UniqueId currentID, dtCore::UniqueId newID, CCTS::RoleActor* self);
      //void ReplacePrototypesContactLists(dtCore::UniqueId currentID, dtCore::UniqueId newID);
      //void UpdateEnterableInventory(const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType, bool added);
      //void WarpLocalEnterableActor(dtCore::UniqueId warpedActorID, osg::Vec3 newTranslation, osg::Vec3 newRotation);

      // Server forward functions
      void ServerForwardGameTimeToNewPlayer(const dtGame::MachineInfo* joinerMachineInfo);
      void ServerForwardPlayerJoinedServer(const dtGame::MachineInfo* joinerMachineInfo);
      void ServerForwardPlayerLeftServer(const dtGame::MachineInfo* quitterMachineInfo);
      //void ServerReplaceLostInventoryItems(const dtGame::MachineInfo* quitterMachineInfo);
      void ServerForwardGameInfoToNewPlayer(const dtGame::MachineInfo* newPlayerMachineInfo);
      //void ServerForwardEnterableActorInfoToNewPlayer(std::vector<dtDAL::ActorProxy*> enterableProxies, const dtGame::MachineInfo* newPlayerMachineInfo);
      //void ServerForwardPickupableInfoToNewPlayer(std::vector<dtDAL::ActorProxy*> pickupableProxies, const dtGame::MachineInfo* newPlayerMachineInfo);
      //void ServerForwardFireActorStatusToNewPlayer(std::vector<dtDAL::ActorProxy*> fireProxies, const dtGame::MachineInfo* newPlayerMachineInfo);
      //void ServerForwardEnterableInventoryUpdate(const dtGame::MachineInfo* machineInfo, const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType, bool added);
      //void ServerUpdatePortableInventoryMap(const dtGame::MachineInfo* machineInfo, const std::string& enterableActorId, int groupIndex, int droppableCount, const std::string& inventoryType, bool added);
      //void ServerRemoveInventoryFromOriginMap(const dtGame::MachineInfo* machineInfo, const std::string& inventoryType);
      //void ServerForwardWarpActorMessage(const std::string& warpedActorID, osg::Vec3 newTranslation, osg::Vec3 newRotation);
      //void ServerForwardFireExtinguishMessage(const std::string& fireUniqueId);

      struct PortableItem
      {
         std::string originEnterableId;
         int originGroupIndex;
         std::string inventoryType;
      };

      // maps machine info unique ID strings to a vector of portable items
      std::map<std::string, std::vector<PortableItem> > mRemovedPortableItemMap;
   };
//}

////////////////////////////////////////////////////////////////////////////////

#endif // SCENARIO_COMPONENT__H
