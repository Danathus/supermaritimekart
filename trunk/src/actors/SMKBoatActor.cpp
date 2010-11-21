#include <actors/SMKBoatActor.h>
#include <actors/PickUpItemHandle.h>
#include <actors/FrontWeaponSlot.h>
#include <actors/BackWeaponSlot.h>
#include <messages/NetworkMessages.h>

#include <dtGame/basemessages.h>
#include <dtGame/gamemanager.h>
#include <dtGame/deadreckoninghelper.h>
#include <dtGame/drpublishingactcomp.h>
#include <dtGame/invokable.h>
#include <dtGame/messagetype.h>
#include <dtAudio/audiomanager.h>

#include <ode/contact.h>

using namespace SMK;

//////////////////////////////////////////////////////////
static const std::string FRONT_WEAPON_FIRED  = "FrontWeaponFired";
static const std::string BACK_WEAPON_FIRED   = "BackWeaponFired";
static const std::string BOAT_HIT            = "BoatHit";
static const std::string PROJECTILE_EXPLODED = "ProjectileExploded";

//////////////////////////////////////////////////////////
// Actor code
//////////////////////////////////////////////////////////
SMKBoatActor::SMKBoatActor(SMKBoatActorProxy& proxy)
   : BoatActor(proxy)
   , mpFrontWeapon(new FrontWeaponSlot())
   , mpBackWeapon(new BackWeaponSlot())
   , mDeadReckoningHelper(new dtGame::DeadReckoningHelper)
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
WeaponSlot* SMKBoatActor::GetFrontWeapon()
{
   return mpFrontWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
const WeaponSlot* SMKBoatActor::GetFrontWeapon() const
{
   return mpFrontWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
WeaponSlot* SMKBoatActor::GetBackWeapon()
{
   return mpBackWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
const WeaponSlot* SMKBoatActor::GetBackWeapon() const
{
   return mpBackWeapon.get();
}

////////////////////////////////////////////////////////////////////////////////
bool SMKBoatActor::FilterContact(dContact* contact, Transformable* collider)
{
   //osg::Vec3 position(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]);
   //osg::Vec3 normal(contact->geom.normal[0], contact->geom.normal[1], contact->geom.normal[2]);

   // Do not send events in STAGE.
   if (!GetGameActorProxy().IsInSTAGE())
   {
      PickUpItemHandle* pickup = dynamic_cast<PickUpItemHandle*>(collider);

      if (pickup && (DoWeWantThisPickUp(*pickup)))
      {
         dtGame::GameActorProxy& boatProxy = GetGameActorProxy();

         //make it inactive locally.  The server will provide the ultimate jugment later
         //pickup->SetActive(false);
         //pickup->SetCollisionDetection(false);

         // TEMP
         // This should happen when the item is acquired
         //{
         //   mPickupAcquireSound->Play();
         //}

         //Send request to server indicating our interest in acquiring "pickup"         
         dtCore::RefPtr<dtGame::Message> msg;
         GetGameActorProxy().GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::REQUEST_PICKUP_PICKUP, msg);
         msg->SetAboutActorId(pickup->GetUniqueId());

         //SendNetworkMessage() doesn't seem to do the job when no-one else is connected?
         GetGameActorProxy().GetGameManager()->SendMessage(*msg);  
      }
      else
      {
         return BoatActor::FilterContact(contact, collider);
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
   mPickupAcquireSound->SetListenerRelative(true);

   GetOSGNode()->setName(GetName());

   if (!IsRemote())
   {
      dtCore::Transform xform;
      GetTransform(xform);
      osg::Vec3 pos;
      xform.GetTranslation(pos);
      GetDeadReckoningHelper()->SetLastKnownTranslation(pos);
      osg::Vec3 rot;
      xform.GetRotation(rot);
      GetDeadReckoningHelper()->SetLastKnownRotation(rot);
   }

   //////////////////////////////
   // DR CONFIGURATION OPTIONS
   dtUtil::ConfigProperties& configParams = GetGameActorProxy().GetGameManager()->GetConfiguration();

   // Use Cubic Splines (vs the older Linear Blend) - If not specified, don't override default
   std::string useCubicSplines = "true"; //configParams.GetConfigPropertyValue("SimCore.DR.UseCubicSpline", "");
   if (useCubicSplines == "true" || useCubicSplines == "TRUE" || useCubicSplines == "1")
   {
      GetDeadReckoningHelper()->SetUseCubicSplineTransBlend(true);
   }
   else if (useCubicSplines == "false" || useCubicSplines == "FALSE" || useCubicSplines == "0")
   {
      GetDeadReckoningHelper()->SetUseCubicSplineTransBlend(false);
   }

   // Always Use Max Smoothing Time (as opposed to averaged update rate)
   // Some systems publish regularly, and some don't. If a system doesn't
   // publish updates like clockwork, then we use the average publish rate to blend. 
   std::string useFixedTimeBlends = ""; //configParams.GetConfigPropertyValue("SimCore.DR.UseFixedTimeBlends", "");
   if (useFixedTimeBlends == "true" || useFixedTimeBlends == "TRUE" || useFixedTimeBlends == "1")
   {
      GetDeadReckoningHelper()->SetUseFixedSmoothingTime(true);
   }
   else if (useFixedTimeBlends == "false" || useFixedTimeBlends == "FALSE" || useFixedTimeBlends == "0")
   {
      GetDeadReckoningHelper()->SetUseFixedSmoothingTime(false);
   }

   // The MaxTransSmoothingTime is usually set, but there are very obscure cases where it might
   // not have been set or not published for some reason. In that case, we need a non-zero value.
   // In practice, a vehicle that publishes will typically set these directly (for example, see 
   // BasePhysicsVehicleActor.SetMaxUpdateSendRate()). 
   // Previously, it set the smoothing time to 0.0 so that local actors would not smooth
   // their DR pos & rot to potentially make a cleaner comparison with less publishes.
   // Turning local smoothing on allows better vis & debugging of DR values (ex the DRGhostActor).
   if (GetDeadReckoningHelper()->GetMaxTranslationSmoothingTime() == 0.0f)
      GetDeadReckoningHelper()->SetMaxTranslationSmoothingTime(0.5f);
   if (GetDeadReckoningHelper()->GetMaxRotationSmoothingTime() == 0.0f)
      GetDeadReckoningHelper()->SetMaxRotationSmoothingTime(0.5f);

   SetupDefaultWeapon();
}


////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::OnRemovedFromWorld()
{
   //clear out our references so instances will be destroyed.
   BoatActor::OnRemovedFromWorld();

   mpFrontWeapon = NULL;
   mpBackWeapon = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::BuildActorComponents()
{
   BoatActor::BuildActorComponents();

   // DEAD RECKONING - ACT COMPONENT
   if (!HasComponent(dtGame::DeadReckoningHelper::TYPE)) // not added by a subclass
   {
      mDeadReckoningHelper = new dtGame::DeadReckoningHelper();

      // Flying was replaced with GroundClampType, and the default is already 'KeepAbove'
      //////mDeadReckoningHelper->SetFlying(false); // Causes ground clamping by default
      //mDeadReckoningHelper->SetGroundClampType(dtGame::GroundClampTypeEnum::KEEP_ABOVE);

      // attempt to fix the z-fighting on treads and wheels that are
      // very close to the ground. We move the vehicle up about 3-4 inches...
      mDeadReckoningHelper->SetGroundOffset(0.09f);

      AddComponent(*mDeadReckoningHelper);
   }
   else
   {
      GetComponent(mDeadReckoningHelper);
   }

   // DEAD RECKONING - PUBLISHING ACTOR COMPONENT
   if (!HasComponent(dtGame::DRPublishingActComp::TYPE)) // not added by a subclass
   {
      mDRPublishingActComp = new dtGame::DRPublishingActComp();
      AddComponent(*mDRPublishingActComp);  // Add AFTER the DRhelper.
   }
   else
   {
      GetComponent(mDRPublishingActComp);
   }

   //mDeadReckoningHelper->SetDeadReckoningAlgorithm(DeadReckoningAlgorithm::NONE);
   mDeadReckoningHelper->SetDeadReckoningAlgorithm(dtGame::DeadReckoningAlgorithm::VELOCITY_AND_ACCELERATION);
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::ProcessMessage(const dtGame::Message& message)
{
   BoatActor::ProcessMessage(message);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::BoatHit(const dtGame::Message& boatHitMessage)
{
   // Only apply damage if this message was from our unique ID
   if (boatHitMessage.GetAboutActorId() == GetUniqueId())
   {
      LOG_ALWAYS("We've been hit!");
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::Initialize()
{
   BoatActor::Initialize();

   RegisterGlobalBoatMessages();
   if (IsRemote())
   {
      RegisterRemoteBoatMessages();
   }
   else
   {
      RegisterLocalBoatMessages();
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::SetupDefaultWeapon()
{
   SMKBoatActorProxy* proxy = dynamic_cast<SMKBoatActorProxy*>(&GetGameActorProxy());
   if (proxy != NULL)
   {
      if (mpFrontWeapon)
      {
         mpFrontWeapon->Initialize(proxy);
      }

      if (mpBackWeapon)
      {
         mpBackWeapon->Initialize(proxy);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::RegisterGlobalBoatMessages()
{
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(BOAT_HIT,
      dtUtil::MakeFunctor(&SMKBoatActor::BoatHit, this)));

   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_BOAT_HIT, BOAT_HIT);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::RegisterRemoteBoatMessages()
{
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(FRONT_WEAPON_FIRED,
      dtUtil::MakeFunctor(&SMKBoatActor::FireFrontWeapon, this)));
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(BACK_WEAPON_FIRED,
      dtUtil::MakeFunctor(&SMKBoatActor::FireBackWeapon, this)));

   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_FRONT_WEAPON_FIRED, FRONT_WEAPON_FIRED);
   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_BACK_WEAPON_FIRED, BACK_WEAPON_FIRED);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::RegisterLocalBoatMessages()
{
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(PROJECTILE_EXPLODED,
      dtUtil::MakeFunctor(&SMKBoatActor::ProjectileExploded, this)));

   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_PROJECTILE_EXPLODED, PROJECTILE_EXPLODED);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::FireFrontWeapon(const dtGame::Message& weaponFiredMessage)
{
   // Only fire our weapon is this message was from our unique ID
   if (weaponFiredMessage.GetAboutActorId() == GetUniqueId())
   {
      mpFrontWeapon->GetWeapon()->FireWeapon();
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::FireBackWeapon(const dtGame::Message& weaponFiredMessage)
{
   // Only fire our weapon is this message was from our unique ID
   if (weaponFiredMessage.GetAboutActorId() == GetUniqueId())
   {
      mpBackWeapon->GetWeapon()->FireWeapon();
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::ProjectileExploded(const dtGame::Message& weaponFiredMessage)
{
   // Check to see if we are in the explosion's blast radius
   LOG_ALWAYS("Projectile exploded!");
}

////////////////////////////////////////////////////////////////////////////////
bool SMKBoatActor::DoWeWantThisPickUp(const PickUpItemHandle& pickup) const
{
   //pickup.GetType();
   return true;
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
void SMKBoatActorProxy::OnEnteredWorld()
{
   BoatActorProxy::OnEnteredWorld();
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

   // Add the properties for dead reckoning such as last known translation, etc...
   actor->GetDeadReckoningHelper()->GetPartialUpdateProperties(propNamesToFill);

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

void SMKBoatActorProxy::NotifyFullActorUpdate()
{
   // Remove the rot and trans from the full actor update.
   // If we send pos & rot out in an update, then that sometimes causes problems
   // on remote items. Network components usually pick up their data on tick local, which
   // sends a message to the DefaultmessageProcessorComponent. However, before that message
   // gets processed, the tick-remote gets picked up by the DeadReckoningComponent. Causes jumpiness.
   std::vector<dtDAL::ActorProperty* > allProperties;
   GetPropertyList(allProperties);

   std::vector<dtUtil::RefString> finalPropNameList;
   finalPropNameList.reserve(allProperties.size());

   for (size_t i = 0; i < allProperties.size(); ++i)
   {
      if (allProperties[i]->GetName() != dtDAL::TransformableActorProxy::PROPERTY_ROTATION &&
          allProperties[i]->GetName() != dtDAL::TransformableActorProxy::PROPERTY_TRANSLATION)
      {
         finalPropNameList.push_back(allProperties[i]->GetName());
      }
   }

   // Even though we are using the partialactorupdate method - it should be treated as a full
   NotifyPartialActorUpdate(finalPropNameList, false);
}

////////////////////////////////////////////////////////////////////////////////
