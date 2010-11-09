#ifndef GAME_FINDER__H
#define GAME_FINDER__H

////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <vector>

#include <NetCore/Beacon.h>

////////////////////////////////////////////////////////////////////////////////

class GameFinder
{
public:
   struct GameDescription
   {
      static const float kMaxLife;

      net::Address mSenderAddress;
      int mPort; // port to connect to
      std::string mName;
      std::string mMapName;
      float mLife;
   };

   GameFinder();

   const GameDescription* GetGame(const std::string& name) const;
   std::vector<std::string> GenerateGameNameList() const;

   void Startup();
   void Shutdown();
   bool IsRunning() const;
   bool Update(float deltaTime); // returns true iff the listing has changed

protected:
private:
   // the lower-level network piece that receives the beacons
   net::BeaconReceiver* mpBeaconReceiver;

#pragma warning (push)
#pragma warning (disable:4251)

   typedef std::map<net::Address, GameDescription> GamesFoundOnNetworkMap;
   GamesFoundOnNetworkMap mGamesFoundOnNetworkMap;

   typedef std::map<std::string, GameDescription*> GameNameToGameMap;
   GameNameToGameMap mGameNameToGameMap;

#pragma warning (pop)

};

////////////////////////////////////////////////////////////////////////////////

#endif // GAME_FINDER__H
