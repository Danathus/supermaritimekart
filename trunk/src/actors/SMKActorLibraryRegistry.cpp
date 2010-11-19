#include <actors/SMKActorLibraryRegistry.h>

#include <actors/SMKBoatActor.h>
#include <actors/PickUpItemHandle.h>

#include <dtDAL/actorpluginregistry.h>
#include <dtDAL/actortype.h>

using dtCore::RefPtr;

RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::SMK_BOAT_ACTOR_TYPE(new dtDAL::ActorType("SMKBoatActor", "SuperMaritimeKart"));
RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::SMK_PICKUP_ACTOR_TYPE(new dtDAL::ActorType("SMKPickUp", "SuperMaritimeKart"));

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
}

//////////////////////////////////////////////////////////////////////////
