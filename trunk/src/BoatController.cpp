#include <BoatController.h>
#include <BoatActors/BoatActor.h>
#include <BoatKeyboardListener.h>

#include <BoatActors/ActorLibraryRegistry.h>

#include <dtCore/keyboard.h>
#include <dtGame/messagetype.h>

////////////////////////////////////////////////////////////////////////////////
BoatController::BoatController(dtCore::Keyboard& keyboard)
: dtGame::GMComponent("BoatController")
, mKeyboardListener(new BoatKeyboardListener())
, mKeyboardToListenTo(&keyboard)
{
}

////////////////////////////////////////////////////////////////////////////////
BoatController::~BoatController()
{
   mKeyboardListener->SetOutboard(NULL);
   mKeyboardToListenTo->RemoveKeyboardListener(mKeyboardListener.get());
}

////////////////////////////////////////////////////////////////////////////////
void BoatController::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      mBoat = GetBoatToControl();
      if (mBoat != NULL)
      {
         mKeyboardListener->SetOutboard(mBoat->GetOutBoard());
         mKeyboardToListenTo->AddKeyboardListener(mKeyboardListener.get());
      }
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOAD_BEGIN)
   {
      mKeyboardListener->SetOutboard(NULL);
      mKeyboardToListenTo->RemoveKeyboardListener(mKeyboardListener.get());
      mBoat = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
BoatActor* BoatController::GetBoatToControl() const
{
   dtDAL::ActorProxy* boatActor;
   GetGameManager()->FindActorByType(*BoatActorsLibraryRegistry::BOAT_ACTOR_TYPE, boatActor);
   if (boatActor)
   {
      return dynamic_cast<BoatActor*>(boatActor->GetActor());
   }
   else
   {
      return NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
