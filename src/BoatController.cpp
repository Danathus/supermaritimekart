#include <BoatController.h>
#include <BoatInputHandler.h>
#include <util/DeltaOceanGetHeight.h>
#include <OceanWindowResize.h>
#include <actors/SMKActorLibraryRegistry.h>
#include <actors/SMKBoatActor.h>
#include <actors/WeaponSlot.h>

#include <BoatActors/OutBoard.h>

#include <dtActors/engineactorregistry.h>
#include <dtActors/playerstartactorproxy.h>
#include <dtCore/deltawin.h>
#include <dtCore/keyboard.h>
#include <dtCore/mouse.h>
#include <dtCore/transform.h>
#include <dtGame/messagetype.h>
#include <dtGame/basemessages.h>
#include <dtGame/deadreckoningcomponent.h>
#include <dtUtil/mathdefines.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/actorregistry.h>
# include <dtOcean/oceanactor.h>
#endif

#include <osgDB/ReadFile>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
const double WHEN_TO_TURN_MIN = 10.0;
const double WHEN_TO_TURN_MAX = 30.0;
const double TURN_DURATION_MIN = 3.0;
const double TURN_DURATION_MAX = 6.0;

////////////////////////////////////////////////////////////////////////////////
BoatController::BoatController(dtCore::DeltaWin& win, dtCore::Keyboard& keyboard, dtCore::Mouse& mouse)
   : dtGame::GMComponent("BoatController")
   , mpInputHandler(new BoatInputHandler(&keyboard, &mouse))
   , mpKeyboardToListenTo(&keyboard)
   , mpMouseToListenTo(&mouse)
   , mpOceanResizer(new OceanWindowResize())
   , mInDemoMode(false)
{
   win.AddResizeCallback(*mpOceanResizer);
}

////////////////////////////////////////////////////////////////////////////////
BoatController::~BoatController()
{
   mpInputHandler->SetOutboard(NULL);
   mpMouseToListenTo = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void BoatController::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      dtOcean::OceanActor* ocean = GetOcean();
      mpOceanResizer->SetOceanActor(ocean);

      mpBoat = CreateBoatToControl();
      if (mpBoat != NULL)
      {
         SetupControlledBoat(ocean);
      }
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOAD_BEGIN)
   {
      if (mpBoat != NULL)
      {
         mpOceanResizer->SetOceanActor(NULL);
         CleanupControlledBoat();
      }
   }
   else if (message.GetMessageType() == dtGame::MessageType::TICK_LOCAL)
   {
      if (mpBoat)
      {
         const dtGame::TickMessage& tickMsg = static_cast<const dtGame::TickMessage&>(message);

         float dt = tickMsg.GetDeltaSimTime();
         static float delay = 0.0f;
         static const float kPeriod = 0.1f;
         delay += dt;

         if (delay > kPeriod)
         {
            //mpBoat->GetGameActorProxy().NotifyPartialActorUpdate();
            //mpBoat->GetGameActorProxy().NotifyFullActorUpdate(); // no longer do this ourselves
            delay -= kPeriod;
            if (!mpBoat->DynamicsEnabled())
            {
               mpBoat->EnableDynamics(true);
            }
         }

         //mpBoat->GetGameActorProxy().NotifyFullActorUpdate();
         mpInputHandler->Update();

         if (mInDemoMode)
         {
            UpdateBoat(dt);
         }
      }
   }

   //todo Something like this.  Should create a message, filled with properties
   //and blast it on the network.  Probably don't want to do this *every* frame
   //with all the properties...
   //else if message.GetMessageType() == ON_TICK_LOCAL...
   //mpBoat->NotifyFullActorUpdate();
}

