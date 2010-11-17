#ifndef BoatController_h__
#define BoatController_h__

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////
class BoatKeyboardListener;
class OceanWindowResize;
class SMKBoatActor;
class WeaponMouseListener;

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

protected:
   virtual ~BoatController();

private:
   SMKBoatActor* CreateBoatToControl();
   osg::Vec3 GetStartLocation() const;
   dtOcean::OceanActor* GetOcean() const;

   void SetupControlledBoat(dtOcean::OceanActor* ocean);
   void CleanupControlledBoat();

   dtCore::RefPtr<SMKBoatActor> mpBoat;
   dtCore::RefPtr<BoatKeyboardListener> mpKeyboardListener;
   dtCore::RefPtr<WeaponMouseListener> mpPrimaryMouseListener;
   dtCore::RefPtr<WeaponMouseListener> mpSecondaryMouseListener;
   dtCore::RefPtr<dtCore::Mouse> mpMouseToListenTo;
   dtCore::RefPtr<dtCore::Keyboard> mpKeyboardToListenTo;
   dtCore::RefPtr<OceanWindowResize> mpOceanResizer;
   dtCore::RefPtr<dtGame::DeadReckoningComponent> mDeadReckoningComponent;
};

////////////////////////////////////////////////////////////////////////////////

#endif //BoatController_h__
