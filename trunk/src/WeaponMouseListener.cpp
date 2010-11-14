#include <WeaponMouseListener.h>
#include <TurretWeapon.h>
#include <Weapon.h>
#include <WeaponActor.h>

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
void WeaponMouseListener::SetWeapon(Weapon* weapon)
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
   TurretWeapon* turret = dynamic_cast<TurretWeapon*>(mpWeapon.get());
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
   TurretWeapon* turret = dynamic_cast<TurretWeapon*>(mpWeapon.get());
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
   UpdateLastMousePosition(x, y);

   return false;
}

////////////////////////////////////////////////////////////////////////////////
bool WeaponMouseListener::HandleMouseDragged(const dtCore::Mouse* mouse, float x, float y)
{
   UpdateLastMousePosition(x, y);

   return false;  
}

///////////////////////////////////////////////////////////////////////////////
void WeaponMouseListener::UpdateLastMousePosition(float x, float y)
{
   TurretWeapon* turret = dynamic_cast<TurretWeapon*>(mpWeapon.get());
   if (turret != NULL)
   {
      WeaponActor* actor = turret->GetWeaponActor();
      dtCore::Transform weaponTransform;
      actor->GetTransform(weaponTransform, dtCore::Transformable::REL_CS);
      weaponTransform.SetRotation(-x * turret->GetHorizontalMaxAngle(),
         y * turret->GetVerticalMaxAngle(), 0);
      actor->SetTransform(weaponTransform, dtCore::Transformable::REL_CS);
   }
}

///////////////////////////////////////////////////////////////////////////////
