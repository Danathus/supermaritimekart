#ifndef BoatController_h__
#define BoatController_h__

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////
class BoatActor;
class BoatKeyboardListener;

namespace dtCore
{
   class Keyboard;
}

////////////////////////////////////////////////////////////////////////////////

class BoatController : public dtGame::GMComponent
{
public:
   BoatController(dtCore::Keyboard& keyboard);

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~BoatController();

private:
   BoatActor* GetBoatToControl() const;

   dtCore::RefPtr<BoatActor> mBoat;
   dtCore::RefPtr<BoatKeyboardListener> mKeyboardListener;
   dtCore::RefPtr<dtCore::Keyboard> mKeyboardToListenTo;
};

////////////////////////////////////////////////////////////////////////////////

#endif //BoatController_h__
