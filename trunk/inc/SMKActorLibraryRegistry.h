#ifndef SMKActorLibraryRegistry_h_
#define SMKActorLibraryRegistry_h_

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <dtDAL/actorpluginregistry.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * Class that exports the applicable actor proxies to a library
 */
class SMK_ACTOR_EXPORT SMKActorLibraryRegistry : public dtDAL::ActorPluginRegistry
{
public:
   static dtCore::RefPtr<dtDAL::ActorType> SMK_BOAT_ACTOR_TYPE;
   static dtCore::RefPtr<dtDAL::ActorType> SMK_PICKUP_ACTOR_TYPE;

   /// Constructor
   SMKActorLibraryRegistry();

   /// Registers all of the actor proxies to be exported
   void RegisterActorTypes();
};

////////////////////////////////////////////////////////////////////////////////



#endif // SMKActorLibraryRegistry_h_
