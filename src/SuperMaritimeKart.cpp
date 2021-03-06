#include <SuperMaritimeKart.h>
#include <BoatController.h>
#include <CameraController.h>
#include <FloaterController.h>
#include <GameFinder.h>
#include <ImpactEffectsComponent.h>
#include <SMK_BeaconData.h>
#include <ScenarioComponent.h>
#include <SuperMaritimeKartMessenger.h>
#include <actors/SMKActorLibraryRegistry.h>
#include <actors/WeaponFactory.h>
#include <messages/NetworkMessages.h>
#include <NetConfig.h>
#include <DeltaNetworkAdapter/NetworkBuddy.h>
#include <actors/PickupItemFactory.h>
#include <actors/HealthPickup.h>
#include <actors/ArmorPickup.h>

#if BUILD_WITH_NETROSPECT
# include <netrospect.h>
#endif

#include <dtAudio/audiomanager.h>
#include <dtCore/system.h>
#include <dtCore/scene.h>
#include <dtCore/deltawin.h>
#include <dtCore/shadermanager.h>
#include <dtGame/gamemanager.h>
#include <dtGame/defaultmessageprocessor.h>
#include <dtGame/defaultnetworkpublishingcomponent.h>
#include <dtGame/deadreckoningcomponent.h>
#include <dtUtil/exception.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/oceanactor.h>
# include <dtOcean/actorregistry.h>
#endif

#include <BoatActors/FloatingActor.h>
#include <BoatActors/ActorLibraryRegistry.h>

#include <NetSetGo/NetCore/NetworkEngine.h>
#include <DeltaNetworkAdapter/NetworkMessages.h>
#include <SMK_NetworkComponent.h>
#include <CollisionCallback.h>
#include <assert.h>

#define CONNECT_ONLY_TO_ME 0 // commit as 0

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::SuperMaritimeKart(int argc, char** argv, const std::string& configFilename)
   : Application(configFilename)
   , mGameFinder(NULL)
   , mTryingToBeServer(false)
{
   WeaponFactory::GetInstance().RegisterInventoryItems();
   // make sure the network engine gets started up early
   net::NetworkEngine::GetRef();
   // lets allow for extra big packets :P
   net::NetworkEngine::GetRef().GetNode().SetMaxPacketSize(3 * 1024); // formerly 2048

#if BUILD_WITH_NETROSPECT
   mNetrospect = new Netrospect(argc, argv);
#endif
}

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKart::~SuperMaritimeKart()
{
#if BUILD_WITH_NETROSPECT
   delete mNetrospect;
   mNetrospect = NULL;
#endif


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

   mInstantiatedPrototypes.clear();
   if (mNetworkComponent)
   {
      mNetworkComponent->ClearNewClientPublishList();
      mGameManager->RemoveComponent(*mNetworkComponent);
   }

   mGameManager->RemoveComponent(*mAppComponent);
   mGameManager->RemoveComponent(*mpBoatComponent);
   mGameManager->RemoveComponent(*mCameraComponent);
   mGameManager->RemoveComponent(*mpFloaterComponent);
   mGameManager->RemoveComponent(*mpEffectsComponent);
   mAppComponent = NULL;
   mpBoatComponent = NULL;
   mCameraComponent = NULL;
   mpFloaterComponent = NULL;
   mpEffectsComponent = NULL;

   mGameManager->Shutdown();

   dtAudio::AudioManager::Destroy();
   SMK::PickupItemFactory::Destroy();
   WeaponFactory::Destroy();
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

      GetScene()->SetUserCollisionCallback(nearCallback, GetScene()->GetPhysicsController());

      mGameManager = new dtGame::GameManager(*GetScene());
      mGameManager->SetApplication(*this);

      try
      {
         mGameManager->SetProjectContext("./data", true);

         try
         {
            // Load in our shader definitions
            dtCore::ShaderManager::GetInstance().LoadShaderDefinitions("shaders/ShaderDefinitions.xml");
         }
         catch (dtUtil::Exception& e)
         {
            LOG_ERROR(e.ToString());
         }

         mAppComponent = new SuperMaritimeKartMessenger(*this);
         mpBoatComponent = new BoatController(*GetWindow(), *GetKeyboard(), *GetMouse());
         mCameraComponent = new CameraController(*GetCamera(),*GetKeyboard(), *GetMouse());
         mpFloaterComponent = new FloaterController();
         mpEffectsComponent = new ImpactEffectsComponent();

         mGameManager->AddComponent(*mAppComponent);
         mGameManager->AddComponent(*mpBoatComponent);
         mGameManager->AddComponent(*mCameraComponent);
         mGameManager->AddComponent(*mpFloaterComponent);
         mGameManager->AddComponent(*mpEffectsComponent);

         mGameManager->AddComponent(*new dtGame::DefaultMessageProcessor(), dtGame::GameManager::ComponentPriority::HIGHEST);
         mGameManager->AddComponent(*new dtGame::DefaultNetworkPublishingComponent());

         dtGame::DeadReckoningComponent* deadReckoningComponent = new dtGame::DeadReckoningComponent();
         mGameManager->AddComponent(*deadReckoningComponent);
         mpBoatComponent->SetDeadReckoningComponent(deadReckoningComponent);

         mNetworkComponent = new SMK_NetworkComponent();
         mGameManager->AddComponent(*mNetworkComponent);
         mGameManager->AddComponent(*new ScenarioComponent());

         GetScene()->SetPhysicsStepSize(0.001);
         GetScene()->SetGravity(0.f, 0.f, -18.f);
      }
      catch (const dtUtil::Exception& e)
      {
         e.LogException(dtUtil::Log::LOG_ERROR);
      }
   }

   RegisterFactoryTypes();

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

