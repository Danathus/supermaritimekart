#ifndef initcomponent_h__
#define initcomponent_h__

#include <dtGame/gmcomponent.h>
#include <dtCore/refptr.h>
#include <dtOcean/oceanactor.h>

namespace dtCore
{
   class Keyboard;
   class DeltaWin;
}

class BoatKeyboardListener;
class BoatActor;
class OceanWindowResize;


class InitComponent : public dtGame::GMComponent
{
public:
   InitComponent(dtCore::Keyboard& keyboard,
                 dtCore::DeltaWin& win);

   virtual void ProcessMessage(const dtGame::Message& message);

   void Reset();

   void AdjustOceanHeight(float adjust);
   float GetOceanHeight();

protected:
   virtual ~InitComponent();

private:
   BoatActor* GetBoatToControl() const;
   void MoveToSpawnPoint(BoatActor& boat) const;
   dtOcean::OceanActor* GetOceanActor() const;

   dtCore::RefPtr<BoatKeyboardListener> mKeyboardListener;
   dtCore::RefPtr<dtCore::Keyboard> mKeyboardToListenTo;

   dtCore::RefPtr<dtCore::DeltaWin> mWindow;

   dtCore::RefPtr<OceanWindowResize> mOceanResizeCB;
};
#endif //initcomponent_h__
