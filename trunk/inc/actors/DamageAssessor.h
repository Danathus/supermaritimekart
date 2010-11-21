#ifndef DAMAGEASSESSOR_h__
#define DAMAGEASSESSOR_h__

namespace dtCore
{
   class Transformable;
}

namespace SMK
{
   class Damage;
   class Health;
   class Armor;

   ///something that takes damage
   struct DamageTaker
   {
      Health* mHealth; ///probably has Health
      Armor* mArmor;   ///might have Armor
      dtCore::Transformable* mTransformable; ///should have a position
   };

   /** 
     * The DamageAssessor's job is to figure out how much to reduce the 
     * DamageTaker's Health and Armor, based on the supplied Damage.
     */
   class DamageAssessor
   {
   public:
      DamageAssessor();
      ~DamageAssessor();

      void Assess(const Damage& damage, DamageTaker& damageTaker) const;
   };
}
#endif // DAMAGEASSESSOR_h__
