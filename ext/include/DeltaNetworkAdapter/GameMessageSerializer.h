#ifndef GAME_MESSAGE_SERIALIZER__H
#define GAME_MESSAGE_SERIALIZER__H

////////////////////////////////////////////////////////////////////////////////

#include <dtCore/refptr.h>

#include <NetSetGo/NetCore/NodeID.h>

namespace dtUtil
{
   class DataStream;
};

namespace dtGame
{
   class GameManager;
   class Message;
}

class NetworkEngineComponent;

////////////////////////////////////////////////////////////////////////////////

class GameMessageSerializer
{
public:
   GameMessageSerializer();
   ~GameMessageSerializer();

   void SetGameManager(dtGame::GameManager* gameManager) { mpGameManager = gameManager; }
   dtGame::GameManager* GetGameManager() { return mpGameManager; }
   const dtGame::GameManager* GetGameManager() const { return mpGameManager; }

   void SetNetworkComponent(NetworkEngineComponent* netComponent) { mpNetComponent = netComponent; }
   NetworkEngineComponent* GetNetworkComponent() { return mpNetComponent; }
   const NetworkEngineComponent* GetNetworkComponent() const { return mpNetComponent; }

   bool SerializeMessage(dtUtil::DataStream& stream, const dtGame::Message* message, size_t maxPayloadSize) const;
   dtCore::RefPtr<dtGame::Message> DeserializeNetworkMessage(net::NodeID senderNodeID, dtUtil::DataStream& stream);

protected:
private:
   dtGame::GameManager* mpGameManager;
   NetworkEngineComponent* mpNetComponent;
};

////////////////////////////////////////////////////////////////////////////////

#endif // GAME_MESSAGE_SERIALIZER__H
