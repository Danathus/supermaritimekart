#ifndef BoatInputHandler_h__
#define BoatInputHandler_h__

#include <osg/Referenced>
#include <dtCore/refptr.h>

namespace dtCore
{
   class Keyboard;
   class Mouse;
   class LogicalInputDevice;
   class InputMapper;
}

class Outboard;
class WeaponSlot;

class BoatInputHandler : public osg::Referenced
{
public:
   BoatInputHandler(dtCore::Keyboard* keyboard, dtCore::Mouse* mouse);
   void SetOutboard(Outboard* outboard);
   void SetPrimaryWeapon(WeaponSlot* weapon) { mpPrimaryWeapon = weapon; }
   void SetSecondaryWeapon(WeaponSlot* weapon) { mpSecondaryWeapon = weapon; }
   Outboard* GetOutboard() { return mpOutboard.get(); }

   bool Update();

protected:
   virtual ~BoatInputHandler();

   void MapKeyboardControls();
   void MapGamepadControls();

private:
   dtCore::RefPtr<Outboard> mpOutboard;
   dtCore::RefPtr<WeaponSlot> mpPrimaryWeapon;
   dtCore::RefPtr<WeaponSlot> mpSecondaryWeapon;

   dtCore::Keyboard* mpKeyboard;

   /**
    * The application input device.
    */
   dtCore::RefPtr<dtCore::LogicalInputDevice> mApplicationInputDevice;

   /**
    * The keyboard and mouse input device.
    */
   dtCore::RefPtr<dtCore::LogicalInputDevice> mKeyboardMouseInputDevice;

   /**
    * The input mapper.
    */
   dtCore::RefPtr<dtCore::InputMapper> mInputMapper;
};
#endif // BoatInputHandler_h__
