#include <actors/MineActor.h>
#include <actors/SimpleFloaterActorComponent.h>

#include <dtCore/transform.h>
#include <dtGame/gamemanager.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/actorregistry.h>
# include <dtOcean/oceanactor.h>
#endif

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
static const float MINE_LIFETIME = 30.0f;
static const float MINE_ARMING_DELAY = 1.5f;

//////////////////////////////////////////////////////////////////////////
MineActor::MineActor(dtGame::GameActorProxy& proxy)
: ProjectileActor(proxy)
{
   SetLifetime(MINE_LIFETIME);
   SetArmingDelay(MINE_ARMING_DELAY);
}

//////////////////////////////////////////////////////////////////////////
MineActor::~MineActor()
{
}

///////////////////////////////////////////////////////////////////////////////
void MineActor::TickLocal(const dtGame::Message& msg)
{
   ProjectileActor::TickLocal(msg);

   dtCore::Transform currentTransform;
   GetTransform(currentTransform);
   currentTransform.SetRotation(currentTransform.GetRotation() + osg::Vec3(1.0, 0.0, 0.0));
   SetTransform(currentTransform);
}

///////////////////////////////////////////////////////////////////////////////
void MineActor::OnEnteredWorld()
{
   SetupFloaterComponent();
   SetMeshResource("Boat:Mine_LG.ive");
}

///////////////////////////////////////////////////////////////////////////////
void MineActor::SetupFloaterComponent()
{
#ifdef BUILD_WITH_DTOCEAN
   //find any OceanActors;
   dtOcean::OceanActorProxy* oceanActorProxy(NULL);
   GetGameActorProxy().GetGameManager()->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
   dtOcean::OceanActor* oceanActor(NULL);
   oceanActorProxy->GetActor(oceanActor);
   assert(oceanActor != NULL);

   // Create a new floater component with the ocean
   mpFloaterComponent = new SimpleFloaterActorComponent(*oceanActor);
   AddComponent(*mpFloaterComponent);
#endif
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
MineActorProxy::MineActorProxy()
{

}

///////////////////////////////////////////////////////////////////////////////
MineActorProxy::~MineActorProxy()
{

}

///////////////////////////////////////////////////////////////////////////////
void MineActorProxy::CreateActor()
{
   MineActor* pActor = new MineActor(*this);
   SetActor(*pActor);
}

////////////////////////////////////////////////////////////////////////////////

