#ifndef Weapon_h__
#define Weapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <osg/Referenced>

#include <WeaponActor.h>

#include <dtCore/refptr.h>

#include <osg/Vec3>

#include <vector>

namespace dtCore
{
   class DeltaDrawable;
}

namespace dtDAL
{
   class BaseActorObject;
}

namespace dtUtil
{
   class RefString;
}

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT Weapon : public osg::Referenced
{
public:
   Weapon(const std::string& name = "Weapon");

   /**
   * Builds the property map for STAGE
   */
   virtual void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   /**
   * Initialize the weapon.
   */
   virtual void Initialize(dtDAL::BaseActorObject* actorProxy);

   virtual void StartWeaponFire() {}
   virtual void StopWeaponFire() {}

   void SetWeapon(const std::string& weaponClass, dtDAL::BaseActorObject* actorProxy);
   WeaponActor* GetWeaponActor() { return mpWeaponActor; }

   ///The relative position of the weapon is placed
   osg::Vec3 GetWeaponPosition() const;
   void SetWeaponPosition(const osg::Vec3& val);

   ///The default weapon for the boat actor
   std::string GetDefaultWeapon() const;
   void SetDefaultWeapon(const std::string& val);

protected:
   virtual ~Weapon();
   virtual WeaponActor* CreateWeaponActor(const std::string& weaponClass);
   virtual void AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat);
   virtual void DetachWeaponFromBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat);

   std::string                 mName;
   osg::Vec3                   mPosition;
   std::string                 mDefaultWeaponClass;
   dtCore::RefPtr<WeaponActor> mpWeaponActor;
};
#endif // Weapon_h__

////////////////////////////////////////////////////////////////////////////////
