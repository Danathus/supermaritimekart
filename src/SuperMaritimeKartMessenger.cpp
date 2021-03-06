#include <SuperMaritimeKartMessenger.h>
#include <SuperMaritimeKart.h>

#include <dtGame/gamemanager.h>
#include <dtGame/message.h>
#include <dtGame/messagetype.h>
#include <dtUtil/exception.h>

using namespace dtCore;
using namespace dtABC;

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKartMessenger::SuperMaritimeKartMessenger(SuperMaritimeKart& application)
   : dtGame::GMComponent("SuperMaritimeKartComponent")
   , mApplication(&application)
{
}

////////////////////////////////////////////////////////////////////////////////
SuperMaritimeKartMessenger::~SuperMaritimeKartMessenger()
{
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKartMessenger::ProcessMessage(const dtGame::Message& message)
{
   if(message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      if (mApplication.valid())
      {
         mApplication->OnMapLoaded();
      }
      
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOADED)
   {
      if (mApplication.valid())
      {
         mApplication->OnMapUnloaded();
      }      
   }  
}

////////////////////////////////////////////////////////////////////////////////
void SuperMaritimeKartMessenger::OnRemovedFromGM()
{
   mApplication = NULL;
}

////////////////////////////////////////////////////////////////////////////////
