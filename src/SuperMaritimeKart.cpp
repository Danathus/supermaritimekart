#include <SuperMaritimeKart.h>
#include <SuperMaritimeKartMessenger.h>
#include <BoatController.h>
#include <CameraController.h>
#include <FloaterController.h>
#include <NetConfig.h>
#include <SMK_BeaconData.h>
#include <GameFinder.h>
#include <ScenarioComponent.h>
#include <NetworkBuddy.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/system.h>
#include <dtCore/scene.h>
#include <dtCore/deltawin.h>
#include <dtGame/gamemanager.h>
#include <dtGame/defaultmessageprocessor.h>
#include <dtGame/defaultnetworkpublishingcomponent.h>
#include <dtUtil/exception.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/oceanactor.h>
# include <dtOcean/actorregistry.h>
#endif

#include <BoatActors/FloatingActor.h>
#include <BoatActors/ActorLibraryRegistry.h>

#include <NetCore/NetworkEngine.h>
#include <DeltaNetworkAdapter/NetworkMessages.h>
#include <SMK_NetworkComponent.h>

#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::SuperMaritimeKart(const std::string& configFilename)
   : Application(configFilename)
   , mGameFinder(NULL)
{
   // make sure the network engine gets started up early
   net::NetworkEngine::GetRef();
   // lets allow for extra big packets :P
   net::NetworkEngine::GetRef().GetNode().SetMaxPacketSize(2048);
}

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::~SuperMaritimeKart()
{
   if (mGameFinder)
   {
      delete mGameFinder;
      mGameFinder = NULL;
   }

   if (net::NetworkEngine::GetRef().GetNode().IsConnected())
   {
      printf("halting advertising of game\n");
      net::NetworkEngine::GetRef().StopAdvertising();

      printf("terminating self connection to game\n");
      net::NetworkEngine::GetRef().GetNode().Stop();

      printf("terminating hosting of game\n");
      net::NetworkEngine::GetRef().GetMesh().Stop();
   }

   dtAudio::AudioManager::Destroy();
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::Config()
{
   GetWindow()->SetWindowTitle("Super Maritime Kart");

   // This isn't equipped to be called a 2nd time
   if (mGameManager->GetProjectContext().empty())
   {
      Application::Config();

      dtAudio::AudioManager::Instantiate();
      GetCamera()->AddChild(dtAudio::AudioManager::GetListener());

      mGameManager = new dtGame::GameManager(*GetScene());
      mGameManager->SetApplication(*this);
      
      //don't need to do this anymore; it happens automatically.
      //DeltaNetworkAdapter::NetworkMessageType::RegisterMessageTypes(*mGameManager);

      try
      {
         mGameManager->SetProjectContext("./data", true);
         //mGameManager->ChangeMap(GetConfigPropertyValue("Map", "JustOcean"));

         SuperMaritimeKartMessenger* appComponent = new SuperMaritimeKartMessenger(*this);
         BoatController* boatComponent = new BoatController(*GetWindow(), *GetKeyboard());
         CameraController* cameraComponent = new CameraController(*GetCamera(),*GetKeyboard(), *GetMouse());
         FloaterController* floaterComponent = new FloaterController();

         mGameManager->AddComponent(*appComponent);
         mGameManager->AddComponent(*boatComponent);
         mGameManager->AddComponent(*cameraComponent);
         mGameManager->AddComponent(*floaterComponent);

         mGameManager->AddComponent(*new dtGame::DefaultMessageProcessor(), dtGame::GameManager::ComponentPriority::HIGHEST);
         mGameManager->AddComponent(*new dtGame::DefaultNetworkPublishingComponent());

         mGameManager->AddComponent(*new SMK_NetworkComponent());
         mGameManager->AddComponent(*new ScenarioComponent());

         GetScene()->SetPhysicsStepSize(0.001);
         GetScene()->SetGravity(0.f, 0.f, -18.f);
      }
      catch (const dtUtil::Exception& e)
      {
         e.LogException(dtUtil::Log::LOG_ERROR);
      }
   }

   // read timeout property
   {
      std::string defaultValue;
      {
         std::stringstream strstrm;
         strstrm << NetworkEngineComponent::GetDefaultNetworkTimeoutDuration();
         defaultValue = strstrm.str();
      }
      float timeoutValue;
      {
         std::stringstream strstrm(GetConfigPropertyValue("NetTimeout", defaultValue));
         strstrm >> timeoutValue;
         printf(">>> Net timeout property read as %f\n", timeoutValue);
      }
      net::NetworkEngine::GetRef().GetMesh().SetTimeout(timeoutValue);
      net::NetworkEngine::GetRef().GetNode().SetTimeout(timeoutValue);
   }

   dtCore::System::GetInstance().SetFrameRate(60.0);
   dtCore::System::GetInstance().SetMaxTimeBetweenDraws(.1);
   dtCore::System::GetInstance().SetUseFixedTimeStep(true);

   mGameFinder = new GameFinder();
   mGameFinder->Startup();
   printf(">>> searching for games...\n");
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

   if (mGameFinder->IsRunning())
   {
      // if we time out, stop listening and instead start hosting
      static float timeout = 3.0f;
      timeout -= deltaFrameTime;
      if (timeout <= 0.0f)
      {
         printf(">>> game search timed out\n");
         // stop scanning for new games to join
         mGameFinder->Shutdown();
         StartHosting();
      }
   }
   if (mGameFinder->Update(deltaFrameTime))
   {
      // try to see if we can update
      std::vector<std::string> gameList = mGameFinder->GenerateGameNameList();
      if (gameList.size() > 0)
      {
         // let's get the first one
         const GameFinder::GameDescription* selectedGame = mGameFinder->GetGame(gameList[0]);
         assert(selectedGame);

         // connect to the game
         printf(">>> game found; joining...\n");
         ConnectToServer(net::Address(selectedGame->mSenderAddress.GetAddress(), selectedGame->mPort));
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
bool SuperMaritimeKart::KeyPressed(const dtCore::Keyboard* keyboard, int kc)
{
   return false;
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::OnMapLoaded()
{
   // ...and let's start advertising on the network
   if (NetworkBuddy::GetRef().IsServer())
   {
      // put some extra info in the beacon packet
      BeaconData* userData = new BeaconData();
      if (userData)
      {
         userData->GetData().SetMapName(mGameManager->GetCurrentMap());
      }

      printf("advertising game via beacon on port %d (listen on port %d)\n", BEACON_SENDING_PORT, BEACON_LISTENER_PORT);
      net::NetworkEngine::GetRef().StartAdvertising(net::NetworkEngine::GetRef().GetHostName(), APP_PROTOCOL_ID, BEACON_LISTENER_PORT, GAME_HOST_PORT, BEACON_SENDING_PORT, userData);
   }
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::StartHosting()
{
   // start hosting game
   printf("hosting game on port %d\n", GAME_HOST_PORT);
   net::NetworkEngine::GetRef().GetMesh().Start(GAME_HOST_PORT);

   // let's connect to ourselves
   {
      printf("connecting to self...\n");
      net::Address hostAddress(127,0,0,1, GAME_HOST_PORT);

      // get our IP address on the network
      hostAddress = net::Address(net::NetworkEngine::GetRef().GetAddressFromHostName(net::NetworkEngine::GetRef().GetHostName()).GetAddress(), GAME_HOST_PORT);

      // begin to attempt connection
      for (int port = GAME_PLAYER_0_PORT; !net::NetworkEngine::GetRef().GetNode().Start(port); ++port) { }
      net::NetworkEngine::GetRef().GetNode().Connect(hostAddress);
      // wait until we're connected
      while (!net::NetworkEngine::GetRef().GetNode().IsConnected())
      {
         net::NetworkEngine::GetRef().Update(0.001f);
         // wait for a millisecond
         OpenThreads::Thread::microSleep(1000);
      }
      printf("...connected!\n");
   }

   mGameManager->ChangeMap(GetConfigPropertyValue("Map", "JustOcean"));
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::ConnectToServer(const net::Address& serverAddress)
{
   // stop scanning for new games to join
   mGameFinder->Shutdown();

   // connect to the selected game
   net::NetworkEngine& netEngine = net::NetworkEngine::GetRef();
   for (int port = GAME_PLAYER_0_PORT; !net::NetworkEngine::GetRef().GetNode().Start(port); ++port) { }
   net::NetworkEngine::GetRef().GetNode().Connect(serverAddress);
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

