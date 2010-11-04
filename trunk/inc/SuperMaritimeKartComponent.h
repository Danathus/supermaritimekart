#ifndef SUPERMARITIMEKART_COMPONENT_H
#define SUPERMARITIMEKART_COMPONENT_H

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////

class SuperMaritimeKart;

class SuperMaritimeKartComponent : public dtGame::GMComponent
{
public:
   SuperMaritimeKartComponent(SuperMaritimeKart& application);

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~SuperMaritimeKartComponent();

private:
   dtCore::RefPtr<SuperMaritimeKart> mApplication;
};

////////////////////////////////////////////////////////////////////////////////

#endif // SUPERMARITIMEKART_COMPONENT_H

