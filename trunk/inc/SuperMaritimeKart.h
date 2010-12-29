#ifndef SUPERMARITIMEKART_H
#define SUPERMARITIMEKART_H

#include <dtABC/application.h>
#include <NetSetGo/NetCore/Address.h>

////////////////////////////////////////////////////////////////////////////////

namespace dtGame
{
   class GameManager;
}
namespace dtDAL
{
   class BaseActorObject;
}

class BoatController;
class CameraController;
class FloaterController;
class FloatingActor;
class GameFinder;
class ImpactEffectsComponent;
class SMK_NetworkComponent;
class Netrospect;

////////////////////////////////////////////////////////////////////////////////

class SuperMaritimeKart : public dtABC::Application
{
   friend class SuperMaritimeKartMessenger;
public:
   SuperMaritimeKart(int argc, char** argv, const std::string& configFilename = "config.xml");

   virtual void Config();

   virtual void PreFrame(const double deltaFrameTime);
   virtual void PostFrame(const double deltaFrameTime);

   virtual bool KeyPressed(const dtCore::Keyboard* keyboard, int kc);
   virtual bool KeyReleased(const dtCore::Keyboard* keyboard, int kc);

protected:
   virtual ~SuperMaritimeKart();

   void OnMapLoaded();
   void OnMapUnloaded();

   void StartHosting();
   void ConnectToServer(const net::Address& serverAddress);

private:
   dtCore::RefPtr<dtGame::GameManager> mGameManager;
   GameFinder* mGameFinder;
   typedef std::vector<dtCore::RefPtr<dtDAL::BaseActorObject> > ProxyContainer;
   ProxyContainer mInstantiatedPrototypes;
   dtCore::RefPtr<SMK_NetworkComponent> mNetworkComponent;
   dtCore::RefPtr<SuperMaritimeKartMessenger> mAppComponent;
   dtCore::RefPtr<BoatController> mpBoatComponent;
   dtCore::RefPtr<CameraController> mCameraComponent;
   dtCore::RefPtr<FloaterController> mpFloaterComponent;
   dtCore::RefPtr<ImpactEffectsComponent> mpEffectsComponent;

   Netrospect* mNetrospect;

   bool mTryingToBeServer; ///<Are we attempting to be the server?
   net::Address mAddressToConnectTo; ///<The address the client is trying to connect to

   void AttachOceanHeightToBoat();

   void GetFloatingActors(std::vector<FloatingActor*>& floatingActors);
   void CreatePickUpItemHandleActors(); //instantiates all PickupItemHandle prototypes in the currently loaded map
   void RegisterFactoryTypes();///<register items for factories
};

////////////////////////////////////////////////////////////////////////////////

#endif // SUPERMARITIMEKART_H
