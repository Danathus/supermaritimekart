#include <actors/WeaponSlot.h>
#include <actors/WeaponFactory.h>

#include <dtCore/deltadrawable.h>
#include <dtCore/transform.h>
#include <dtDAL/stringactorproperty.h>
#include <dtDAL/vectoractorproperties.h>
#include <dtGame/gameactorproxy.h>

//////////////////////////////////////////////////////////////////////////
WeaponSlot::WeaponSlot(const std::string& name /*= "Weapon"*/)
: mName(name)
, mpWeaponActor(NULL)
{
}

//////////////////////////////////////////////////////////////////////////
WeaponSlot::~WeaponSlot()
{
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::Update(float deltaTime)
{
   if (mpWeaponActor.valid())
   {
      mpWeaponActor->Update(deltaTime);
   }
}

////////////////////////////////////////////////////////////////////////////////
void WeaponSlot::BuildPropertyMap(dtDAL::BaseActorObject* actorProxy)
{
   dtCore::DeltaDrawable* actor = NULL;
   actorProxy->GetActor(actor);

   if (!actor)
   {
      return;
   }

   actorProxy->AddProperty(new dtDAL::Vec3ActorProperty(
      mName + "Position", "Weapon Position",
      dtDAL::Vec3ActorProperty::SetFuncType(this, &WeaponSlot::SetWeaponPosition),
      dtDAL::Vec3ActorProperty::GetFuncType(this, &WeaponSlot::GetWeaponPosition),
      "The weapon's location relative to the BoatActor", mName));

   actorProxy->AddProperty(new dtDAL::StringActorProperty(
      mName + "DefaultWeapon", "Default Weapon",
      dtDAL::StringActorProperty::SetFuncType(this, &WeaponSlot::SetDefaultWeapon),
      dtDAL::StringActorProperty::GetFuncType(this, &WeaponSlot::GetDefaultWeapon),
      "The class name of the SMKBoatActor's default weapon", mName));
}

////////////////////////////////////////////////////////////////////////////////
void WeaponSlot::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::Initialize(SMKBoatActorProxy* actorProxy)
{
   if (!mDefaultWeaponClass.empty())
   {
      SetWeapon(mDefaultWeaponClass, actorProxy);
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::StartWeaponFire()
{
   if (mpWeaponActor.valid())
   {
      mpWeaponActor->StartWeaponFiring();
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::StopWeaponFire()
{
   if (mpWeaponActor.valid())
   {
      mpWeaponActor->StopWeaponFiring();
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::SetWeapon(const std::string& weaponClass, SMKBoatActorProxy* actorProxy)
{
   if (mpWeaponActor != NULL)
   {
      DetachWeaponFromBoat(mpWeaponActor, actorProxy->GetActor());
   }

   // Create default weapon actor
   mpWeaponActor = CreateWeaponActor(weaponClass, actorProxy);

   if (mpWeaponActor != NULL)
   {
      // Attach weapon actor to boat at location
      AttachWeaponToBoat(mpWeaponActor, actorProxy->GetActor());
   }
}

///////////////////////////////////////////////////////////////////////////////
osg::Vec3 WeaponSlot::GetWeaponPosition() const
{
   return mPosition;
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::SetWeaponPosition(const osg::Vec3& val)
{
   mPosition = val;
}

///////////////////////////////////////////////////////////////////////////////
std::string WeaponSlot::GetDefaultWeapon() const
{
   return mDefaultWeaponClass;
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::SetDefaultWeapon(const std::string& val)
{
   mDefaultWeaponClass = val;
}

///////////////////////////////////////////////////////////////////////////////
WeaponActor* WeaponSlot::CreateWeaponActor(const std::string& weaponClass, SMKBoatActorProxy* actorProxy)
{
   //use the Item Factory to create the InventoryItem
   WeaponActor* weapon = WeaponFactory::GetInstance().Create(weaponClass);
   weapon->SetSMKBoatActorProxy(actorProxy);

   //Add the item to the RoleActor
   if (weapon == NULL)
   {
      LOG_ERROR("Unable to create weapon: " + weaponClass);
   }
   return weapon;
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::AttachWeaponToBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   dtCore::Transform weaponTransform;
   weaponTransform.SetTranslation(mPosition);
   weaponActor->SetTransform(weaponTransform);
   boat->AddChild(weaponActor);
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::DetachWeaponFromBoat(WeaponActor* weaponActor, dtCore::DeltaDrawable* boat)
{
   boat->RemoveChild(weaponActor);
}

////////////////////////////////////////////////////////////////////////////////
