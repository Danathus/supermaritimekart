#ifndef WeaponMouseListener_h__
#define WeaponMouseListener_h__

////////////////////////////////////////////////////////////////////////////////
#include <dtCore/mouse.h>
namespace dtCore
{
   class Mouse;
}

class Weapon;

////////////////////////////////////////////////////////////////////////////////
class WeaponMouseListener : public dtCore::MouseListener
{
public:
   WeaponMouseListener();
   void SetWeapon(Weapon* weapon);
   Weapon* GetWeapon() { return mpWeapon.get(); }

   virtual bool HandleMouseScrolled(const dtCore::Mouse* mouse, int delta);
   virtual bool HandleButtonPressed(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button);
   virtual bool HandleButtonReleased(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button);
   virtual bool HandleButtonClicked(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button, int clickCount);
   virtual bool HandleMouseMoved(const dtCore::Mouse* mouse, float x, float y);
   virtual bool HandleMouseDragged(const dtCore::Mouse* mouse, float x, float y);

protected:
   virtual ~WeaponMouseListener();

private:
   void UpdateLastMousePosition(float x, float y);

   dtCore::RefPtr<Weapon> mpWeapon;
};
#endif // WeaponMouseListener_h__

////////////////////////////////////////////////////////////////////////////////
