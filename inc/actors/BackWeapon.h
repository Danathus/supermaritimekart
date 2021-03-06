#ifndef BackWeapon_h__
#define BackWeapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Weapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT BackWeapon : public Weapon
{
public:
   const static std::string MACHINE_GUN_ACTOR_TYPE;

   BackWeapon(const dtDAL::ResourceDescriptor& resource = dtDAL::ResourceDescriptor());

   virtual void FireWeapon();

protected:
   virtual ~BackWeapon();

private:
};
#endif // BackWeapon_h__

////////////////////////////////////////////////////////////////////////////////
