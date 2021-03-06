#include <actors/SMKBoatActor.h>
#include <actors/PickUpItemHandle.h>
#include <actors/FrontWeaponSlot.h>
#include <actors/BackWeaponSlot.h>
#include <actors/PickupItemFactory.h>
#include <actors/PickupItem.h>
#include <messages/DamageMessage.h>
#include <messages/NetworkMessages.h>
#include <DeltaNetworkAdapter/NetworkBuddy.h>
#include <util/Damage.h>
#include <util/DamageAssessor.h>

#include <BoatActors/Outboard.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/odebodywrap.h>
#include <dtCore/particlesystem.h>
#include <dtCore/shadermanager.h>
#include <dtCore/shaderparamfloat.h>
#include <dtCore/scene.h>
#include <dtCore/transform.h>
#include <dtGame/basemessages.h>
#include <dtGame/gamemanager.h>
#include <dtGame/deadreckoninghelper.h>
#include <dtGame/drpublishingactcomp.h>
#include <dtGame/invokable.h>
#include <dtGame/messagetype.h>
#include <dtUtil/mathdefines.h>

#include <ode/contact.h>

#include <assert.h>

using namespace SMK;

#define ENABLE_DAMAGE_SHADER 1

//////////////////////////////////////////////////////////
static const std::string FRONT_WEAPON_FIRED  = "FrontWeaponFired";
static const std::string BACK_WEAPON_FIRED   = "BackWeaponFired";
static const std::string BOAT_HIT            = "BoatHit";
static const std::string PROJECTILE_EXPLODED = "ProjectileExploded";
static const std::string BOAT_EXPLODED       = "BoatExploded";
static const std::string PICKUP_ACQUIRED     = "PickupAcquired";
static const std::string LOGNAME             = "SMKBoatActor";

