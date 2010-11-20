#include <WeaponMouseListener.h>
#include <actors/FrontWeaponSlot.h>
#include <actors/WeaponSlot.h>
#include <actors/Weapon.h>

#include <dtCore/transform.h>
#include <dtUtil/mathdefines.h>

////////////////////////////////////////////////////////////////////////////////
WeaponMouseListener::WeaponMouseListener()
{
}

////////////////////////////////////////////////////////////////////////////////
WeaponMouseListener::~WeaponMouseListener()
{
}

///////////////////////////////////////////////////////////////////////////////
void WeaponMouseListener::SetWeapon(WeaponSlot* weapon)
{
   mpWeapon = weapon;
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleMouseScrolled(const  dtCore::Mouse* mouse, int delta)
{
   return false;
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleButtonPressed(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button)
{
   FrontWeaponSlot* turret = dynamic_cast<FrontWeaponSlot*>(mpWeapon.get());
   if ((turret != NULL && button == dtCore::Mouse::LeftButton) ||
      ( turret == NULL && button == dtCore::Mouse::RightButton))
   {
      mpWeapon->StartWeaponFire();
      return true;
   }
   return false;  
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleButtonReleased(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button)
{
   FrontWeaponSlot* turret = dynamic_cast<FrontWeaponSlot*>(mpWeapon.get());
   if ((turret != NULL && button == dtCore::Mouse::LeftButton) ||
      ( turret == NULL && button == dtCore::Mouse::RightButton))
   {
      mpWeapon->StopWeaponFire();
      return true;
   }
   return false;  
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleButtonClicked(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button, int clickCount)
{
   return false;  
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleMouseMoved(const dtCore::Mouse* mouse, float x, float y)
{
   //UpdateLastMousePosition(x, y);

   return false;
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleMouseDragged(const dtCore::Mouse* mouse, float x, float y)
{
   //UpdateLastMousePosition(x, y);

   return false;  
}

///////////////////////////////////////////////////////////////////////////////
void WeaponMouseListener::UpdateLastMousePosition(float x, float y)
{
   FrontWeaponSlot* turret = dynamic_cast<FrontWeaponSlot*>(mpWeapon.get());
   if (turret != NULL)
   {
      Weapon* actor = turret->GetWeaponActor();
      dtCore::Transform weaponTransform;
      actor->GetTransform(weaponTransform, dtCore::Transformable::REL_CS);
      weaponTransform.SetRotation(-x * turret->GetHorizontalMaxAngle(),
         y * turret->GetVerticalMaxAngle(), 0);
      actor->SetTransform(weaponTransform, dtCore::Transformable::REL_CS);
   }
}

///////////////////////////////////////////////////////////////////////////////
