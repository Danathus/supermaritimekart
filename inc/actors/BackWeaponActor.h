#ifndef BackWeaponActor_h__
#define BackWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/WeaponActor.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT BackWeaponActor : public WeaponActor
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   BackWeaponActor(const std::string& filename = "");

   virtual void FireWeapon();

protected:
   virtual ~BackWeaponActor();

private:
};
#endif // BackWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
