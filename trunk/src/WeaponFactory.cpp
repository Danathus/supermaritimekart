#include <WeaponFactory.h>
#include <MachineGunActor.h>

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
WeaponActor* WeaponFactory::Create(const std::string& itemType)
{
   WeaponActor* inventoryItem = mpWeaponFactory->CreateObject(itemType);
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

   success &= weaponFactory.RegisterType<MachineGunActor>(MachineGunActor::MACHINE_GUN_ACTOR_TYPE);

   // sanity check
   assert(success);
}

////////////////////////////////////////////////////////////////////////////////

