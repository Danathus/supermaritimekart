#ifndef BoatController_h__
#define BoatController_h__

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////
class BoatActor;
class BoatKeyboardListener;
class OceanWindowResize;

namespace dtCore
{
   class DeltaWin;
   class Keyboard;
}

namespace dtOcean
{
   class OceanActor;
}

////////////////////////////////////////////////////////////////////////////////

class BoatController : public dtGame::GMComponent
{
public:
   BoatController(dtCore::DeltaWin& win, dtCore::Keyboard& keyboard);

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~BoatController();

private:
   BoatActor* CreateBoatToControl();
   osg::Vec3 GetStartLocation() const;
   dtOcean::OceanActor* GetOcean() const;

   void SetupControlledBoat(dtOcean::OceanActor* ocean);
   void CleanupControlledBoat();

   dtCore::RefPtr<BoatActor> mpBoat;
   dtCore::RefPtr<BoatKeyboardListener> mpKeyboardListener;
   dtCore::RefPtr<dtCore::Keyboard> mpKeyboardToListenTo;
   dtCore::RefPtr<OceanWindowResize> mpOceanResizer;
};

////////////////////////////////////////////////////////////////////////////////

#endif //BoatController_h__