#if BUILD_WITH_NETROSPECT
   // Get the data from the GameFinder
   std::vector<const GameFinder::GameDescription*> gameList;
   mGameFinder->GetAllAvailableGames(gameList);

   std::vector<Netro::BeaconData> convertedList;

   for (size_t gameIndex = 0; gameIndex < gameList.size(); ++gameIndex)
   {
      Netro::BeaconData newData;
      newData.name = gameList[gameIndex]->mName;
      newData.senderAddress = gameList[gameIndex]->mSenderAddress;
      newData.port = gameList[gameIndex]->mPort;
      newData.map = gameList[gameIndex]->mMapName;
      newData.life = gameList[gameIndex]->mLife;

      convertedList.push_back(newData);
   }

   // Update the tool/UI
   mNetrospect->SetBeaconData(convertedList);
   mNetrospect->Update();
#endif
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
         mTryingToBeServer = true;

         //Load our map
         //TODO get this map name from somewhere else
         mGameManager->ChangeMap(GetConfigPropertyValue("Map", "JustOcean"));
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
#if CONNECT_ONLY_TO_ME
         // quick hack to limit connecting to only one specific IP
         if (selectedGame->mSenderAddress.GetAddress() != net::NetworkEngine::GetRef().GetAddressFromHostName(net::NetworkEngine::GetRef().GetHostName()).GetAddress())
         {
            return;
         }
#endif

         //start loading the map, given by the game finder
         mGameManager->ChangeMap(selectedGame->mMapName);

         // connect to the game
         printf(">>> game found; joining...\n");

         //save this address for later use
         mAddressToConnectTo = net::Address(selectedGame->mSenderAddress.GetAddress(), selectedGame->mPort);

         // stop scanning for new games to join
         mGameFinder->Shutdown();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
bool SuperMaritimeKart::KeyPressed(const dtCore::Keyboard* keyboard, int kc)
{
   static bool usingShaders = false;

   if (kc == 'g')
   {
      dtCore::ShaderManager& shaderManager = dtCore::ShaderManager::GetInstance();

      if (!usingShaders)
      {
         // Retrieve the shader from the shader manager and assign it to this stateset
         const dtCore::ShaderProgram* prototypeProgram = shaderManager.FindShaderPrototype("PerPixelLit");
         dtCore::ShaderProgram* program = shaderManager.AssignShaderFromPrototype(*prototypeProgram, *GetScene()->GetSceneNode());
         assert(program);
      }
      else
      {
         shaderManager.UnassignShaderFromNode(*GetScene()->GetSceneNode());
      }

      usingShaders = !usingShaders;
   }

   return false;
}

