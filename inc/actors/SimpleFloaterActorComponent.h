#ifndef SimpleFloaterActorComponent_h__
#define SimpleFloaterActorComponent_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <dtGame/actorcomponent.h>

#include <util/DeltaOceanGetHeight.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/oceanactor.h>
#else
namespace dtOcean
{
   class OceanActor;
}
#endif

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT SimpleFloaterActorComponent : public dtGame::ActorComponent
{
public:
   SimpleFloaterActorComponent(dtOcean::OceanActor& oceanActor, const std::string& type = "SimpleFloaterComponent");

   virtual void OnAddedToActor(dtGame::GameActor& actor);
   virtual void OnRemovedFromActor(dtGame::GameActor& actor);

   virtual void OnTickLocal(const dtGame::TickMessage& tickMessage);

protected:
   virtual ~SimpleFloaterActorComponent();

private:
   dtCore::RefPtr<DeltaOceanGetHeight> mpOceanHeightGetter;
};

////////////////////////////////////////////////////////////////////////////////

#endif // SimpleFloaterActorComponent_h__
