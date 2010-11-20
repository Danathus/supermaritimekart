#ifndef FrontWeaponActor_h__
#define FrontWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/WeaponActor.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT FrontWeaponActor : public WeaponActor
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   FrontWeaponActor(const std::string& filename = "");

   virtual void FireWeapon();

protected:
   virtual ~FrontWeaponActor();

private:
};
#endif // FrontWeaponActor_h__

////////////////////////////////////////////////////////////////////////////////
