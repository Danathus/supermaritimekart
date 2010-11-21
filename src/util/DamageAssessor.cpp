#include <util/DamageAssessor.h>
#include <util/Armor.h>
#include <util/Damage.h>
#include <util/Health.h>
#include <dtCore/transformable.h>
#include <dtCore/transform.h>
#include <osg/BoundingSphere>

using namespace SMK;
////////////////////////////////////////////////////////////////////////////////
DamageAssessor::DamageAssessor()
{

}

////////////////////////////////////////////////////////////////////////////////
DamageAssessor::~DamageAssessor()
{

}


////////////////////////////////////////////////////////////////////////////////
void DamageAssessor::Assess(const Damage& damage, DamageTaker& damageTaker) const
{
   switch (damage.GetDamageType())
   {
      case Damage::DAMAGE_BLAST:
      {
         AssessBlastDamage(damage, damageTaker);
         break;
      }
      case Damage::DAMAGE_PROJECTILE:
      {
         AssessProjectileDamage(damage, damageTaker);
         break;
      }
      case Damage::DAMAGE_IMPACT:
      {
         AssessImpactDamage(damage, damageTaker);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMK::DamageAssessor::AssessBlastDamage(const Damage& damage, DamageTaker& damageTaker) const
{
   dtCore::Transform xform;
   damageTaker.mTransformable->GetTransform(xform);
   osg::BoundingSphere bsphere(damage.GetLocation(), damage.GetRadius());

   //the damage taker is within distance to receive damage
   if (bsphere.contains(xform.GetTranslation()))
   {
      // TODO: Alter damage based on distance to explosion location
      ApplyDamage(damage, damageTaker);
   }
}

///////////////////////////////////////////////////////////////////////////////
void SMK::DamageAssessor::AssessProjectileDamage(const Damage& damage, DamageTaker& damageTaker) const
{
   ApplyDamage(damage, damageTaker);
}

///////////////////////////////////////////////////////////////////////////////
void SMK::DamageAssessor::AssessImpactDamage(const Damage& damage, DamageTaker& damageTaker) const
{
   // TODO: Alter damage based on velocity of impacter?
   ApplyDamage(damage, damageTaker);
}

///////////////////////////////////////////////////////////////////////////////
void SMK::DamageAssessor::ApplyDamage(const Damage& damage, DamageTaker& damageTaker) const
{
   HealthType damageRemaining = damage.GetAmount();
   if (damageTaker.mArmor)
   {
      //has armor
      switch (damageTaker.mArmor->GetArmorType())
      {
         case Armor::ARMOR_STEEL:
         case Armor::ARMOR_IRON:
         case Armor::ARMOR_ALUM:
         {
            // TODO: Alter damage based on armor
            break;
         }
         default:
         {
            break;
         }
      }
   }

   //Reduce armor by damage and any extra damage to apply goes to the health
   damageRemaining = damageTaker.mArmor->DecrementHealth(damage.GetAmount());
   if (damageRemaining > 0)
   {
      damageTaker.mHealth->DecrementHealth(damageRemaining);
   }
}

///////////////////////////////////////////////////////////////////////////////

