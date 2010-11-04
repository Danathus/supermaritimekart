#include <SuperMaritimeKartComponent.h>
#include <SuperMaritimeKart.h>

#include <dtGame/gamemanager.h>
#include <dtGame/message.h>
#include <dtGame/messagetype.h>
#include <dtUtil/exception.h>

using namespace dtCore;
using namespace dtABC;

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKartComponent::SuperMaritimeKartComponent(SuperMaritimeKart& application)
: dtGame::GMComponent("SuperMaritimeKartComponent")
, mApplication(&application)
{
}

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKartComponent::~SuperMaritimeKartComponent()
{
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKartComponent::ProcessMessage(const dtGame::Message& message)
{
   if(message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      mApplication->OnMapLoaded();
   }
}

////////////////////////////////////////////////////////////////////////////////

