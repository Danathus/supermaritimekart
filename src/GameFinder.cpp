#include <GameFinder.h>

#include <NetConfig.h>
#include <SMK_BeaconData.h>

#include <vector>

////////////////////////////////////////////////////////////////////////////////

// The make amount of time a game is alive for
const float GameFinder::GameDescription::kMaxLife = 1.0f;

////////////////////////////////////////////////////////////////////////////////
GameFinder::GameFinder()
   : mpBeaconReceiver(NULL)
{
   //
}

////////////////////////////////////////////////////////////////////////////////
const GameFinder::GameDescription* GameFinder::GetGame(const std::string& name) const
{
   GameDescription* selectedGame = NULL;

   // Get the selected game's server information
   GameNameToGameMap::const_iterator found = mGameNameToGameMap.find(name);
   if (found != mGameNameToGameMap.end())
   {
      selectedGame = found->second;
   }

   return selectedGame;
}

////////////////////////////////////////////////////////////////////////////////
void GameFinder::GetAllAvailableGames(std::vector<const GameDescription*>& gameList)
{
   gameList.clear();

   GamesFoundOnNetworkMap::iterator iter = mGamesFoundOnNetworkMap.begin();

   while (iter != mGamesFoundOnNetworkMap.end())
   {
      gameList.push_back(&iter->second);
      ++iter;
   }
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> GameFinder::GenerateGameNameList() const
{
   std::vector<std::string> gameNameList;

   for (GamesFoundOnNetworkMap::const_iterator i = mGamesFoundOnNetworkMap.begin(); i != mGamesFoundOnNetworkMap.end(); ++i)
   {
      const GameDescription& desc = i->second;

      gameNameList.push_back(desc.mName);
   }

   return gameNameList;
}

////////////////////////////////////////////////////////////////////////////////
void GameFinder::Startup()
{
   mpBeaconReceiver = new net::BeaconReceiver(BEACON_LISTENER_PORT, new BeaconData());
}

////////////////////////////////////////////////////////////////////////////////
void GameFinder::Shutdown()
{
   delete mpBeaconReceiver;
   mpBeaconReceiver = NULL;
}

////////////////////////////////////////////////////////////////////////////////
bool GameFinder::IsRunning() const
{
   const bool running = mpBeaconReceiver != NULL;
   return running;
}

////////////////////////////////////////////////////////////////////////////////
bool GameFinder::Update(float deltaTime)
{
   net::Address senderAddress;
   bool listingHasChanged = false;

   // check for incoming beacons
   while (mpBeaconReceiver && mpBeaconReceiver->ReceiveBeacon(senderAddress))
   {
      // received beacon!

      // add it to map if it's not already there
      GamesFoundOnNetworkMap::iterator gameFound = mGamesFoundOnNetworkMap.find(senderAddress);
      if (gameFound != mGamesFoundOnNetworkMap.end())
      {
         // re-found a game; refresh timeout
         GameDescription& desc = gameFound->second;
         desc.mLife = GameDescription::kMaxLife;
      }
      else
      {
         // found a new game; add to map and gui
         GameDescription& desc = mGamesFoundOnNetworkMap[senderAddress];
         desc.mSenderAddress = senderAddress;
         desc.mPort = mpBeaconReceiver->GetHeader().GetData().mServerPort;
         desc.mName = mpBeaconReceiver->GetHeader().GetData().mName;
         desc.mMapName = static_cast<BeaconData*>(mpBeaconReceiver->GetUserData())->GetData().mMapName;
         printf("detected map name: \"%s\"\n", desc.mMapName.c_str());
         desc.mLife = GameDescription::kMaxLife;
         mGameNameToGameMap[desc.mName] = &desc;

         // Make sure we update the UI for getting a new server
         listingHasChanged = true;
      }
   }

   // let stale games time out
   std::vector<GamesFoundOnNetworkMap::iterator> killList;
   for (GamesFoundOnNetworkMap::iterator i = mGamesFoundOnNetworkMap.begin(); i != mGamesFoundOnNetworkMap.end(); ++i)
   {
      GameDescription& desc = i->second;

      desc.mLife -= deltaTime;

      if (desc.mLife <= 0.0f)
      {
         // mark game for death
         killList.push_back(i);
      }
   }

   // Make sure we update the UI for losing a server
   if (killList.size())
   {
      listingHasChanged = true;
   }

   // kill games on kill list
   for (size_t i = 0; i < killList.size(); ++i)
   {
      GameDescription& desc = killList[i]->second;

      // remove from map
      mGamesFoundOnNetworkMap.erase(killList[i]);
   }

   return listingHasChanged;
}

////////////////////////////////////////////////////////////////////////////////
