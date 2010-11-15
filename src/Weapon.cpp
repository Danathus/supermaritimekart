#include <Weapon.h>
#include <WeaponFactory.h>

#include <dtCore/deltadrawable.h>
#include <dtCore/transform.h>
#include <dtDAL/actorproxy.h>
#include <dtDAL/stringactorproperty.h>
#include <dtDAL/vectoractorproperties.h>

//////////////////////////////////////////////////////////////////////////
Weapon::Weapon(const std::string& name /*= "Weapon"*/)
: mName(name)
, mpWeaponActor(NULL)
{
}

//////////////////////////////////////////////////////////////////////////
Weapon::~Weapon()
{
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::Update(float deltaTime)
{
   if (mpWeaponActor.valid())
   {
      mpWeaponActor->Update(deltaTime);
   }
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
      mName + "Position", "Weapon Position",
      dtDAL::Vec3ActorProperty::SetFuncType(this, &Weapon::SetWeaponPosition),
      dtDAL::Vec3ActorProperty::GetFuncType(this, &Weapon::GetWeaponPosition),
      "The weapon's location relative to the BoatActor", mName));

   actorProxy->AddProperty(new dtDAL::StringActorProperty(
      mName + "DefaultWeapon", "Default Weapon",
      dtDAL::StringActorProperty::SetFuncType(this, &Weapon::SetDefaultWeapon),
      dtDAL::StringActorProperty::GetFuncType(this, &Weapon::GetDefaultWeapon),
      "The class name of the SMKBoatActor's default weapon", mName));
}

////////////////////////////////////////////////////////////////////////////////
void Weapon::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::Initialize(dtDAL::BaseActorObject* actorProxy)
{
   if (!mDefaultWeaponClass.empty())
   {
      SetWeapon(mDefaultWeaponClass, actorProxy);
   }
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::StartWeaponFire()
{
   if (mpWeaponActor.valid())
   {
      mpWeaponActor->StartWeaponFiring();
   }
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::StopWeaponFire()
{
   if (mpWeaponActor.valid())
   {
      mpWeaponActor->StopWeaponFiring();
   }
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::SetWeapon(const std::string& weaponClass, dtDAL::BaseActorObject* actorProxy)
{
   if (mpWeaponActor != NULL)
   {
      DetachWeaponFromBoat(mpWeaponActor, actorProxy->GetActor());
   }

   // Create default weapon actor
   mpWeaponActor = CreateWeaponActor(weaponClass);

   if (mpWeaponActor != NULL)
   {
      // Attach weapon actor to boat at location
      AttachWeaponToBoat(mpWeaponActor, actorProxy->GetActor());
   }
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

///////////////////////////////////////////////////////////////////////////////
std::string Weapon::GetDefaultWeapon() const
{
   return mDefaultWeaponClass;
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::SetDefaultWeapon(const std::string& val)
{
   mDefaultWeaponClass = val;
}

///////////////////////////////////////////////////////////////////////////////
WeaponActor* Weapon::CreateWeaponActor(const std::string& weaponClass)
{
   //use the Item Factory to create the InventoryItem
   WeaponActor* weapon = WeaponFactory::GetInstance().Create(weaponClass);
   //Add the item to the RoleActor
   if (weapon == NULL)
   {
      LOG_ERROR("Unable to create weapon: " + weaponClass);
   }
   return weapon;
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   dtCore::Transform weaponTransform;
   weaponTransform.SetTranslation(mPosition);
   weaponActor->SetTransform(weaponTransform);
   boat->AddChild(weaponActor);
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::DetachWeaponFromBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   boat->RemoveChild(weaponActor);
}

////////////////////////////////////////////////////////////////////////////////
