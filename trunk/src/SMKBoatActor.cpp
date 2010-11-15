#include <SMKBoatActor.h>
#include <PickUpItemHandle.h>
#include <TurretWeapon.h>
#include <Weapon.h>

#include <dtGame/basemessages.h>
#include <dtGame/gamemanager.h>
#include <dtAudio/audiomanager.h>

#include <ode/contact.h>

using namespace SMK;

//////////////////////////////////////////////////////////
// Actor code
//////////////////////////////////////////////////////////
SMKBoatActor::SMKBoatActor(SMKBoatActorProxy& proxy)
: BoatActor(proxy)
, mpFrontWeapon(new TurretWeapon("Front Weapon"))
, mpBackWeapon(new Weapon("Back Weapon"))
{
   SetName("SMKBoat");
}

////////////////////////////////////////////////////////////////////////////////
SMKBoatActor::~SMKBoatActor()
{
   mpFrontWeapon = NULL;
   mpBackWeapon = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::TickLocal(const dtGame::Message& msg)
{
   BoatActor::TickLocal(msg);

   const dtGame::TickMessage& tickMessage = static_cast<const dtGame::TickMessage&>(msg);
   float dt = tickMessage.GetDeltaSimTime();

   if (mpFrontWeapon)
   {
      mpFrontWeapon->Update(dt);
   }

   if (mpBackWeapon)
   {
      mpBackWeapon->Update(dt);
   }
}

////////////////////////////////////////////////////////////////////////////////
Weapon* SMKBoatActor::GetFrontWeapon()
{
   return mpFrontWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
const Weapon* SMKBoatActor::GetFrontWeapon() const
{
   return mpFrontWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
Weapon* SMKBoatActor::GetBackWeapon()
{
   return mpBackWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
const Weapon* SMKBoatActor::GetBackWeapon() const
{
   return mpBackWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
bool SMKBoatActor::FilterContact(dContact* contact, Transformable* collider)
{
   // Do not send events in STAGE.
   if (!GetGameActorProxy().IsInSTAGE())
   {
      PickUpItemHandle* pickup = dynamic_cast<PickUpItemHandle*>(collider);

      if (pickup)
      {
         dtGame::GameActorProxy& boatProxy = GetGameActorProxy();

         osg::Vec3 position(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]);
         osg::Vec3 normal(contact->geom.normal[0], contact->geom.normal[1], contact->geom.normal[2]);         

         // Remove the pickup actor
         boatProxy.GetGameManager()->DeleteActor(pickup->GetGameActorProxy());

         // TEMP
         // This should happen when the item is acquired
         {
            mPickupAcquireSound->Play();
         }         
      }
   }

   return false;
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::OnEnteredWorld()
{
   BoatActor::OnEnteredWorld();

   mPickupAcquireSound = dtAudio::AudioManager::GetInstance().NewSound();
   mPickupAcquireSound->LoadFile("sounds/pop.wav");
   mPickupAcquireSound->SetGain(1.0f);
   mPickupAcquireSound->SetListenerRelative(false);
}


////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::OnRemovedFromWorld()
{
   //clear out our references so instances will be destroyed.
   BoatActor::OnRemovedFromWorld();

   mpFrontWeapon = NULL;
   mpBackWeapon = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::SetupDefaultWeapon()
{
   if (mpFrontWeapon)
   {
      mpFrontWeapon->Initialize(&GetGameActorProxy());
   }

   if (mpBackWeapon)
   {
      mpBackWeapon->Initialize(&GetGameActorProxy());
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::Initialize()
{
   BoatActor::Initialize();
}

///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Proxy code
//////////////////////////////////////////////////////////
SMKBoatActorProxy::SMKBoatActorProxy() 
{
   SetClassName("SMKBoatActor");
}

////////////////////////////////////////////////////////////////////////////////
SMKBoatActorProxy::~SMKBoatActorProxy()
{
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActorProxy::BuildPropertyMap()
{
   BoatActorProxy::BuildPropertyMap();

   SMKBoatActor *actor = NULL;
   GetActor(actor);

   if (!actor)
   {
      return;
   }

   if (actor->GetFrontWeapon())
   {
      actor->GetFrontWeapon()->BuildPropertyMap(this);
   }

   if (actor->GetBackWeapon())
   {
      actor->GetBackWeapon()->BuildPropertyMap(this);
   }
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActorProxy::BuildInvokables()
{
   BoatActorProxy::BuildInvokables();
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActorProxy::CreateActor()
{
   SMKBoatActor* pActor = new SMKBoatActor(*this);
   SetActor(*pActor);
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActorProxy::OnRemovedFromWorld()
{
   //tell our BoatActor it's time to go
   static_cast<BoatActor*>(GetActor())->OnRemovedFromWorld();
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActorProxy::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   BoatActorProxy::GetPartialUpdateProperties(propNamesToFill);

   SMKBoatActor *actor = NULL;
   GetActor(actor);

   if (!actor)
   {
      return;
   }

   if (actor->GetFrontWeapon())
   {
      actor->GetFrontWeapon()->GetPartialUpdateProperties(propNamesToFill);
   }

   if (actor->GetBackWeapon())
   {
      actor->GetBackWeapon()->GetPartialUpdateProperties(propNamesToFill);
   }
}

////////////////////////////////////////////////////////////////////////////////
