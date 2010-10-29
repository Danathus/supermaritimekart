#ifndef boatdemomode_h__
#define boatdemomode_h__

#include <dtGame/gmcomponent.h>
#include <dtCore/RefPtr.h>

class BoatActor;


class BoatDemoMode : public dtGame::GMComponent
{
public:
   BoatDemoMode();
   virtual void ProcessMessage(const dtGame::Message& message);
   
   void SetEnabled(bool enabled);
   bool GetIsEnabled() const;

   virtual void OnAddedToGM();

   virtual void OnRemovedFromGM();

protected:
   virtual ~BoatDemoMode();
private:
   BoatActor* GetBoatToControl() const;
   void UpdateBoat(double deltaTime);
   bool NeedToTurn();
   bool CurrentlyTurning() const;
   bool TimeToStartTurning() const;
   void Reset();

   bool mEnabled;
   dtCore::RefPtr<BoatActor> mBoatToControl;
   double mTimeSinceLastTurn;
   bool mTurnDirection; ///<True if the turn is to the left
   double mWhenToTurnNext; ///<When to take the next turn
   double mTurnDuration; ///<How long to turn;

};
#endif //boatdemomode_h__
