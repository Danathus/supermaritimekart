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

namespace dtOcean
{
   class OceanActor;
}

////////////////////////////////////////////////////////////////////////////////

class BoatController : public dtGame::GMComponent
{
public:
   BoatController(dtCore::DeltaWin& win, dtCore::Keyboard& keyboard, dtCore::Mouse& mouse);

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
};

////////////////////////////////////////////////////////////////////////////////

#endif //BoatController_h__
