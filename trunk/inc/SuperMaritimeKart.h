#ifndef SUPERMARITIMEKART_H
#define SUPERMARITIMEKART_H

#include <dtABC/application.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}

namespace net
{
   class Address;
}

class FloatingActor;
class GameFinder;

////////////////////////////////////////////////////////////////////////////////

class SuperMaritimeKart : public dtABC::Application
{
   friend class SuperMaritimeKartMessenger;
public:
   SuperMaritimeKart(const std::string& configFilename = "config.xml");

   virtual void Config();

   virtual void PreFrame(const double deltaFrameTime);
   virtual void PostFrame(const double deltaFrameTime);

   virtual bool KeyPressed(const dtCore::Keyboard* keyboard, int kc);

protected:
   virtual ~SuperMaritimeKart();

   void OnMapLoaded();
   void StartHosting();
   void ConnectToServer(const net::Address& serverAddress);

private:
   dtCore::RefPtr<dtGame::GameManager> mGameManager;
   GameFinder* mGameFinder;

   void AttachOceanHeightToBoat();

   void GetFloatingActors(std::vector<FloatingActor*>& floatingActors);
};

////////////////////////////////////////////////////////////////////////////////

#endif // SUPERMARITIMEKART_H
