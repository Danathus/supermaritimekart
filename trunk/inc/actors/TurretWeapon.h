#ifndef TurretWeapon_h__
#define TurretWeapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <actors/Weapon.h>

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT TurretWeapon : public Weapon
{
public:
   TurretWeapon(const std::string& name = "TurretWeapon");

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
   virtual ~TurretWeapon();

private:
   float mHorizontalMaxAngle;
   float mVerticalMaxAngle;
};
#endif // TurretWeapon_h__

////////////////////////////////////////////////////////////////////////////////
