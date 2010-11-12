#include <BoatController.h>
#include <BoatKeyboardListener.h>
#include <DeltaOceanGetHeight.h>
#include <OceanWindowResize.h>
#include <SMKActorLibraryRegistry.h>

#include <BoatActors/BoatActor.h>

#include <dtActors/engineactorregistry.h>
#include <dtActors/playerstartactorproxy.h>
#include <dtCore/deltawin.h>
#include <dtCore/keyboard.h>
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
BoatController::BoatController(dtCore::DeltaWin& win, dtCore::Keyboard& keyboard)
   : dtGame::GMComponent("BoatController")
   , mpKeyboardListener(new BoatKeyboardListener())
   , mpKeyboardToListenTo(&keyboard)
   , mpOceanResizer(new OceanWindowResize())
{
   win.AddResizeCallback(*mpOceanResizer);
}

////////////////////////////////////////////////////////////////////////////////
BoatController::~BoatController()
{
   mpKeyboardListener->SetOutboard(NULL);
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
            mpBoat->GetGameActorProxy().NotifyPartialActorUpdate();
            delay -= kPeriod;
            mpBoat->EnableDynamics(true);
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
BoatActor* BoatController::CreateBoatToControl()
{
   std::vector<dtDAL::ActorProxy*> boatPrototypes;
   GetGameManager()->FindPrototypesByActorType(*SMKActorLibraryRegistry::SMK_BOAT_ACTOR_TYPE, boatPrototypes);
   if (!boatPrototypes.empty())
   {
      dtCore::RefPtr<dtGame::GameActorProxy> boatActor;
      GetGameManager()->CreateActorFromPrototype(boatPrototypes[0]->GetActor()->GetUniqueId(), boatActor);
      if (boatActor.valid())
      {
         GetGameManager()->AddActor(*boatActor, false, true);
         return dynamic_cast<BoatActor*>(boatActor->GetActor());
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
   mpKeyboardListener->SetOutboard(mpBoat->GetOutBoard());
   mpKeyboardToListenTo->AddKeyboardListener(mpKeyboardListener.get());

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

   mpBoat->SetGetHeight(NULL);
   mpBoat->EnableDynamics(false);
   mpBoat = NULL;
}

////////////////////////////////////////////////////////////////////////////////
