#ifndef WeaponFactory_h__
#define WeaponFactory_h__

////////////////////////////////////////////////////////////////////////////////

#include <actors/SMKActorExport.h>
//#include <CCTS_ActorLibrary/InventoryItem.h>
#include <dtCore/refptr.h>
#include <dtUtil/objectfactory.h>

#include <string>

////////////////////////////////////////////////////////////////////////////////

class Weapon;

/**
 * Singleton class used to create derived InventoryItems from the
 * supplied InventoryItem type.
 * Example:
 * @code
 * WeaponFactory::GetInstance().RegisterType<TestItem>("TestItem");
 * InventoryItem* i = WeaponFactory::GetInstance().Create("TestItem");
 * @endcode
 */
class SMK_ACTOR_EXPORT WeaponFactory
{
public:
   static WeaponFactory& GetInstance();
   static void Destroy();

   template<typename ItemDerivative>
   bool RegisterType(const std::string& type)
   {
      return mpWeaponFactory->RegisterType<ItemDerivative>(type);
   }

   void RemoveType(const std::string& type)
   {
      mpWeaponFactory->RemoveType(type);
   }

   void RegisterInventoryItems();

   Weapon* Create(const std::string& itemType);

protected:
   WeaponFactory();
   ~WeaponFactory();

private:
   static WeaponFactory* mInstance;

   typedef dtUtil::ObjectFactory<std::string, Weapon> FactoryType;
   dtCore::RefPtr<FactoryType> mpWeaponFactory;
};

////////////////////////////////////////////////////////////////////////////////

#endif // WeaponFactory_h__
