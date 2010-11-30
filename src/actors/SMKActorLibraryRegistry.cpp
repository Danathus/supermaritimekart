#include <actors/SMKActorLibraryRegistry.h>

#include <actors/MineActor.h>
#include <actors/PickUpItemHandle.h>
#include <actors/RocketActor.h>
#include <actors/TorpedoActor.h>
#include <actors/SMKBoatActor.h>

#include <dtDAL/actorpluginregistry.h>
#include <dtDAL/actortype.h>

using dtCore::RefPtr;

RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::SMK_BOAT_ACTOR_TYPE(new dtDAL::ActorType("SMKBoatActor", "SuperMaritimeKart"));
RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::SMK_PICKUP_ACTOR_TYPE(new dtDAL::ActorType("SMKPickUp", "SuperMaritimeKart"));
RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::MINE_ACTOR_TYPE(new dtDAL::ActorType("MineActor", "SuperMaritimeKart"));
RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::ROCKET_ACTOR_TYPE(new dtDAL::ActorType("RocketActor", "SuperMaritimeKart"));
RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::TORPEDO_ACTOR_TYPE(new dtDAL::ActorType("TorpedoActor", "SuperMaritimeKart"));

//////////////////////////////////////////////////////////////////////////
extern "C" SMK_ACTOR_EXPORT dtDAL::ActorPluginRegistry* CreatePluginRegistry()
{
   return new SMKActorLibraryRegistry;
}

//////////////////////////////////////////////////////////////////////////
extern "C" SMK_ACTOR_EXPORT void DestroyPluginRegistry(dtDAL::ActorPluginRegistry* registry)
{
   delete registry;
}

//////////////////////////////////////////////////////////////////////////
SMKActorLibraryRegistry::SMKActorLibraryRegistry()
   : dtDAL::ActorPluginRegistry("Actors used across Delta3D based projects.")
{
}

//////////////////////////////////////////////////////////////////////////
void SMKActorLibraryRegistry::RegisterActorTypes()
{
   mActorFactory->RegisterType<SMKBoatActorProxy>(SMK_BOAT_ACTOR_TYPE.get());
   mActorFactory->RegisterType<SMK::PickUpItemBaseProxy>(SMK_PICKUP_ACTOR_TYPE.get());
   mActorFactory->RegisterType<MineActorProxy>(MINE_ACTOR_TYPE.get());
   mActorFactory->RegisterType<RocketActorProxy>(ROCKET_ACTOR_TYPE.get());
   mActorFactory->RegisterType<TorpedoActorProxy>(TORPEDO_ACTOR_TYPE.get());
}

//////////////////////////////////////////////////////////////////////////
