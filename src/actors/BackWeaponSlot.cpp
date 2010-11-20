#include <actors/BackWeaponSlot.h>
#include <actors/BackWeaponActor.h>

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
void BackWeaponSlot::AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   // Our weapon actor must be a BackWeaponActor type
   if (dynamic_cast<BackWeaponActor*>(weaponActor) != NULL)
   {
      WeaponSlot::AttachWeaponToBoat(weaponActor, boat);
   }
   else
   {
      LOG_ERROR("Invalid weapon type " + weaponActor->GetName() + " tried to attach to back spot");
      mpWeaponActor = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
