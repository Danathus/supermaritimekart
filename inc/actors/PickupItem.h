#ifndef PICKUPITEM_h__
#define PICKUPITEM_h__

#include <actors/SMKActorExport.h>

class SMKBoatActor;

namespace SMK
{
   ///Interface class for all PickupItems.  Derive and overwrite Apply()
   class SMK_ACTOR_EXPORT PickupItem
   {
   public:
      PickupItem() {};
      virtual ~PickupItem() {};

      /** 
        * Apply this PickupItem to the supplied actor.  
        *@return true if the application was successful, false otherwise
        */
      virtual bool Apply(SMKBoatActor& actor) const = 0;
   };
}
#endif // PICKUPITEM_h__
