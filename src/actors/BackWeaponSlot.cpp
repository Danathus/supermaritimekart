#include <actors/BackWeaponSlot.h>
#include <actors/BackWeapon.h>

#include <dtUtil/log.h>

//////////////////////////////////////////////////////////////////////////
BackWeaponSlot::BackWeaponSlot(const std::string& name /*= "BackWeaponSlot"*/)
: WeaponSlot(name)
{
}

//////////////////////////////////////////////////////////////////////////
BackWeaponSlot::~BackWeaponSlot()
{
}

////////////////////////////////////////////////////////////////////////////////
void BackWeaponSlot::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
{
   WeaponSlot::BuildPropertyMap(actorProxy);

   dtCore::DeltaDrawable* actor = NULL;
   actorProxy->GetActor(actor);

   if (!actor)
   {
      return;
   }
}

////////////////////////////////////////////////////////////////////////////////
void BackWeaponSlot::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   WeaponSlot::GetPartialUpdateProperties(propNamesToFill);
}

///////////////////////////////////////////////////////////////////////////////
void BackWeaponSlot::AttachWeaponToBoat(Weapon* Weapon, dtCore::DeltaDrawable* boat)
{
   // Our weapon actor must be a BackWeapon type
   if (dynamic_cast<BackWeapon*>(Weapon) != NULL)
   {
      WeaponSlot::AttachWeaponToBoat(Weapon, boat);
   }
   else
   {
      LOG_ERROR("Invalid weapon type " + Weapon->GetName() + " tried to attach to back spot");
      mpWeapon = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
