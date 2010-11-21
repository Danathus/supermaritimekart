#ifndef FrontWeaponActor_h__
#define FrontWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Weapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT FrontWeapon : public Weapon
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   FrontWeapon(const std::string& filename = "");

   virtual void FireWeapon();

protected:
   virtual ~FrontWeapon();

private:
};
#endif // FrontWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////