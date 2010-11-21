#include <actors/FrontWeaponSlot.h>
#include <actors/FrontWeapon.h>

#include <dtDAL/actorproxy.h>
#include <dtDAL/floatactorproperty.h>
#include <dtUtil/log.h>

//////////////////////////////////////////////////////////////////////////
FrontWeaponSlot::FrontWeaponSlot(const std::string& name /*= "FrontWeapon"*/)
: WeaponSlot(name)
, mHorizontalMaxAngle(30.0f)
, mVerticalMaxAngle(30.0f)
{
}

//////////////////////////////////////////////////////////////////////////
FrontWeaponSlot::~FrontWeaponSlot()
{
}

////////////////////////////////////////////////////////////////////////////////
void FrontWeaponSlot::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
{
   WeaponSlot::BuildPropertyMap(actorProxy);

   dtCore::DeltaDrawable* actor = NULL;
   actorProxy->GetActor(actor);

   if (!actor)
   {
      return;
   }

   actorProxy->AddProperty(new dtDAL::FloatActorProperty(
      mName + "MaxHorizontalAngle", "Max Horizontal Angle",
      dtDAL::FloatActorProperty::SetFuncType(this, &FrontWeaponSlot::SetHorizontalMaxAngle),
      dtDAL::FloatActorProperty::GetFuncType(this, &FrontWeaponSlot::GetHorizontalMaxAngle),
      "The max angle the weapon can rotate left or right to", mName));

   actorProxy->AddProperty(new dtDAL::FloatActorProperty(
      mName + "MaxVerticalAngle", "Max Vertical Angle",
      dtDAL::FloatActorProperty::SetFuncType(this, &FrontWeaponSlot::SetVerticalMaxAngle),
      dtDAL::FloatActorProperty::GetFuncType(this, &FrontWeaponSlot::GetVerticalMaxAngle),
      "The max angle the weapon can rotate up or down to", mName));
}

////////////////////////////////////////////////////////////////////////////////
void FrontWeaponSlot::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   WeaponSlot::GetPartialUpdateProperties(propNamesToFill);
}

///////////////////////////////////////////////////////////////////////////////
float FrontWeaponSlot::GetHorizontalMaxAngle() const
{
   return mHorizontalMaxAngle;
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeaponSlot::SetHorizontalMaxAngle(const float val)
{
   mHorizontalMaxAngle = val;
}

///////////////////////////////////////////////////////////////////////////////
float FrontWeaponSlot::GetVerticalMaxAngle() const
{
   return mVerticalMaxAngle;
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeaponSlot::SetVerticalMaxAngle(const float val)
{
   mVerticalMaxAngle = val;
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeaponSlot::AttachWeaponToBoat(Weapon* Weapon, dtCore::DeltaDrawable* boat)
{
   // Our weapon actor must be a FrontWeapon type
   if (dynamic_cast<FrontWeapon*>(Weapon) != NULL)
   {
      WeaponSlot::AttachWeaponToBoat(Weapon, boat);
   }
   else
   {
      LOG_ERROR("Invalid weapon type " + Weapon->GetName() + " tried to attach to front spot");
      mpWeapon = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
