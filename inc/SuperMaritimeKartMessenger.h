#ifndef SUPERMARITIMEKART_COMPONENT_H
#define SUPERMARITIMEKART_COMPONENT_H

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////

class SuperMaritimeKart;

class SuperMaritimeKartMessenger : public dtGame::GMComponent
{
public:
   SuperMaritimeKartMessenger(SuperMaritimeKart& application);

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~SuperMaritimeKartMessenger();

private:
   dtCore::RefPtr<SuperMaritimeKart> mApplication;
};

////////////////////////////////////////////////////////////////////////////////

#endif // SUPERMARITIMEKART_COMPONENT_H

