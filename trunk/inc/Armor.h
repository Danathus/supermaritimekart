#ifndef ARMOR_h__
#define ARMOR_h__

#include <Health.h>

namespace SMK
{
   /** Armor can be used to reduce the amount of damage weapons inflict.
     * Armor itself has health associated with it, this means Armor types
     * can take damage themselves and eventually fail.
   */
   class Armor : public Health
   {
   public:
      enum ArmorType
      {      
         ARMOR_IRON = 0,
         ARMOR_STEEL,
         ARMOR_ALUM,
         ARMOR_TITANIUM
      };

      Armor(ArmorType type=ARMOR_IRON);
      ~Armor();

      Armor::ArmorType GetArmorType() const;
      
   private:
      ArmorType mArmorType;
   };
}
#endif // ARMOR_h__
