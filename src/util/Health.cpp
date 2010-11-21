#include <util/Health.h>
#include <dtUtil/mathdefines.h>
#include <limits>

using namespace SMK;

////////////////////////////////////////////////////////////////////////////////
Health::Health():
 mAmount(0)
,mMax(100)
{

}

////////////////////////////////////////////////////////////////////////////////
Health::Health(HealthType health, HealthType maxHealth):
 mAmount(health)
,mMax(maxHealth)
{
}

////////////////////////////////////////////////////////////////////////////////
void Health::SetHealth(HealthType val)
{
   mAmount = val;

   dtUtil::Clamp(mAmount, HealthType(0), GetMax());

   //TODO notify parent object?
}

////////////////////////////////////////////////////////////////////////////////
HealthType Health::GetHealth() const
{
   return mAmount;
}


////////////////////////////////////////////////////////////////////////////////
HealthType Health::DecrementHealth(HealthType amount)
{
   mAmount -= amount;

   if (mAmount < 0)
   {
      HealthType damageRemaining = 0 - mAmount;
      dtUtil::ClampMin(mAmount, HealthType(0));
      return damageRemaining;
   }
   //TODO notify parent object?
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
void Health::IncrementHealth(HealthType amount)
{
   mAmount += amount;
   dtUtil::ClampMax(mAmount, GetMax());
   //TODO notify parent object?
}

////////////////////////////////////////////////////////////////////////////////
HealthType Health::GetMax() const
{
   return mMax;
}

////////////////////////////////////////////////////////////////////////////////
void Health::SetMax(const HealthType& val)
{
   mMax = val;
}

