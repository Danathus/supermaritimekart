#include <actors/WeaponFactory.h>
#include <actors/MachineGun.h>
#include <actors/MineDropper.h>
#include <actors/RocketLauncher.h>

#include <cassert>

WeaponFactory* WeaponFactory::mInstance = NULL;

////////////////////////////////////////////////////////////////////////////////
WeaponFactory::WeaponFactory()
: mpWeaponFactory(new FactoryType())
{

}

////////////////////////////////////////////////////////////////////////////////
WeaponFactory::~WeaponFactory()
{

}

//Static
////////////////////////////////////////////////////////////////////////////////
WeaponFactory& WeaponFactory::GetInstance()
{
   if (mInstance == NULL)
   {
      mInstance = new WeaponFactory();
   }

   return *mInstance;
}

////////////////////////////////////////////////////////////////////////////////
Weapon* WeaponFactory::Create(const std::string& itemType)
{
   Weapon* inventoryItem = mpWeaponFactory->CreateObject(itemType);
   return inventoryItem;
}

////////////////////////////////////////////////////////////////////////////////
void WeaponFactory::Destroy()
{
   if (mInstance)
   {
      delete mInstance;
      mInstance = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
void WeaponFactory::RegisterInventoryItems()
{
   WeaponFactory& weaponFactory = WeaponFactory::GetInstance();

   bool success = true;

   success &= weaponFactory.RegisterType<MachineGun>(MachineGun::MACHINE_GUN_ACTOR_TYPE);
   success &= weaponFactory.RegisterType<MineDropper>(MineDropper::MINE_DROPPER_WEAPON_TYPE);
   success &= weaponFactory.RegisterType<RocketLauncher>(RocketLauncher::ROCKET_LAUNCHER_ACTOR_TYPE);

   // sanity check
   assert(success);
}

////////////////////////////////////////////////////////////////////////////////

