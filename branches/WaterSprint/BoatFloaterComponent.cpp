#include "BoatFloaterComponent.h"
#include "OsgOceanGetHeight.h"
#include <BoatActors/FloatingActor.h>
#include <BoatActors/ActorLibraryRegistry.h>
#include <dtOcean/actorregistry.h>
#include <cassert>
#include <osgDB/ReadFile>
#include <dtGame/message.h>
#include <dtGame/messagetype.h>
////////////////////////////////////////////////////////////////////////////////
BoatFloaterComponent::BoatFloaterComponent():
dtGame::GMComponent("BoatFloaterComponent")
{

}

////////////////////////////////////////////////////////////////////////////////
BoatFloaterComponent::~BoatFloaterComponent()
{

}

////////////////////////////////////////////////////////////////////////////////
void BoatFloaterComponent::ProcessMessage(const dtGame::Message& message)
{
   if(message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      //find any OceanActors;
      dtOcean::OceanActorProxy* oceanActorProxy(NULL);

      GetGameManager()->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
      dtOcean::OceanActor* oceanActor(NULL);
      oceanActorProxy->GetActor(oceanActor);

      assert(oceanActor != NULL);

      //find any FloatingActors
      std::vector<FloatingActor*> boats = GetFloatingActors();

      //and give them a OceanGetHeight to use
      std::vector<FloatingActor*>::iterator boatItr = boats.begin();
      while (boatItr != boats.end())
      {
         (*boatItr)->SetGetHeight(new OsgOceanGetHeight(*oceanActor));
         (*boatItr)->EnableDynamics(true);

         const std::string altCollisionMeshFilename = (*boatItr)->GetAlternateCollisionMesh();
         if (!altCollisionMeshFilename.empty())
         {
            osg::Node* collisionMesh = osgDB::readNodeFile(altCollisionMeshFilename);
            if (collisionMesh)
            {
               (*boatItr)->SetCollisionMesh(collisionMesh);
            }
         }

         ++boatItr;
      }
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOAD_BEGIN)
   {
      std::vector<FloatingActor*> boats = GetFloatingActors();

      //and give them a OceanGetHeight to use
      std::vector<FloatingActor*>::iterator boatItr = boats.begin();
      while (boatItr != boats.end())
      {
         (*boatItr)->SetGetHeight(NULL);
         (*boatItr)->EnableDynamics(false);

         ++boatItr;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
std::vector<FloatingActor*> BoatFloaterComponent::GetFloatingActors() const
{
   //TODO duplicate of InitComponent
   //find any FloatingActors
   std::vector<dtDAL::ActorProxy*> floatingActorProxies;
   std::vector<dtDAL::ActorProxy*> boatActorProxies;
   GetGameManager()->FindActorsByType(*BoatActorsLibraryRegistry::FLOATING_ACTOR_TYPE, floatingActorProxies);
   GetGameManager()->FindActorsByType(*BoatActorsLibraryRegistry::BOAT_ACTOR_TYPE, boatActorProxies);
   
   std::vector<FloatingActor*> floatingActors;

   {
      std::vector<dtDAL::ActorProxy*>::iterator floatItr = floatingActorProxies.begin();
      while (floatItr != floatingActorProxies.end())
      {
         floatingActors.push_back(static_cast<FloatingActor*>((*floatItr)->GetActor()));
         ++floatItr;
      }
   }

   {
      std::vector<dtDAL::ActorProxy*>::iterator boatItr = boatActorProxies.begin();
      while (boatItr != boatActorProxies.end())
      {
         floatingActors.push_back(static_cast<FloatingActor*>((*boatItr)->GetActor()));
         ++boatItr;
      }
   }
   return floatingActors;
}