//////////////////////////////////////////////////////////
// Actor code
//////////////////////////////////////////////////////////
SMKBoatActor::SMKBoatActor(SMKBoatActorProxy& proxy)
   : BoatActor(proxy)
   , mState(ALIVE)
   , mpFrontWeapon(new FrontWeaponSlot())
   , mpBackWeapon(new BackWeaponSlot())
   , mDeadReckoningHelper(new dtGame::DeadReckoningHelper)
   , mHealth(100, 100)
   , mArmor(SMK::Armor::ARMOR_NONE)
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

   switch (GetState())
   {
   case ALIVE:
      if (mpFrontWeapon)
      {
         mpFrontWeapon->Update(dt);
      }

      if (mpBackWeapon)
      {
         mpBackWeapon->Update(dt);
      }

      UpdateHealthShader(dt);
      break;
   case DEAD:
      mRespawnCountdown -= dt;
      if (mRespawnCountdown <= 0.0f)
      {
         RespawnBoat();
      }
      break;
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::TickRemote(const dtGame::Message& msg)
{
   BoatActor::TickRemote(msg);

   const dtGame::TickMessage& tickMessage = static_cast<const dtGame::TickMessage&>(msg);
   float dt = tickMessage.GetDeltaSimTime();

   switch (GetState())
   {
   case DEAD:
      mRespawnCountdown -= dt;
      if (mRespawnCountdown <= 0.0f)
      {
         RespawnBoat();
      }
      break;
   }
   //UpdateHealthShader(dt);
}

////////////////////////////////////////////////////////////////////////////////
//void SMKBoatActor::SetMesh(const std::string& meshFile)
//{
//
//}

////////////////////////////////////////////////////////////////////////////////
SMK::Health SMKBoatActor::GetHealth() const
{
   return mHealth;
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::SetHealth(const SMK::Health& health)
{
   mHealth = health;
   LOGN_DEBUG(LOGNAME,"Health level: " + dtUtil::ToString(mHealth.GetHealth()));
}

////////////////////////////////////////////////////////////////////////////////
SMK::Armor SMKBoatActor::GetArmor() const
{
   return mArmor;
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::SetArmor(const SMK::Armor& armor)
{
   mArmor = armor;
   LOGN_DEBUG(LOGNAME,"Armor type: " + dtUtil::ToString(mArmor.GetArmorType()) +
      ". Level: " + dtUtil::ToString(mArmor.GetHealth()));
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

   // Do not send events in STAGE or if we're a remote actor
   if (GetGameActorProxy().IsInSTAGE() || IsRemote())
   {
      return false;
   }

   PickUpItemHandle* pickup = dynamic_cast<PickUpItemHandle*>(collider);

   if (pickup && (DoWeWantThisPickUp(*pickup)))
   {
      dtGame::GameActorProxy& boatProxy = GetGameActorProxy();

      //TODO make it inactive locally.  The server will provide the ultimate jugment later
      //pickup->SetActive(false);
      //pickup->SetCollisionDetection(false);

      //Send request to server indicating our interest in acquiring "pickup"
      dtCore::RefPtr<dtGame::Message> msg;
      GetGameActorProxy().GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::REQUEST_PICKUP_PICKUP, msg);
      msg->SetAboutActorId(pickup->GetUniqueId());
      msg->SetSendingActorId(GetUniqueId());

      if (DeltaNetworkAdapter::NetworkBuddy::GetRef().IsServer())
      {
         //if we're the server and this is our local boat, send the message locally (?)
         GetGameActorProxy().GetGameManager()->SendMessage(*msg);
      }
      else
      {
         GetGameActorProxy().GetGameManager()->SendNetworkMessage(*msg);
      }
      return false;
   }

  return BoatActor::FilterContact(contact, collider);

}

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::OnEnteredWorld()
{
   BoatActor::OnEnteredWorld();

   // Setup pickup sound
   mPickupAcquireSound = dtAudio::AudioManager::GetInstance().NewSound();
   mPickupAcquireSound->LoadFile("sounds/pop.wav");
   mPickupAcquireSound->SetGain(1.0f);
   mPickupAcquireSound->SetListenerRelative(true);

   // Setup explosion particles
   mpExplosionParticles = new dtCore::ParticleSystem;
   mpExplosionParticles->LoadFile("particles/boat_explosion.osg");
   mpExplosionParticles->SetEnabled(false);
   GetGameActorProxy().GetGameManager()->GetScene().AddDrawable(mpExplosionParticles);

   GetOSGNode()->setName(GetName());

   // Retrieve the shader from the shader manager and assign it to this stateset
   dtCore::ShaderManager& shaderManager = dtCore::ShaderManager::GetInstance();
   const dtCore::ShaderProgram* prototypeProgram = shaderManager.FindShaderPrototype("Boat");
   mBoatProgram = shaderManager.AssignShaderFromPrototype(*prototypeProgram, *GetOSGNode());
   //shaderManager.AssignShaderFromPrototype(*prototypeProgram, *GetMeshNode());

   // hack for iitsec, remote boats don't respect the shader so manually put it on the mesh
   GetMeshNode()->getOrCreateStateSet()->setAttributeAndModes(mBoatProgram->GetShaderProgram(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
   assert(mBoatProgram);

   mHealthUniform = dynamic_cast<dtCore::ShaderParamFloat*>(mBoatProgram->FindParameter("health"));
   mHealthUniform->SetValue(1.0f);
   assert(mHealthUniform);

   //GetOSGNode()->getOrCreateStateSet()->setRenderingHint(osg::StateSet::OPAQUE_BIN);
   //GetOSGNode()->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");

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
   else
   {
      std::cout << "remotely entered world" << std::endl;
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
}


////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::OnRemovedFromWorld()
{
   UnregisterGlobalBoatMessages();
   if (IsRemote())
   {
      UnregisterRemoteBoatMessages();
   }
   else
   {
      UnregisterLocalBoatMessages();
   }

   mpFrontWeapon = NULL;
   mpBackWeapon = NULL;

   //clear out our references so instances will be destroyed.
   BoatActor::OnRemovedFromWorld();
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

   SMKBoatActorProxy* proxy = dynamic_cast<SMKBoatActorProxy*>(&GetGameActorProxy());
   if (mpFrontWeapon.valid())
   {
      mpFrontWeapon->Initialize(proxy);
   }

   if (mpBackWeapon.valid())
   {
      mpBackWeapon->Initialize(proxy);
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
         mpFrontWeapon->SetWeapon(mpFrontWeapon->GetDefaultWeapon(), &GetGameActorProxy());

         //Weapon* frontWeapon = mpFrontWeapon->GetWeapon();
         //osg::Node* frontWeaponNode = frontWeapon->GetOSGNode();
         //osg::StateSet* weaponState = frontWeaponNode->getOrCreateStateSet();
         //if (weaponState == NULL || mBoatProgram == NULL)
         //{
         //   LOG_ERROR("WeaponState is null");
         //}
         //else
         //{
         //   weaponState->setAttributeAndModes(mBoatProgram->GetShaderProgram(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
         //}
      }

      if (mpBackWeapon)
      {
         mpBackWeapon->SetWeapon(mpBackWeapon->GetDefaultWeapon(), &GetGameActorProxy());

         //osg::StateSet* weaponState = mpBackWeapon->GetWeapon()->GetOSGNode()->getOrCreateStateSet();
         //weaponState->setAttributeAndModes(mBoatProgram->GetShaderProgram(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::RegisterGlobalBoatMessages()
{
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(PICKUP_ACQUIRED,
      dtUtil::MakeFunctor(&SMKBoatActor::PickupAquired, this)));
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(BOAT_HIT,
      dtUtil::MakeFunctor(&SMKBoatActor::BoatHit, this)));
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(BOAT_EXPLODED,
      dtUtil::MakeFunctor(&SMKBoatActor::DieBoat, this)));

   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::INFO_PICKUP_ITEM_ACQUIRED, PICKUP_ACQUIRED);
   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_BOAT_HIT, BOAT_HIT);
   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_BOAT_EXPLODED, BOAT_EXPLODED);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::UnregisterGlobalBoatMessages()
{
   GetGameActorProxy().UnregisterForMessages(SMK::SMKNetworkMessages::INFO_PICKUP_ITEM_ACQUIRED, PICKUP_ACQUIRED);
   GetGameActorProxy().UnregisterForMessages(SMK::SMKNetworkMessages::ACTION_BOAT_HIT, BOAT_HIT);
   GetGameActorProxy().UnregisterForMessages(SMK::SMKNetworkMessages::ACTION_BOAT_EXPLODED, BOAT_EXPLODED);
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
void SMKBoatActor::UnregisterRemoteBoatMessages()
{
   GetGameActorProxy().UnregisterForMessages(SMK::SMKNetworkMessages::ACTION_FRONT_WEAPON_FIRED, FRONT_WEAPON_FIRED);
   GetGameActorProxy().UnregisterForMessages(SMK::SMKNetworkMessages::ACTION_BACK_WEAPON_FIRED, BACK_WEAPON_FIRED);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::RegisterLocalBoatMessages()
{
   GetGameActorProxy().AddInvokable(*new dtGame::Invokable(PROJECTILE_EXPLODED,
      dtUtil::MakeFunctor(&SMKBoatActor::ProjectileExploded, this)));

   GetGameActorProxy().RegisterForMessages(SMK::SMKNetworkMessages::ACTION_PROJECTILE_EXPLODED, PROJECTILE_EXPLODED);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::UnregisterLocalBoatMessages()
{
   GetGameActorProxy().UnregisterForMessages(SMK::SMKNetworkMessages::ACTION_PROJECTILE_EXPLODED, PROJECTILE_EXPLODED);
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
void SMKBoatActor::Teleport(const dtCore::Transform& transform)
{
   Outboard* outboard = GetOutBoard();
   if (outboard)
   {
      outboard->CutEngine();
   }

   GetBodyWrapper()->SetLinearVelocity(osg::Vec3(0.f,0.f,0.f));
   GetBodyWrapper()->SetAngularVelocity(osg::Vec3(0.f,0.f,0.f));
   SetTransform(transform);
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::BoatHit(const SMK::DamageMessage& boatHitMessage)
{
   // Only apply damage if this message was from our unique ID
   if (boatHitMessage.GetAboutActorId() == GetUniqueId())
   {
      ApplyDamage(boatHitMessage.GetDamage());
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::ProjectileExploded(const SMK::DamageMessage& weaponFiredMessage)
{
   // Apply damage function will determine if we take damage from this explosion
   ApplyDamage(weaponFiredMessage.GetDamage());
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::DieBoat(const dtGame::Message& weaponFiredMessage)
{
   if (GetState() == DEAD)
   {
      return;
   }

   if (weaponFiredMessage.GetAboutActorId() == GetUniqueId())
   {
      SetState(DEAD);
      mRespawnCountdown = 3.0f;

      // Play explosion particle
      dtCore::Transform currentTransform;
      GetTransform(currentTransform);
      mpExplosionParticles->SetTransform(currentTransform);
      mpExplosionParticles->ResetTime();
      mpExplosionParticles->SetEnabled(true);

      // cut engines
      GetOutBoard()->CutEngine();

      // flip out
      GetBodyWrapper()->SetLinearVelocity(osg::Vec3(0, 0, 0));
      GetBodyWrapper()->SetAngularVelocity(osg::Vec3(0, 0, 0));
      GetBodyWrapper()->ApplyRelTorque(osg::Vec3(0.0f, 1000000.0f, 0.0f));

      // Reset our health and weapons
      //mHealth.SetHealth(0);

      // Nulling so that the shading will be reinitialized
      //mpFrontWeapon = NULL;
      //mpBackWeapon = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::RespawnBoat()
{
   SetState(ALIVE);

   // Reset our health and weapons
   mHealth.SetHealth(mHealth.GetMax());

   SetupDefaultWeapon();

   // If this is our boat, move back to the respawn point
   if (!IsRemote())
   {
      // enable physics
      //EnableDynamics(true);

      SMKBoatActorProxy* prototypeProxy;
      GetGameActorProxy().GetGameManager()->FindPrototypeByName(GetPrototypeName(), prototypeProxy);
      dtCore::Transform transform;
      prototypeProxy->GetGameActor().GetTransform(transform);
      Teleport(transform);
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::ApplyDamage(const SMK::Damage& damage)
{
   // Setup our damage taker
   SMK::DamageTaker damageTaker;
   damageTaker.mTransformable = this;
   damageTaker.mArmor = &mArmor;
   damageTaker.mHealth = &mHealth;

   // Assess and apply the damage
   SMK::DamageAssessor assessor;
   assessor.Assess(damage, damageTaker);
   LOGN_DEBUG(LOGNAME, "My health is now: " + dtUtil::ToString(mHealth.GetHealth()));

   if (mHealth.GetHealth() <= 0 && !IsRemote())
   {
      dtCore::RefPtr<dtGame::Message> deathMsg;
      GetGameActorProxy().GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_BOAT_EXPLODED, deathMsg);
      deathMsg->SetAboutActorId(GetUniqueId());
      GetGameActorProxy().GetGameManager()->SendMessage(*deathMsg);
      GetGameActorProxy().GetGameManager()->SendNetworkMessage(*deathMsg);
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::UpdateHealthShader(float timeStep)
{
#if ENABLE_DAMAGE_SHADER
   float currentValue = mHealthUniform->GetValue();
   float targetValue = float(mHealth.GetHealth()) / float(mHealth.GetMax());

   float alpha = dtUtil::Min(timeStep * 5.0f, 1.0f);

   mHealthUniform->SetValue(currentValue + (targetValue - currentValue) * alpha);
#endif
}

////////////////////////////////////////////////////////////////////////////////
void SMKBoatActor::PickupAquired(const dtGame::MachineInfoMessage& pickupAcquiredMsg)
{
   if (pickupAcquiredMsg.GetAboutActorId() == GetUniqueId())
   {
      const dtCore::UniqueId& pickupUID = dtCore::UniqueId(pickupAcquiredMsg.GetUniqueId());
      //find the actual PickUpItemHandle to get it's data

      PickUpItemBaseProxy* pickupProxy(NULL);
      GetGameActorProxy().GetGameManager()->FindActorById(pickupUID, pickupProxy);
      if (pickupProxy)
      {
         const PickUpItemHandle* pickup = static_cast<const PickUpItemHandle*>(pickupProxy->GetActor());
         LOGN_DEBUG(LOGNAME, "acquired a pickup of type:" + pickup->GetType());

         //create the pickup via the pickup factory
         if ((pickup->GetPickupCategory() == PickupCategoryEnum::PICKUP_ARMOR) ||
            (pickup->GetPickupCategory() == PickupCategoryEnum::PICKUP_HEALTH))
         {
            const PickupItem* pickupItem = PickupItemFactory::GetInstance().Create(pickup->GetType());
            if (pickupItem)
            {
               //apply the pickup
               bool applied = pickupItem->Apply(*this);
            }
            else
            {
               LOGN_WARNING(LOGNAME, "PickupItemFactory couldn't create pickup of type: " + pickup->GetType());
            }
         }
         else
         {
            //maybe this is a Weapon instead?
            WeaponSlot* slot(NULL);
            if (pickup->GetPickupCategory() == PickupCategoryEnum::PICKUP_BOW_WEAPON)
            {
               slot = GetFrontWeapon();
            }
            else if (pickup->GetPickupCategory() == PickupCategoryEnum::PICKUP_TOP_WEAPON)
            {
               //slot = GetTopWeapon();
            }
            else if (pickup->GetPickupCategory() == PickupCategoryEnum::PICKUP_REAR_WEAPON)
            {
               slot = GetBackWeapon();
            }
            else if (pickup->GetPickupCategory() == PickupCategoryEnum::PICKUP_SIDE_WEAPON)
            {
               //slot = GetSideWeapon();
            }

            if (slot)
            {
               slot->SetWeapon(pickup->GetType(), &GetGameActorProxy());
            }
         }

         if (!IsRemote())
         {
            // aural feedback
            mPickupAcquireSound->Play();
         }
      }
   }
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
   BoatActorProxy::OnRemovedFromWorld();
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
