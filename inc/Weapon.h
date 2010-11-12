#ifndef Weapon_h__
#define Weapon_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <osg/Referenced>

#include <dtCore/refptr.h>

#include <osg/Vec3>

#include <vector>

namespace dtDAL
{
   class BaseActorObject;
}

namespace dtUtil
{
   class RefString;
}

////////////////////////////////////////////////////////////////////////////////
class SMK_ACTOR_EXPORT Weapon : public osg::Referenced
{
public:
   Weapon(const std::string& name = "Weapon");

   /**
   * Builds the property map for STAGE
   */
   void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   ///The relative position of where the thrust comes from
   osg::Vec3 GetWeaponPosition() const;
   void SetWeaponPosition(const osg::Vec3& val);

protected:
   virtual ~Weapon();

private:

   std::string mName;
   osg::Vec3   mPosition;
};
#endif // Weapon_h__

////////////////////////////////////////////////////////////////////////////////
