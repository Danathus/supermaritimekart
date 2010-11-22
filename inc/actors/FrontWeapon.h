#ifndef FrontWeapon_h__
#define FrontWeapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Weapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT FrontWeapon : public Weapon
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   FrontWeapon(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor());

   virtual void FireWeapon();

protected:
   virtual ~FrontWeapon();

private:
};
#endif // FrontWeapon_h__

////////////////////////////////////////////////////////////////////////////////
