#include <DamageAssessor.h>
#include <Damage.h>
#include <Health.h>
#include <Armor.h>
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
   dtCore::Transform xform;
   damageTaker.mTransformable->GetTransform(xform);
   osg::BoundingSphere bsphere(damage.GetLocation(), damage.GetRadius());
   
   //the damage taker is within distance to receive damage
   if (bsphere.contains(xform.GetTranslation()))
   {
      HealthType currentArmorHealth = 0;

      if (damageTaker.mArmor)
      {
         //has armor
         switch (damageTaker.mArmor->GetArmorType())
         {
         case Damage::DAMAGE_BLAST:
         case Damage::DAMAGE_PROJECTILE:
         case Damage::DAMAGE_IMPACT:
            {
               //trivial reduction of the health of the armor
               damageTaker.mArmor->DecrementHealth(damage.GetAmount());               
               currentArmorHealth = damageTaker.mArmor->GetHealth();
            }
            break;
         default:
            break;
         }        
      }

      if (damageTaker.mHealth)
      {
         //has no more armor; time to inflict pain
         if (currentArmorHealth == 0)
         {
            damageTaker.mHealth->DecrementHealth(damage.GetAmount());
         }
      }
   }
}
