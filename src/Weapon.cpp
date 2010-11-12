#include <Weapon.h>

#include <dtCore/deltadrawable.h>
#include <dtDAL/actorproxy.h>
#include <dtDAL/vectoractorproperties.h>

//////////////////////////////////////////////////////////////////////////
Weapon::Weapon(const std::string& name /*= "Weapon"*/)
: mName(name)
{
}

//////////////////////////////////////////////////////////////////////////
Weapon::~Weapon()
{
}

////////////////////////////////////////////////////////////////////////////////
void Weapon::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
{
   dtCore::DeltaDrawable* actor = NULL;
   actorProxy->GetActor(actor);

   if (!actor)
   {
      return;
   }

   actorProxy->AddProperty(new dtDAL::Vec3ActorProperty(
      "WeaponPosition", "Weapon Position",
      dtDAL::Vec3ActorProperty::SetFuncType(this, &Weapon::SetWeaponPosition),
      dtDAL::Vec3ActorProperty::GetFuncType(this, &Weapon::GetWeaponPosition),
      "The weapon's location relative to the BoatActor", mName));
}

////////////////////////////////////////////////////////////////////////////////
void Weapon::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
}

///////////////////////////////////////////////////////////////////////////////
osg::Vec3 Weapon::GetWeaponPosition() const
{
   return mPosition;
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::SetWeaponPosition(const osg::Vec3& val)
{
   mPosition = val;
}

////////////////////////////////////////////////////////////////////////////////
