#include <SMKActorLibraryRegistry.h>

#include <dtDAL/actorpluginregistry.h>
#include <dtDAL/actortype.h>

#include <SMKBoatActor.h>

using dtCore::RefPtr;

RefPtr<dtDAL::ActorType> SMKActorLibraryRegistry::SMK_BOAT_ACTOR_TYPE(new dtDAL::ActorType("SMKBoatActor", "SuperMaritimeKart"));

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
}

//////////////////////////////////////////////////////////////////////////
