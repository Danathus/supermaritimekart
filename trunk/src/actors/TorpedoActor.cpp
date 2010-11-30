#include <actors/TorpedoActor.h>
#include <actors/SimpleFloaterActorComponent.h>

#include <dtCore/transform.h>

#include <dtGame/gamemanager.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/actorregistry.h>
# include <dtOcean/oceanactor.h>
#endif

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
static const float TORPEDO_SPEED = 1.0f;
static const float TORPEDO_LIFETIME = 5.0f;
static const float TORPEDO_ARMING_DELAY = 0.5f;

//////////////////////////////////////////////////////////////////////////
TorpedoActor::TorpedoActor(dtGame::GameActorProxy& proxy)
: ProjectileActor(proxy)
{
   SetLifetime(TORPEDO_LIFETIME);
   SetArmingDelay(TORPEDO_ARMING_DELAY);
}

//////////////////////////////////////////////////////////////////////////
TorpedoActor::~TorpedoActor()
{
   if (mpFloaterComponent.valid())
   {
      RemoveComponent(*mpFloaterComponent);
      mpFloaterComponent = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
void TorpedoActor::TickLocal(const dtGame::Message& msg)
{
   ProjectileActor::TickLocal(msg);

   dtCore::Transform currentTransform;
   GetTransform(currentTransform);
   osg::Vec3 right, up, forward, translation;
   currentTransform.GetOrientation(right, up, forward);
   currentTransform.GetTranslation(translation);
   if (!mpFloaterComponent.valid())
   {
      currentTransform.SetRotation(currentTransform.GetRotation() + osg::Vec3(0.0, -0.4, 0.0));
   }
   currentTransform.SetTranslation(translation + forward * TORPEDO_SPEED);
   SetTransform(currentTransform);

   if (IsArmed() && !mpFloaterComponent.valid())
   {
      SetupFloaterComponent();
   }
}

///////////////////////////////////////////////////////////////////////////////
void TorpedoActor::OnEnteredWorld()
{
   SetMeshResource("Boat:TorpedoType1.IVE");
}

///////////////////////////////////////////////////////////////////////////////
void TorpedoActor::SetupFloaterComponent()
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

   // Clear out any pitch we have so we're moving forward
   dtCore::Transform currentTransform;
   GetTransform(currentTransform);
   osg::Vec3 rotation = currentTransform.GetRotation();
   rotation.y() = 0.0;
   currentTransform.SetRotation(rotation);
   SetTransform(currentTransform);
#endif
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TorpedoActorProxy::TorpedoActorProxy()
{

}

///////////////////////////////////////////////////////////////////////////////
TorpedoActorProxy::~TorpedoActorProxy()
{

}

///////////////////////////////////////////////////////////////////////////////
void TorpedoActorProxy::CreateActor()
{
   TorpedoActor* pActor = new TorpedoActor(*this);
   SetActor(*pActor);
}

////////////////////////////////////////////////////////////////////////////////

