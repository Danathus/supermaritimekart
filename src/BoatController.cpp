#include <BoatController.h>
#include <BoatKeyboardListener.h>
#include <DeltaOceanGetHeight.h>
#include <OceanWindowResize.h>

#include <BoatActors/ActorLibraryRegistry.h>
#include <BoatActors/BoatActor.h>

#include <dtCore/deltawin.h>
#include <dtCore/keyboard.h>
#include <dtGame/messagetype.h>

#include <dtOcean/actorregistry.h>
#include <dtOcean/oceanactor.h>

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

      mpBoat = GetBoatToControl();
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
}

////////////////////////////////////////////////////////////////////////////////
BoatActor* BoatController::GetBoatToControl() const
{
   dtDAL::ActorProxy* boatActor;
   GetGameManager()->FindActorByType(*BoatActorsLibraryRegistry::BOAT_ACTOR_TYPE, boatActor);
   if (boatActor)
   {
      return dynamic_cast<BoatActor*>(boatActor->GetActor());
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
dtOcean::OceanActor* BoatController::GetOcean() const
{
   //find any OceanActors;
   dtOcean::OceanActorProxy* oceanActorProxy(NULL);

   GetGameManager()->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
   dtOcean::OceanActor* oceanActor(NULL);
   oceanActorProxy->GetActor(oceanActor);

   assert(oceanActor != NULL);
   return oceanActor;
}

///////////////////////////////////////////////////////////////////////////////
void BoatController::SetupControlledBoat(dtOcean::OceanActor* ocean)
{
   // Setup BoatKeyboardListener
   mpKeyboardListener->SetOutboard(mpBoat->GetOutBoard());
   mpKeyboardToListenTo->AddKeyboardListener(mpKeyboardListener.get());

   // Setup Boat to float
   mpBoat->SetGetHeight(new DeltaOceanGetHeight(*ocean));
   mpBoat->EnableDynamics(true);

   const std::string altCollisionMeshFilename = mpBoat->GetAlternateCollisionMesh();
   if (!altCollisionMeshFilename.empty())
   {
      osg::Node* collisionMesh = osgDB::readNodeFile(altCollisionMeshFilename);
      if (collisionMesh)
      {
         mpBoat->SetCollisionMesh(collisionMesh);
      }
   }
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
