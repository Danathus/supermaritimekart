#ifndef ARMOR_h__
#define ARMOR_h__

#include <util/SMKUtilExport.h>
#include <util/Health.h>

namespace SMK
{
   /** Armor can be used to reduce the amount of damage weapons inflict.
     * Armor itself has health associated with it, this means Armor types
     * can take damage themselves and eventually fail.
   */
   class SMK_UTIL_EXPORT Armor : public Health
   {
   public:
      enum ArmorType
      {
         ARMOR_NONE = 0,
         ARMOR_IRON,  ///<Heavy, easily damaged, abundant
         ARMOR_STEEL, ///<mid-grade, general purpose
         ARMOR_ALUM,  ///<lightweight, fairly resistant, rare
         ARMOR_TITANIUM ///<super lightweight, very durable, very rare!
      };

      Armor(ArmorType type=ARMOR_NONE);
      ~Armor();

      Armor::ArmorType GetArmorType() const;

   private:
      ArmorType mArmorType;
   };
}
#endif // ARMOR_h__
