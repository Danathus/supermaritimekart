#ifndef PICKUPTYPEENUM_h__
#define PICKUPTYPEENUM_h__

#include <actors/SMKActorExport.h>
#include <dtUtil/enumeration.h>

namespace SMK
{
   class SMK_ACTOR_EXPORT PickupCategoryEnum : public dtUtil::Enumeration
   {
      DECLARE_ENUM(PickupCategoryEnum);
   public:
      static PickupCategoryEnum PICKUP_NONE;
      static PickupCategoryEnum PICKUP_HEALTH;
      static PickupCategoryEnum PICKUP_ARMOR;
      static PickupCategoryEnum PICKUP_BOW_WEAPON;
      static PickupCategoryEnum PICKUP_REAR_WEAPON;
      static PickupCategoryEnum PICKUP_TOP_WEAPON;
      static PickupCategoryEnum PICKUP_SIDE_WEAPON;
           
   private:
      PickupCategoryEnum(const std::string& name);
   };

}
#endif // PICKUPTYPEENUM_h__