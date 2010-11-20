#ifndef FrontWeaponSlot_h__
#define FrontWeaponSlot_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/WeaponSlot.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT FrontWeaponSlot : public WeaponSlot
{
public:
   FrontWeaponSlot(const std::string& name = "FrontWeapon");

   /**
   * Builds the property map for STAGE
   */
   virtual void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   ///The max angle the weapon can rotate left or right to
   float GetHorizontalMaxAngle() const;
   void SetHorizontalMaxAngle(const float val);

   ///The max angle the weapon can rotate up or down to
   float GetVerticalMaxAngle() const;
   void SetVerticalMaxAngle(const float val);

protected:
   virtual ~FrontWeaponSlot();
   virtual void AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat);

private:
   float mHorizontalMaxAngle;
   float mVerticalMaxAngle;
};
#endif // FrontWeaponSlot_h__

////////////////////////////////////////////////////////////////////////////////
