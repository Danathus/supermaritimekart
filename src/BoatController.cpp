#include <BoatController.h>
#include <BoatKeyboardListener.h>
#include <DeltaOceanGetHeight.h>
#include <OceanWindowResize.h>
#include <SMKActorLibraryRegistry.h>
#include <SMKBoatActor.h>
#include <Weapon.h>
#include <WeaponMouseListener.h>

#include <dtActors/engineactorregistry.h>
#include <dtActors/playerstartactorproxy.h>
#include <dtCore/deltawin.h>
#include <dtCore/keyboard.h>
#include <dtCore/mouse.h>
#include <dtCore/transform.h>
#include <dtGame/messagetype.h>
#include <dtGame/basemessages.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/actorregistry.h>
# include <dtOcean/oceanactor.h>
#endif

#include <osgDB/ReadFile>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
BoatController::BoatController(dtCore::DeltaWin& win, dtCore::Keyboard& keyboard, dtCore::Mouse& mouse)
   : dtGame::GMComponent("BoatController")
   , mpKeyboardListener(new BoatKeyboardListener())
   , mpPrimaryMouseListener(new WeaponMouseListener())
   , mpSecondaryMouseListener(new WeaponMouseListener())
   , mpKeyboardToListenTo(&keyboard)
   , mpMouseToListenTo(&mouse)
   , mpOceanResizer(new OceanWindowResize())
{
   win.AddResizeCallback(*mpOceanResizer);
}

////////////////////////////////////////////////////////////////////////////////
BoatController::~BoatController()
{
   mpKeyboardListener->SetOutboard(NULL);
   mpMouseToListenTo->RemoveMouseListener(mpPrimaryMouseListener.get());
   mpMouseToListenTo->RemoveMouseListener(mpSecondaryMouseListener.get());
   mpMouseToListenTo = NULL;
   mpPrimaryMouseListener = NULL;
   mpSecondaryMouseListener = NULL;
   mpKeyboardToListenTo->RemoveKeyboardListener(mpKeyboardListener.get());
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

         static float delay = 0.0f;
         static const float kPeriod = 0.1f;
         delay += tickMsg.GetDeltaSimTime();

         if (delay > kPeriod)
         {
            //mpBoat->GetGameActorProxy().NotifyPartialActorUpdate();
            mpBoat->GetGameActorProxy().NotifyFullActorUpdate();
            delay -= kPeriod;
            if (!mpBoat->DynamicsEnabled())
            {
               mpBoat->EnableDynamics(true);
            }
         }

         //mpBoat->GetGameActorProxy().NotifyFullActorUpdate();
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
   mpBoat->SetupDefaultWeapon();

   // Setup BoatKeyboardListener
   mpKeyboardListener->SetOutboard(mpBoat->GetOutBoard());
   mpKeyboardToListenTo->AddKeyboardListener(mpKeyboardListener.get());

   // Setup WeaponMouseListener
   mpPrimaryMouseListener->SetWeapon(mpBoat->GetFrontWeapon());
   mpSecondaryMouseListener->SetWeapon(mpBoat->GetBackWeapon());
   mpMouseToListenTo->AddMouseListener(mpPrimaryMouseListener.get());
   mpMouseToListenTo->AddMouseListener(mpSecondaryMouseListener.get());

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
   mpKeyboardListener->SetOutboard(NULL);
   mpKeyboardToListenTo->RemoveKeyboardListener(mpKeyboardListener.get());
   mpMouseToListenTo->RemoveMouseListener(mpPrimaryMouseListener.get());
   mpMouseToListenTo->RemoveMouseListener(mpSecondaryMouseListener.get());

   mpBoat->SetGetHeight(NULL);
   mpBoat->EnableDynamics(false);
   mpBoat = NULL;
}

////////////////////////////////////////////////////////////////////////////////
