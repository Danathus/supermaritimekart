#ifndef ARMORPICKUP_h__
#define ARMORPICKUP_h__

#include <actors/SMKActorExport.h>
#include <actors/PickupItem.h>
#include <util/Health.h>
#include <string>
#include <util/Armor.h>

namespace SMK
{

   ///A Iron Armor pickup
   class SMK_ACTOR_EXPORT IronArmorPickup : public PickupItem
   {
   public:
      const static std::string IRON_ARMOR_PICKUP_TYPE;

      IronArmorPickup();
      virtual ~IronArmorPickup();

      virtual bool Apply(SMKBoatActor& actor) const;
      
   private:
      Armor mArmor;
   };
}

#endif // ARMORPICKUP_h__
