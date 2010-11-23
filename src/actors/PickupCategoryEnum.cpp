#include <actors/PickupCategoryEnum.h>

using namespace SMK;

IMPLEMENT_ENUM(PickupCategoryEnum);

////////////////////////////////////////////////////////////////////////////////
PickupCategoryEnum::PickupCategoryEnum(const std::string& name)
: dtUtil::Enumeration(name)
{
   AddInstance(this);
}

 PickupCategoryEnum PickupCategoryEnum::PICKUP_NONE("None");
 PickupCategoryEnum PickupCategoryEnum::PICKUP_HEALTH("Health");
 PickupCategoryEnum PickupCategoryEnum::PICKUP_ARMOR("Armor");
 PickupCategoryEnum PickupCategoryEnum::PICKUP_BOW_WEAPON("Bow Weapon");
 PickupCategoryEnum PickupCategoryEnum::PICKUP_REAR_WEAPON("Rear Weapon");
 PickupCategoryEnum PickupCategoryEnum::PICKUP_TOP_WEAPON("Top Weapon");
 PickupCategoryEnum PickupCategoryEnum::PICKUP_SIDE_WEAPON("Side Weapon");
