#include <SuperMaritimeKart.h>
#include <SuperMaritimeKartMessenger.h>
#include <BoatController.h>
#include <CameraController.h>
#include <FloaterController.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/scene.h>
#include <dtGame/gamemanager.h>
#include <dtUtil/exception.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/oceanactor.h>
# include <dtOcean/actorregistry.h>
#endif

#include <BoatActors/FloatingActor.h>
#include <BoatActors/ActorLibraryRegistry.h>

#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::SuperMaritimeKart(const std::string& configFilename)
: Application(configFilename)
{
}

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::~SuperMaritimeKart()
{
   dtAudio::AudioManager::Destroy();
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::Config()
{
   // This isn't equipped to be called a 2nd time
   if (mGameManager->GetProjectContext().empty())
   {
      Application::Config();

      dtAudio::AudioManager::Instantiate();
      GetCamera()->AddChild(dtAudio::AudioManager::GetListener());

      mGameManager = new dtGame::GameManager(*GetScene());
      mGameManager->SetApplication(*this);

      try
      {
         mGameManager->SetProjectContext("./data", true);
         mGameManager->ChangeMap(GetConfigPropertyValue("Map", "JustOcean"));

         SuperMaritimeKartMessenger* appComponent = new SuperMaritimeKartMessenger(*this);
         BoatController* boatComponent = new BoatController(*GetWindow(), *GetKeyboard());
         CameraController* cameraComponent = new CameraController(*GetCamera(),*GetKeyboard(), *GetMouse());
         FloaterController* floaterComponent = new FloaterController();

         mGameManager->AddComponent(*appComponent);
         mGameManager->AddComponent(*boatComponent);
         mGameManager->AddComponent(*cameraComponent);
         mGameManager->AddComponent(*floaterComponent);

         GetScene()->SetPhysicsStepSize(0.001);
         GetScene()->SetGravity(0.f, 0.f, -18.f);
      }
      catch (const dtUtil::Exception& e)
      {
         e.LogException(dtUtil::Log::LOG_ERROR);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::PreFrame(const double deltaFrameTime)
{
   dtABC::Application::PreFrame(deltaFrameTime);
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::PostFrame(const double deltaFrameTime)
{
   dtABC::Application::PostFrame(deltaFrameTime);
}

////////////////////////////////////////////////////////////////////////////////
bool SuperMaritimeKart::KeyPressed(const dtCore::Keyboard* keyboard, int kc)
{
   return false;
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::OnMapLoaded()
{

}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::AttachOceanHeightToBoat()
{
#ifdef BUILD_WITH_DTOCEAN
   //find any OceanActors;
   dtOcean::OceanActorProxy* oceanActorProxy(NULL);

   mGameManager->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
   dtOcean::OceanActor* oceanActor(NULL);
   oceanActorProxy->GetActor(oceanActor);

   assert(oceanActor != NULL);

   //find any FloatingActors
   std::vector<FloatingActor*> boats;
   GetFloatingActors(boats);

   ////and give them a OceanGetHeight to use
   //std::vector<FloatingActor*>::iterator boatItr = boats.begin();
   //while (boatItr != boats.end())
   //{
   //   (*boatItr)->SetGetHeight(new OsgOceanGetHeight(*oceanActor));
   //   (*boatItr)->EnableDynamics(true);

   //   const std::string altCollisionMeshFilename = (*boatItr)->GetAlternateCollisionMesh();
   //   if (!altCollisionMeshFilename.empty())
   //   {
   //      osg::Node* collisionMesh = osgDB::readNodeFile(altCollisionMeshFilename);
   //      if (collisionMesh)
   //      {
   //         (*boatItr)->SetCollisionMesh(collisionMesh);
   //      }
   //   }

   //   ++boatItr;
   //}
#endif
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::GetFloatingActors(std::vector<FloatingActor*>& floatingActors)
{
   floatingActors.clear();

   //find any FloatingActors
   std::vector<dtDAL::ActorProxy*> floatingActorProxies;
   std::vector<dtDAL::ActorProxy*> boatActorProxies;
   mGameManager->FindActorsByType(*BoatActorsLibraryRegistry::FLOATING_ACTOR_TYPE, floatingActorProxies);
   mGameManager->FindActorsByType(*BoatActorsLibraryRegistry::BOAT_ACTOR_TYPE, boatActorProxies);

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
}




////////////////////////////////////////////////////////////////////////////////