////////////////////////////////////////////////////////////////////////////////
SMKBoatActor* BoatController::CreateBoatToControl()
{
   std::vector<dtDAL::ActorProxy*> boatPrototypes;
   GetGameManager()->FindPrototypesByActorType(*SMKActorLibraryRegistry::SMK_BOAT_ACTOR_TYPE, boatPrototypes);
   if (!boatPrototypes.empty())
   {
      dtCore::RefPtr<SMKBoatActorProxy> boatActor;
      GetGameManager()->CreateActorFromPrototype(boatPrototypes[0]->GetActor()->GetUniqueId(), boatActor);
      boatActor->RemoveProperty("Enable Dynamics"); // "ODE Enable Dynamics"
      if (boatActor.valid())
      {
         GetGameManager()->AddActor(*boatActor, false, true);
         return dynamic_cast<SMKBoatActor*>(boatActor->GetActor());
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
osg::Vec3 BoatController::GetStartLocation() const
{
   dtActors::PlayerStartActorProxy* spawnProxy;
   GetGameManager()->FindActorByType(*dtActors::EngineActorRegistry::PLAYER_START_ACTOR_TYPE, spawnProxy);
   if (spawnProxy)
   {
      dtActors::PlayerStartActor* startPoint(NULL);
      spawnProxy->GetActor(startPoint);
      dtCore::Transform xform;
      startPoint->GetTransform(xform);

      return xform.GetTranslation();
   }

   return osg::Vec3(0.0, 0.0, 0.0);
}

///////////////////////////////////////////////////////////////////////////////
dtOcean::OceanActor* BoatController::GetOcean() const
{
#ifdef BUILD_WITH_DTOCEAN
   //find any OceanActors;
   dtOcean::OceanActorProxy* oceanActorProxy(NULL);

   GetGameManager()->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
   dtOcean::OceanActor* oceanActor(NULL);
   oceanActorProxy->GetActor(oceanActor);

   assert(oceanActor != NULL);
   return oceanActor;
#else
   return NULL;
#endif
}

///////////////////////////////////////////////////////////////////////////////
void BoatController::SetupControlledBoat(dtOcean::OceanActor* ocean)
{
   // Setup BoatKeyboardListener
   mpInputHandler->SetOutboard(mpBoat->GetOutBoard());
   mpInputHandler->SetPrimaryWeapon(mpBoat->GetFrontWeapon());
   mpInputHandler->SetSecondaryWeapon(mpBoat->GetBackWeapon());

   // Setup Boat to float
   mpBoat->SetGetHeight(new DeltaOceanGetHeight(*ocean));
   mpBoat->EnableDynamics(false);

   const std::string altCollisionMeshFilename = mpBoat->GetAlternateCollisionMesh();
   if (!altCollisionMeshFilename.empty())
   {
      osg::Node* collisionMesh = osgDB::readNodeFile(altCollisionMeshFilename);
      if (collisionMesh)
      {
         mpBoat->SetCollisionMesh(collisionMesh);
      }
   }

   // Move boat to its initial location
   //dtCore::Transform boatTransform;
   //boatTransform.SetTranslation(GetStartLocation());
   //mpBoat->SetTransform(boatTransform);
}

///////////////////////////////////////////////////////////////////////////////
void BoatController::CleanupControlledBoat()
{
   mpInputHandler->SetOutboard(NULL);

   mpBoat->SetGetHeight(NULL);
   mpBoat->EnableDynamics(false);
   mpBoat = NULL;
}

////////////////////////////////////////////////////////////////////////////////
void BoatController::UpdateBoat(double deltaTime)
{
   mTimeSinceLastTurn += deltaTime;

   if (NeedToTurn())
   {
      mpBoat->GetOutBoard()->ActivateTurning(mTurnDirection);
   }
   else
   {
      mpBoat->GetOutBoard()->DeactivateTurning();
   }
}

////////////////////////////////////////////////////////////////////////////////
bool BoatController::NeedToTurn()
{
   if (CurrentlyTurning())
   {
      return true;
   }
   else if (TimeToStartTurning())
   {
      Reset();
      return true;
   }
   else
   {
      return false;
   }

}

////////////////////////////////////////////////////////////////////////////////
bool BoatController::CurrentlyTurning() const
{
   if (mTimeSinceLastTurn <= mTurnDuration)
   {
      return true;
   }
   else
   {
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
bool BoatController::TimeToStartTurning() const
{
   if (mTimeSinceLastTurn >= mWhenToTurnNext)
   {
      return true;
   }
   else
   {
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
void BoatController::Reset()
{
   mTimeSinceLastTurn = 0.0;
   mTurnDirection = dtUtil::RandRange(0,1) ? true : false;
   mWhenToTurnNext = dtUtil::RandFloat(WHEN_TO_TURN_MIN, WHEN_TO_TURN_MAX);
   mTurnDuration = dtUtil::RandFloat(TURN_DURATION_MIN, TURN_DURATION_MAX);
}

////////////////////////////////////////////////////////////////////////////////
/*
void WeaponMouseListener::UpdateLastMousePosition(float x, float y)
{
   FrontWeaponSlot* turret = dynamic_cast<FrontWeaponSlot*>(mpWeapon.get());
   if (turret != NULL)
   {
      Weapon* actor = turret->GetWeapon();
      dtCore::Transform weaponTransform;
      actor->GetTransform(weaponTransform, dtCore::Transformable::REL_CS);
      weaponTransform.SetRotation(-x * turret->GetHorizontalMaxAngle(),
         y * turret->GetVerticalMaxAngle(), 0);
      actor->SetTransform(weaponTransform, dtCore::Transformable::REL_CS);
   }
}
//*/
