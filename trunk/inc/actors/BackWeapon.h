#ifndef BackWeapon_h__
#define BackWeapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Weapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT BackWeapon : public Weapon
{
public:
   BackWeapon(const std::string& name = "BackWeapon");

   /**
   * Builds the property map for STAGE
   */
   virtual void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

protected:
   virtual ~BackWeapon();
   virtual void AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat);

private:
};
#endif // BackWeapon_h__

////////////////////////////////////////////////////////////////////////////////
