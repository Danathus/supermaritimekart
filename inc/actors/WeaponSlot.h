#ifndef WeaponSlot_h__
#define WeaponSlot_h__

////////////////////////////////////////////////////////////////////////////////
#include <actors/SMKActorExport.h>
#include <osg/Referenced>

#include <actors/Weapon.h>

#include <dtCore/refptr.h>

#include <osg/Vec3>

#include <vector>

namespace dtCore
{
   class DeltaDrawable;
}

namespace dtUtil
{
   class RefString;
}

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT WeaponSlot : public osg::Referenced
{
public:
   WeaponSlot(const std::string& name = "Weapon");

   virtual void Update(float deltaTime);

   /**
   * Builds the property map for STAGE
   */
   virtual void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   /**
   * Initialize the weapon.
   */
   virtual void Initialize(SMKBoatActorProxy* actorProxy);

   void StartWeaponFire();
   void StopWeaponFire();

   void SetWeapon(const std::string& weaponClass, SMKBoatActorProxy* actorProxy);
   Weapon* GetWeaponActor() { return mpWeaponActor; }

   ///The relative position of the weapon is placed
   osg::Vec3 GetWeaponPosition() const;
   void SetWeaponPosition(const osg::Vec3& val);

   ///The default weapon for the boat actor
   std::string GetDefaultWeapon() const;
   void SetDefaultWeapon(const std::string& val);

protected:
   virtual ~WeaponSlot();
   virtual Weapon* CreateWeaponActor(const std::string& weaponClass, SMKBoatActorProxy* actorProxy);
   virtual void AttachWeaponToBoat(Weapon* Weapon, dtCore::DeltaDrawable* boat);
   virtual void DetachWeaponFromBoat(Weapon* Weapon, dtCore::DeltaDrawable* boat);

   std::string                 mName;
   osg::Vec3                   mPosition;
   std::string                 mDefaultWeaponClass;
   dtCore::RefPtr<Weapon> mpWeaponActor;
};
#endif // WeaponSlot_h__

////////////////////////////////////////////////////////////////////////////////
