#ifndef BackWeaponActor_h__
#define BackWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Weapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT BackWeapon : public Weapon
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   BackWeapon(const std::string& filename = "");

   virtual void FireWeapon();

protected:
   virtual ~BackWeapon();

private:
};
#endif // BackWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
