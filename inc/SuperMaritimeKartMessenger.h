#ifndef SUPERMARITIMEKART_COMPONENT_H
#define SUPERMARITIMEKART_COMPONENT_H

#include <dtGame/gmcomponent.h>
#include <dtCore/observerptr.h>

////////////////////////////////////////////////////////////////////////////////

class SuperMaritimeKart;

class SuperMaritimeKartMessenger : public dtGame::GMComponent
{
public:
   SuperMaritimeKartMessenger(SuperMaritimeKart& application);

   virtual void ProcessMessage(const dtGame::Message& message);

   virtual void OnRemovedFromGM();

protected:
   virtual ~SuperMaritimeKartMessenger();

private:
   dtCore::ObserverPtr<SuperMaritimeKart> mApplication;
};

////////////////////////////////////////////////////////////////////////////////

#endif // SUPERMARITIMEKART_COMPONENT_H

