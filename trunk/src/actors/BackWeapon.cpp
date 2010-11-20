#include <actors/BackWeapon.h>
#include <actors/BackWeaponActor.h>

#include <dtUtil/log.h>

//////////////////////////////////////////////////////////////////////////
BackWeapon::BackWeapon(const std::string& name /*= "BackWeapon"*/)
: Weapon(name)
{
}

//////////////////////////////////////////////////////////////////////////
BackWeapon::~BackWeapon()
{
}

////////////////////////////////////////////////////////////////////////////////
void BackWeapon::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
{
   Weapon::BuildPropertyMap(actorProxy);

   dtCore::DeltaDrawable* actor = NULL;
   actorProxy->GetActor(actor);

   if (!actor)
   {
      return;
   }
}

////////////////////////////////////////////////////////////////////////////////
void BackWeapon::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   Weapon::GetPartialUpdateProperties(propNamesToFill);
}

///////////////////////////////////////////////////////////////////////////////
void BackWeapon::AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   // Our weapon actor must be a BackWeaponActor type
   if (dynamic_cast<BackWeaponActor*>(weaponActor) != NULL)
   {
      Weapon::AttachWeaponToBoat(weaponActor, boat);
   }
   else
   {
      LOG_ERROR("Invalid weapon type " + weaponActor->GetName() + " tried to attach to back spot");
      mpWeaponActor = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