////////////////////////////////////////////////////////////////////////////////
bool SuperMaritimeKart::KeyReleased(const dtCore::Keyboard* keyboard, int kc)
{
   bool handled(true);
   switch(kc)
   {
   case 'c':
   case 'C':
      mCameraComponent->CycleCameraModes();
      break;
   case osgGA::GUIEventAdapter::KEY_Return:
      mpBoatComponent->ToggleDemoMode();
      break;
   case osgGA::GUIEventAdapter::KEY_Escape:
      Quit();
      break;
   default:
      handled = false;
      break;
   }

   return handled;
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::OnMapLoaded()
{
   if (mTryingToBeServer)//we're the server
   {
      StartHosting();
   }

   //wipe out our container of any previous instantiated Prototypes
   mInstantiatedPrototypes.clear();
   if (mNetworkComponent)
   {
      mNetworkComponent->ClearNewClientPublishList();
   }

   // ...and let's start advertising on the network
   if (DeltaNetworkAdapter::NetworkBuddy::GetRef().IsServer())
   {
      // put some extra info in the beacon packet
      BeaconData* userData = new BeaconData();
      if (userData)
      {
         userData->GetData().SetMapName(mGameManager->GetCurrentMap());
      }

      printf("advertising game via beacon on port %d (listen on port %d)\n", BEACON_SENDING_PORT, BEACON_LISTENER_PORT);
      net::NetworkEngine::GetRef().StartAdvertising(net::NetworkEngine::GetRef().GetHostName(), APP_PROTOCOL_ID, BEACON_LISTENER_PORT, GAME_HOST_PORT, BEACON_SENDING_PORT, userData);

      //now instantiate all prototype PickUps defined in the map
      CreatePickUpItemHandleActors();
   }
   else //we're a client
   {
      //connect to server
      ConnectToServer(mAddressToConnectTo);

      //Tell the server we finished loading our maps.  Now we can receive msgs
      //about actors!
      LOG_DEBUG("sending map loaded message to server");

      dtCore::RefPtr<dtGame::Message> msg;
      mGameManager->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::INFO_CLIENT_MAP_LOADED, msg);
      mGameManager->SendNetworkMessage(*msg);
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
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::ConnectToServer(const net::Address& serverAddress)
{
   // connect to the selected game
   net::NetworkEngine& netEngine = net::NetworkEngine::GetRef();
   for (int port = GAME_PLAYER_0_PORT; !net::NetworkEngine::GetRef().GetNode().Start(port); ++port) { }
   net::NetworkEngine::GetRef().GetNode().Connect(serverAddress);

   //wait until we're connected
   //TODO Probably need to break out of this after some period
   while (!net::NetworkEngine::GetRef().GetNode().IsConnected())
   {
      net::NetworkEngine::GetRef().Update(0.001f);
      // wait for a millisecond
      OpenThreads::Thread::microSleep(1000);
   }
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
void SuperMaritimeKart::CreatePickUpItemHandleActors()
{
   if (DeltaNetworkAdapter::NetworkBuddy::GetRef().IsServer() == false) { return; }

   //Assuming we're the server, lets create the PickUpItems defined in the map
   const dtGame::GameManager::NameVector& mapNames = mGameManager->GetCurrentMapSet();
   if (mapNames.empty()) {return;} //no maps loaded?

   std::vector<dtDAL::BaseActorObject*> proxiesToCreate;

   std::vector<dtDAL::BaseActorObject*> pickupProtoProxies;
   mGameManager->FindPrototypesByActorType(*SMKActorLibraryRegistry::SMK_PICKUP_ACTOR_TYPE, pickupProtoProxies);

   std::vector<dtDAL::BaseActorObject*> floatingProtoProxies;
   mGameManager->FindPrototypesByActorType(*BoatActorsLibraryRegistry::FLOATING_ACTOR_TYPE, floatingProtoProxies);

   proxiesToCreate.reserve(pickupProtoProxies.size() + floatingProtoProxies.size());
   proxiesToCreate.insert(proxiesToCreate.end(), pickupProtoProxies.begin(), pickupProtoProxies.end());
   proxiesToCreate.insert(proxiesToCreate.end(), floatingProtoProxies.begin(), floatingProtoProxies.end());

   std::vector<dtDAL::BaseActorObject*>::iterator itr = proxiesToCreate.begin();
   while (itr != proxiesToCreate.end())
   {
      dtCore::RefPtr<dtDAL::BaseActorObject> actorProxy = mGameManager->CreateActorFromPrototype((*itr)->GetActor()->GetUniqueId());
      dtGame::GameActorProxy* proxy = dynamic_cast<dtGame::GameActorProxy*>(actorProxy.get());
      if (proxy)
      {
         try
         {
            mGameManager->AddActor(*proxy, false, true);
            mInstantiatedPrototypes.push_back(proxy);
            mNetworkComponent->AddToNewClientPublishList(*proxy);
         }
         catch (const dtUtil::Exception& e)
         {
            LOG_ERROR("Problem adding the actor prototype to the GM. " + e.ToString());
         }
      }
      ++itr;
   }
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::OnMapUnloaded()
{
   //wipe out our container of any previous instantiated Prototypes
   mInstantiatedPrototypes.clear();

   if (mNetworkComponent)
   {
      mNetworkComponent->ClearNewClientPublishList();
   }

}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKart::RegisterFactoryTypes()
{
   SMK::PickupItemFactory& factory = SMK::PickupItemFactory::GetInstance();

   factory.RegisterType<SMK::HealthPickup>(SMK::HealthPickup::HEALTH_PICKUP_TYPE);
   factory.RegisterType<SMK::IronArmorPickup>(SMK::IronArmorPickup::IRON_ARMOR_PICKUP_TYPE);

}

////////////////////////////////////////////////////////////////////////////////

