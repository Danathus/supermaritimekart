#ifndef HEALTH_h__
#define HEALTH_h__

#include <util/SMKUtilExport.h>

namespace SMK
{
   typedef short HealthType;

   class SMK_UTIL_EXPORT Health
   {
   public:

      Health();
      Health(HealthType health, HealthType maxHealth);
      Health(const Health& copy);

      ~Health() {};

      ///Get the current health amount
      HealthType GetHealth() const;

      ///Set the current health amount.  Will clamp  [0..GetMax()]
      void SetHealth(HealthType val);

      ///Reduce the health by this amount.  Will clamp to 0 and return any extra damage not applied
      HealthType DecrementHealth(HealthType amount);

      ///Increase the health by this amount.  Will clamp to GetMax()
      void IncrementHealth(HealthType amount);

      ///Get the max amount of health possible
      HealthType GetMax() const;

      ///Set the max amount of health possible
      void SetMax(const HealthType& val);

   protected:

   private:
      HealthType mAmount;
      HealthType mMax;
   };
}
#endif // HEALTH_h__
