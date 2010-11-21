#ifndef DAMAGE_h__
#define DAMAGE_h__
#include <osg/Vec3>

namespace SMK
{
   typedef unsigned char DamageAmountType;

   /** 
    * Represents some type of potential damage to another object.  Resulting
    * damage to objects may be affected by Health and Armor.
    */
   class Damage
   {
   public:
      enum DamageType
      {
         DAMAGE_INVALID = -1,
         DAMAGE_BLAST,   ///< explosions
         DAMAGE_PROJECTILE,  ///< bullets, missiles
         DAMAGE_IMPACT       ///< collisions with static objects, players, ground
      };

      Damage();
      ~Damage() {};
    
      ///Get the amount of potential damage
      DamageAmountType GetAmount() const;

      ///Set the amount of damaging inflicting potential
      void SetAmount(const DamageAmountType& val);

      /** 
        * Get the location of the damage object.  This is where
        * the bullet, explosion, collision is.
        */
      osg::Vec3 GetLocation() const;

      /** 
        * Set the location of the damage object.  This is where
        * the bullet, explosion, collision is.
        */
      void SetLocation(const osg::Vec3& val);

      /** 
        * Get the radius of the damage effect.  Useful for large area damage
        * like explosions.
        */
      float GetRadius() const;

      /** 
        * Set the radius of the damage effect.  Useful for large area damage
        * like explosions.
        */
      void SetRadius(const float& val);

      ///Get the type of this Damage
      Damage::DamageType GetDamageType() const;

      ///Set the type of this Damage
      void SetDamageType(const Damage::DamageType& val);

   private:
      DamageAmountType mAmount; ///<damage inflicting power
      DamageType mDamageType; ///<Type of damage maker
      osg::Vec3 mLocation; ///<world coords
      float mRadius;  ///<world coords
   };
}
#endif // DAMAGE_h__
