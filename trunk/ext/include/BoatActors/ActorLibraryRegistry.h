#ifndef _BOAT_ACTORS_LIBRARY_ACTOR_REGISTRY_H_
#define _BOAT_ACTORS_LIBRARY_ACTOR_REGISTRY_H_

#include <dtDAL/actorpluginregistry.h>

#include <BoatActors/ActorLibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * Class that exports the applicable actor proxies to a library
 */
class BOAT_ACTORS_LIBRARY_EXPORT BoatActorsLibraryRegistry : public dtDAL::ActorPluginRegistry
{
public:


   static dtCore::RefPtr<dtDAL::ActorType> BOAT_ACTOR_TYPE;
   static dtCore::RefPtr<dtDAL::ActorType> FLOATING_ACTOR_TYPE;

   /// Constructor
   BoatActorsLibraryRegistry();

   /// Registers all of the actor proxies to be exported
   void RegisterActorTypes();
};

////////////////////////////////////////////////////////////////////////////////



#endif // _BOAT_ACTORS_LIBRARY_ACTOR_REGISTRY_H_
