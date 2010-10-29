#include "BoatDemoMode.h"
#include <BoatActors/BoatActor.h>
#include <BoatActors/ActorLibraryRegistry.h>
#include <BoatActors/OutBoard.h>
#include <dtGame/basemessages.h>
#include <dtGame/messagetype.h>
#include <dtUtil/mathdefines.h>

const double WHEN_TO_TURN_MIN = 10.0;
const double WHEN_TO_TURN_MAX = 30.0;
const double TURN_DURATION_MIN = 3.0;
const double TURN_DURATION_MAX = 6.0;

//////////////////////////////////////////////////////////////////////////
BoatDemoMode::BoatDemoMode():
dtGame::GMComponent("BoatDemoMode"),
mEnabled(false),
mBoatToControl(NULL),
mTimeSinceLastTurn(0.0),
mTurnDirection(true),
mWhenToTurnNext(WHEN_TO_TURN_MIN),
mTurnDuration(TURN_DURATION_MIN)
{

}

//////////////////////////////////////////////////////////////////////////
BoatDemoMode::~BoatDemoMode()
{
   mBoatToControl = NULL;
}

//////////////////////////////////////////////////////////////////////////
void BoatDemoMode::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      mBoatToControl = GetBoatToControl();
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOADED)
   {
      mBoatToControl = NULL;
   }
   else if (message.GetMessageType() == dtGame::MessageType::TICK_LOCAL)
   {
      if (GetIsEnabled())
      {
         if (!mBoatToControl.valid())
         {
            mBoatToControl = GetBoatToControl();
         }

         if (mBoatToControl.valid())
         {
            const dtGame::TickMessage& mess = static_cast<const dtGame::TickMessage&>(message);

            UpdateBoat(mess.GetDeltaSimTime());
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
BoatActor* BoatDemoMode::GetBoatToControl() const
{
   //TODO duplicate of BoatFloaterComponent
   std::vector<dtDAL::ActorProxy*> boatActors;
   GetGameManager()->FindActorsByType(*BoatActorsLibraryRegistry::BOAT_ACTOR_TYPE, boatActors);
   if (!boatActors.empty())
   {
      return static_cast<BoatActor*>(boatActors[0]->GetActor());
   }
   else
   {
      return NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
void BoatDemoMode::SetEnabled(bool enabled)
{
   mEnabled = enabled;
   if (mEnabled)
   {
      Reset();
   }
}

////////////////////////////////////////////////////////////////////////////////
bool BoatDemoMode::GetIsEnabled() const
{ 
   return mEnabled;
}

////////////////////////////////////////////////////////////////////////////////
void BoatDemoMode::UpdateBoat(double deltaTime)
{
   mTimeSinceLastTurn += deltaTime;

   if (NeedToTurn())
   {
      mBoatToControl->GetOutBoard()->ActivateTurning(mTurnDirection);
   }
   else
   {
      mBoatToControl->GetOutBoard()->DeactivateTurning();
   }
}

////////////////////////////////////////////////////////////////////////////////
bool BoatDemoMode::NeedToTurn()
{
   if (CurrentlyTurning())
   {
      return true;
   }
   else if (TimeToStartTurning())
   {
      Reset();
      return true;
   }
   else
   {
      return false;
   }

}

////////////////////////////////////////////////////////////////////////////////
bool BoatDemoMode::CurrentlyTurning() const
{
   if (mTimeSinceLastTurn <= mTurnDuration)
   {
      return true;
   }
   else
   {
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
bool BoatDemoMode::TimeToStartTurning() const
{
   if (mTimeSinceLastTurn >= mWhenToTurnNext)
   {
      return true;
   }
   else
   {
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
void BoatDemoMode::Reset()
{
   mTimeSinceLastTurn = 0.0;
   mTurnDirection = dtUtil::RandRange(0,1) ? true : false;
   mWhenToTurnNext = dtUtil::RandFloat(WHEN_TO_TURN_MIN, WHEN_TO_TURN_MAX);
   mTurnDuration = dtUtil::RandFloat(TURN_DURATION_MIN, TURN_DURATION_MAX);
}


////////////////////////////////////////////////////////////////////////////////
void BoatDemoMode::OnAddedToGM()
{
   Reset();
}

////////////////////////////////////////////////////////////////////////////////
void BoatDemoMode::OnRemovedFromGM()
{
   mBoatToControl = NULL;
}
