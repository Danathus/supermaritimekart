#include <actors/ArmorPickup.h>
#include <actors/SMKBoatActor.h>

using namespace SMK;

const std::string IronArmorPickup::IRON_ARMOR_PICKUP_TYPE = "armor_iron";

////////////////////////////////////////////////////////////////////////////////
IronArmorPickup::IronArmorPickup()
: mArmor(Armor::ARMOR_IRON)
{
   mArmor.SetMax(50);
   mArmor.SetHealth(50);
}

////////////////////////////////////////////////////////////////////////////////
IronArmorPickup::~IronArmorPickup()
{

}

////////////////////////////////////////////////////////////////////////////////
bool IronArmorPickup::Apply(SMKBoatActor& actor) const
{
   actor.SetArmor(mArmor);
   return true;
}
