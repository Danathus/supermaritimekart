#ifndef HEALTHPICKUP_h__
#define HEALTHPICKUP_h__

#include <actors/SMKActorExport.h>
#include <actors/PickupItem.h>
#include <util/Health.h>
#include <string>

#include <BoatActors/BoatActor.h>

namespace SMK
{
   ///A health pickup with 100 health units!
   class SMK_ACTOR_EXPORT HealthPickup : public PickupItem
   {
   public:
      const static std::string HEALTH_PICKUP_TYPE;

      HealthPickup();
      virtual ~HealthPickup();

      virtual bool Apply(SMKBoatActor& actor) const;
      
   private:
      HealthType mAmount;
   };
}
#endif // HEALTHPICKUP_h__
