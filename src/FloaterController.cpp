#include <FloaterController.h>
#include <DeltaOceanGetHeight.h>
#include <PickUpItemHandle.h>
#include <SMKActorLibraryRegistry.h>

#include <BoatActors/ActorLibraryRegistry.h>
#include <BoatActors/FloatingActor.h>

#include <dtGame/messagetype.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/actorregistry.h>
# include <dtOcean/oceanactor.h>
#endif

#include <osgDB/ReadFile>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
FloaterController::FloaterController()
: dtGame::GMComponent("FloaterController")
{
}

////////////////////////////////////////////////////////////////////////////////
FloaterController::~FloaterController()
{
}

////////////////////////////////////////////////////////////////////////////////
void FloaterController::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      // Get the ocean
      dtOcean::OceanActor* ocean = GetOcean();

      //find any FloatingActors
      std::vector<FloatingActor*> floaters = GetFloatingActors();

      //and give them a OceanGetHeight to use
      std::vector<FloatingActor*>::iterator floatItr;
      for (floatItr = floaters.begin(); floatItr != floaters.end(); ++floatItr)
      {
         SetupFloater(*floatItr, ocean);
      }
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOAD_BEGIN)
   {
      //find any FloatingActors
      std::vector<FloatingActor*> floaters = GetFloatingActors();

      //and clear their OceanGetHeight
      std::vector<FloatingActor*>::iterator floatItr;
      for (floatItr = floaters.begin(); floatItr != floaters.end(); ++floatItr)
      {
         CleanupFloater(*floatItr);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
std::vector<FloatingActor*> FloaterController::GetFloatingActors() const
{
   //find any FloatingActors
   std::vector<dtDAL::ActorProxy*> floatingActorProxies;
   std::vector<dtDAL::ActorProxy*> pickupActorProxies;
   GetGameManager()->FindActorsByType(*BoatActorsLibraryRegistry::FLOATING_ACTOR_TYPE, floatingActorProxies);
   GetGameManager()->FindActorsByType(*SMKActorLibraryRegistry::SMK_PICKUP_ACTOR_TYPE, pickupActorProxies);

   std::vector<FloatingActor*> floatingActors;
   std::vector<dtDAL::ActorProxy*>::iterator floatItr = floatingActorProxies.begin();
   while (floatItr != floatingActorProxies.end())
   {
      floatingActors.push_back(static_cast<FloatingActor*>((*floatItr)->GetActor()));
      ++floatItr;
   }

   floatItr = pickupActorProxies.begin();
   while (floatItr != pickupActorProxies.end())
   {
      floatingActors.push_back(static_cast<FloatingActor*>((*floatItr)->GetActor()));
      ++floatItr;
   }

   return floatingActors;
}

///////////////////////////////////////////////////////////////////////////////
dtOcean::OceanActor* FloaterController::GetOcean() const
{
#if BUILD_WITH_DTOCEAN
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
void FloaterController::SetupFloater(FloatingActor* floater, dtOcean::OceanActor* ocean)
{
   floater->SetGetHeight(new DeltaOceanGetHeight(*ocean));
   floater->EnableDynamics(true);

   const std::string altCollisionMeshFilename = floater->GetAlternateCollisionMesh();
   if (!altCollisionMeshFilename.empty())
   {
      osg::Node* collisionMesh = osgDB::readNodeFile(altCollisionMeshFilename);
      if (collisionMesh)
      {
         floater->SetCollisionMesh(collisionMesh);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void FloaterController::CleanupFloater(FloatingActor* floater)
{
   floater->SetGetHeight(NULL);
   floater->EnableDynamics(false);
}

////////////////////////////////////////////////////////////////////////////////
