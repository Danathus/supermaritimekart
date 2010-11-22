#include <actors/PickupItemFactory.h>
#include <string>
using namespace SMK;

PickupItemFactory* PickupItemFactory::mInstance = NULL;

////////////////////////////////////////////////////////////////////////////////
PickupItemFactory::PickupItemFactory()
: mFactory(new FactoryType())
{

}

////////////////////////////////////////////////////////////////////////////////
SMK::PickupItemFactory::~PickupItemFactory()
{

}

////////////////////////////////////////////////////////////////////////////////
PickupItemFactory& SMK::PickupItemFactory::GetInstance()
{
   if (mInstance == NULL)
   {
      mInstance = new PickupItemFactory();
   }

   return *mInstance;
}

////////////////////////////////////////////////////////////////////////////////
void SMK::PickupItemFactory::Destroy()
{
   if (mInstance)
   {
      delete mInstance;
      mInstance = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
void SMK::PickupItemFactory::RemoveType(const std::string& pickupType)
{
   mFactory->RemoveType(pickupType);
}

////////////////////////////////////////////////////////////////////////////////
PickupItem* PickupItemFactory::Create(const std::string& pickupType)
{
   PickupItem* item = mFactory->CreateObject(pickupType);
   return item;
}
