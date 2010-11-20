#ifndef BackWeaponSlot_h__
#define BackWeaponSlot_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/WeaponSlot.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT BackWeaponSlot : public WeaponSlot
{
public:
   BackWeaponSlot(const std::string& name = "BackWeapon");

   /**
   * Builds the property map for STAGE
   */
   virtual void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

protected:
   virtual ~BackWeaponSlot();
   virtual void AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat);

private:
};
#endif // BackWeaponSlot_h__

////////////////////////////////////////////////////////////////////////////////
