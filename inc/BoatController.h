#ifndef BoatController_h__
#define BoatController_h__

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////
class BoatInputHandler;
class OceanWindowResize;
class SMKBoatActor;

namespace dtCore
{
   class DeltaWin;
   class Keyboard;
   class Mouse;
}

namespace dtGame
{
   class DeadReckoningComponent;
}

namespace dtOcean
{
   class OceanActor;
}

////////////////////////////////////////////////////////////////////////////////

class BoatController : public dtGame::GMComponent
{
public:
   BoatController(dtCore::DeltaWin& win, dtCore::Keyboard& keyboard, dtCore::Mouse& mouse);

   void SetDeadReckoningComponent(dtGame::DeadReckoningComponent* component) { mDeadReckoningComponent = component; }

   virtual void ProcessMessage(const dtGame::Message& message);

   void ToggleDemoMode() { mInDemoMode = !mInDemoMode; }

protected:
   virtual ~BoatController();

private:
   SMKBoatActor* CreateBoatToControl();
   osg::Vec3 GetStartLocation() const;
   dtOcean::OceanActor* GetOcean() const;

   void SetupControlledBoat(dtOcean::OceanActor* ocean);
   void CleanupControlledBoat();

   void UpdateBoat(double deltaTime);
   bool NeedToTurn();
   bool CurrentlyTurning() const;
   bool TimeToStartTurning() const;
   void Reset();

   dtCore::RefPtr<SMKBoatActor> mpBoat;
   dtCore::RefPtr<BoatInputHandler> mpInputHandler;
   dtCore::RefPtr<dtCore::Mouse> mpMouseToListenTo;
   dtCore::RefPtr<dtCore::Keyboard> mpKeyboardToListenTo;
   dtCore::RefPtr<OceanWindowResize> mpOceanResizer;
   dtCore::RefPtr<dtGame::DeadReckoningComponent> mDeadReckoningComponent;

   bool mInDemoMode;
   double mTimeSinceLastTurn;
   bool mTurnDirection; ///<True if the turn is to the left
   double mWhenToTurnNext; ///<When to take the next turn
   double mTurnDuration; ///<How long to turn;
};

////////////////////////////////////////////////////////////////////////////////

#endif //BoatController_h__
