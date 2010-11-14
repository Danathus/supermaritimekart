#include <TurretWeapon.h>

#include <dtDAL/actorproxy.h>
#include <dtDAL/floatactorproperty.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////////
TurretWeapon::TurretWeapon(const std::string& name /*= "TurretWeapon"*/)
: Weapon(name)
, mHorizontalMaxAngle(30.0f)
, mVerticalMaxAngle(30.0f)
{
}

//////////////////////////////////////////////////////////////////////////
TurretWeapon::~TurretWeapon()
{
}

////////////////////////////////////////////////////////////////////////////////
void TurretWeapon::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
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
      dtDAL::FloatActorProperty::SetFuncType(this, &TurretWeapon::SetHorizontalMaxAngle),
      dtDAL::FloatActorProperty::GetFuncType(this, &TurretWeapon::GetHorizontalMaxAngle),
      "The max angle the weapon can rotate left or right to", mName));

   actorProxy->AddProperty(new dtDAL::FloatActorProperty(
      mName + "MaxVerticalAngle", "Max Vertical Angle",
      dtDAL::FloatActorProperty::SetFuncType(this, &TurretWeapon::SetVerticalMaxAngle),
      dtDAL::FloatActorProperty::GetFuncType(this, &TurretWeapon::GetVerticalMaxAngle),
      "The max angle the weapon can rotate up or down to", mName));
}

////////////////////////////////////////////////////////////////////////////////
void TurretWeapon::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
   Weapon::GetPartialUpdateProperties(propNamesToFill);
}

///////////////////////////////////////////////////////////////////////////////
void TurretWeapon::StartWeaponFire()
{
   std::cout << "Firing turret" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
void TurretWeapon::StopWeaponFire()
{
   std::cout << "Stopping turret fire" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
float TurretWeapon::GetHorizontalMaxAngle() const
{
   return mHorizontalMaxAngle;
}

///////////////////////////////////////////////////////////////////////////////
void TurretWeapon::SetHorizontalMaxAngle(const float val)
{
   mHorizontalMaxAngle = val;
}

///////////////////////////////////////////////////////////////////////////////
float TurretWeapon::GetVerticalMaxAngle() const
{
   return mVerticalMaxAngle;
}

///////////////////////////////////////////////////////////////////////////////
void TurretWeapon::SetVerticalMaxAngle(const float val)
{
   mVerticalMaxAngle = val;
}

////////////////////////////////////////////////////////////////////////////////
