#include <actors/HealthPickup.h>
#include <actors/SMKBoatActor.h>

using namespace SMK;

const std::string HealthPickup::HEALTH_PICKUP_TYPE = "health";

////////////////////////////////////////////////////////////////////////////////
HealthPickup::HealthPickup()
: mAmount(100)
{

}

////////////////////////////////////////////////////////////////////////////////
HealthPickup::~HealthPickup()
{

}

////////////////////////////////////////////////////////////////////////////////
bool HealthPickup::Apply(SMKBoatActor& actor) const
{
   Health health(actor.GetHealth());
   health.IncrementHealth(HealthType(mAmount));
   actor.SetHealth(health);
   return true;
}
