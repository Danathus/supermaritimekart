#include <actors/FrontWeapon.h>
#include <actors/FrontWeaponActor.h>

#include <dtDAL/actorproxy.h>
#include <dtDAL/floatactorproperty.h>
#include <dtUtil/log.h>

//////////////////////////////////////////////////////////////////////////
FrontWeapon::FrontWeapon(const std::string& name /*= "FrontWeapon"*/)
: Weapon(name)
, mHorizontalMaxAngle(30.0f)
, mVerticalMaxAngle(30.0f)
{
}

//////////////////////////////////////////////////////////////////////////
FrontWeapon::~FrontWeapon()
{
}

////////////////////////////////////////////////////////////////////////////////
void FrontWeapon::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
{
   Weapon::BuildPropertyMap(actorProxy);

   dtCore::DeltaDrawable* actor = NULL;
   actorProxy->GetActor(actor);

   if (!actor)
   {
      return;
   }

   actorProxy->AddProperty(new dtDAL::FloatActorProperty(
      mName + "MaxHorizontalAngle", "Max Horizontal Angle",
      dtDAL::FloatActorProperty::SetFuncType(this, &FrontWeapon::SetHorizontalMaxAngle),
      dtDAL::FloatActorProperty::GetFuncType(this, &FrontWeapon::GetHorizontalMaxAngle),
      "The max angle the weapon can rotate left or right to", mName));

   actorProxy->AddProperty(new dtDAL::FloatActorProperty(
      mName + "MaxVerticalAngle", "Max Vertical Angle",
      dtDAL::FloatActorProperty::SetFuncType(this, &FrontWeapon::SetVerticalMaxAngle),
      dtDAL::FloatActorProperty::GetFuncType(this, &FrontWeapon::GetVerticalMaxAngle),
      "The max angle the weapon can rotate up or down to", mName));
}

////////////////////////////////////////////////////////////////////////////////
void FrontWeapon::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   Weapon::GetPartialUpdateProperties(propNamesToFill);
}

///////////////////////////////////////////////////////////////////////////////
float FrontWeapon::GetHorizontalMaxAngle() const
{
   return mHorizontalMaxAngle;
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeapon::SetHorizontalMaxAngle(const float val)
{
   mHorizontalMaxAngle = val;
}

///////////////////////////////////////////////////////////////////////////////
float FrontWeapon::GetVerticalMaxAngle() const
{
   return mVerticalMaxAngle;
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeapon::SetVerticalMaxAngle(const float val)
{
   mVerticalMaxAngle = val;
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeapon::AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   // Our weapon actor must be a FrontWeaponActor type
   if (dynamic_cast<FrontWeaponActor*>(weaponActor) != NULL)
   {
      Weapon::AttachWeaponToBoat(weaponActor, boat);
   }
   else
   {
      LOG_ERROR("Invalid weapon type " + weaponActor->GetName() + " tried to attach to front spot");
      mpWeaponActor = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
