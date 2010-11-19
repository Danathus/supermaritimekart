#include <actors/Health.h>
#include <dtUtil/mathdefines.h>
#include <limits>

using namespace SMK;

////////////////////////////////////////////////////////////////////////////////
SMK::Health::Health():
 mAmount(UCHAR_MAX)
,mMax(UCHAR_MAX)
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
void Health::DecrementHealth(HealthType amount)
{  
   mAmount =- amount ;

   dtUtil::ClampMin(mAmount, HealthType(0));
   //TODO notify parent object?
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
