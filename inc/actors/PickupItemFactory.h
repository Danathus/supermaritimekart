#ifndef PICKUPITEMFACTORY_h__
#define PICKUPITEMFACTORY_h__
#include <actors/SMKActorExport.h>

#include <dtUtil/objectfactory.h>
#include <dtCore/refptr.h>

namespace SMK
{
   class PickupItem;

   /** 
    * Singleton factory used to create PickupItems.  Call RegisterType()
    * to register new types of PickupItems.
   */
   class SMK_ACTOR_EXPORT PickupItemFactory
   {
   public:
      static PickupItemFactory& GetInstance();

      static void Destroy();

      template<typename ItemDerivative>
      bool RegisterType(const std::string& pickupType)
      {
         return mFactory->RegisterType<ItemDerivative>(pickupType);
      }

      void RemoveType(const std::string& pickupType);

      /** 
        * Create the PickupItem.  
        * @return could be NULL if pickupType isn't registered
        */
      PickupItem* Create(const std::string& pickupType);

   protected:
      PickupItemFactory();
      ~PickupItemFactory();
      
   private: 
      static PickupItemFactory* mInstance;

      typedef dtUtil::ObjectFactory<std::string, PickupItem> FactoryType;
      dtCore::RefPtr<FactoryType> mFactory;

   };
}
#endif // PICKUPITEMFACTORY_h__
