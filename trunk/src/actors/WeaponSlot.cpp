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
, mpWeapon(NULL)
{
}

//////////////////////////////////////////////////////////////////////////
WeaponSlot::~WeaponSlot()
{
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::Update(float deltaTime)
{
   if (mpWeapon.valid())
   {
      mpWeapon->Update(deltaTime);
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

   if (!actorProxy->IsInSTAGE())
   {
      actorProxy->AddProperty(new dtDAL::StringActorProperty(
         mName + "CurrentWeapon", "Current Weapon",
         dtDAL::StringActorProperty::SetFuncType(this, &WeaponSlot::SetCurrentWeapon),
         dtDAL::StringActorProperty::GetFuncType(this, &WeaponSlot::GetCurrentWeapon),
         "The class name of the SMKBoatActor's current weapon", mName));
   }
}

////////////////////////////////////////////////////////////////////////////////
void WeaponSlot::GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill)
{
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::Initialize(SMKBoatActorProxy* actorProxy)
{
   std::string initialWeapon = mDefaultWeaponClass;
   if (actorProxy->IsRemote())
   {
      initialWeapon = mCurrentWeaponClass;
   }
   if (!initialWeapon.empty())
   {
      SetWeapon(initialWeapon, actorProxy);
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::StartWeaponFire()
{
   if (mpWeapon.valid())
   {
      mpWeapon->StartWeaponFiring();
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::StopWeaponFire()
{
   if (mpWeapon.valid())
   {
      mpWeapon->StopWeaponFiring();
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::SetWeapon(const std::string& weaponClass, dtGame::GameActorProxy* actorProxy)
{
   // Set this weapon as our new default
   mCurrentWeaponClass = weaponClass;

   if (mpWeapon != NULL)
   {
      DetachWeaponFromBoat(mpWeapon, actorProxy->GetActor());
   }

   // Create default weapon actor
   mpWeapon = CreateWeapon(weaponClass, actorProxy);

   if (mpWeapon != NULL)
   {
      // Attach weapon actor to boat at location
      AttachWeaponToBoat(mpWeapon, actorProxy->GetActor());
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
std::string WeaponSlot::GetCurrentWeapon() const
{
   return mCurrentWeaponClass;
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::SetCurrentWeapon(const std::string& val)
{
   mCurrentWeaponClass = val;
}

///////////////////////////////////////////////////////////////////////////////
Weapon* WeaponSlot::CreateWeapon(const std::string& weaponClass, dtGame::GameActorProxy* actorProxy)
{
   //use the Item Factory to create the InventoryItem
   Weapon* weapon = WeaponFactory::GetInstance().Create(weaponClass);
   weapon->SetSMKBoatActorProxy(actorProxy);

   //Add the item to the RoleActor
   if (weapon == NULL)
   {
      LOG_ERROR("Unable to create weapon: " + weaponClass);
   }
   return weapon;
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::AttachWeaponToBoat(Weapon* Weapon, dtCore::DeltaDrawable* boat)
{
   dtCore::Transform weaponTransform;
   weaponTransform.SetTranslation(mPosition);
   Weapon->SetTransform(weaponTransform);
   boat->AddChild(Weapon);
}

///////////////////////////////////////////////////////////////////////////////
void WeaponSlot::DetachWeaponFromBoat(Weapon* Weapon, dtCore::DeltaDrawable* boat)
{
   boat->RemoveChild(Weapon);
}

////////////////////////////////////////////////////////////////////////////////
