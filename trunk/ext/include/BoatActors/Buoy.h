#ifndef Buoy_h__
#define Buoy_h__

#include <dtCore/base.h>
#include <dtCore/refptr.h>
#include <dtCore/odecontroller.h>

namespace dtCore
{
   class Physical;
   class Transformable;
}

namespace dtDAL
{
   class BaseActorObject;
}

class IGetHeight;

///Makes the supplied Physical appear like its floating in water
class Buoy : public dtCore::Base
{
public:
  Buoy();

   /**
   * Builds the property map for STAGE
   */
   void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   ///Update every physics time step
   void Update();

   ///The object to "float"
   void SetObject(dtCore::Physical &obj);

   ///required for creating contact points; keeps the boat from sinking
   void SetODEController(dtCore::ODEController &controller);
   dtCore::ODEController* GetController() const;

   ///The interface to get the height of the water
   void SetGetHeight(IGetHeight* getHeight);

   ///The body relative friction co-efficient, used for drag
   void SetMu(const osg::Vec3& val);
   osg::Vec3 GetMu() const;

   ///Set the rotational drag, similar to Mu, but for rotations
   void SetRotationalDrag(const osg::Vec3& val);
   osg::Vec3 GetRotationalDrag() const;

   /** The Error Reduction Parameter; affects how fast the boat recovers
     * from sinking under water.
     * @param val (0..1) 0 = no correction, 1 = no errors
     */
   void SetERP(float val);
   float GetERP() const;

   /** The Constraint Mixing Force.  Affects how "spongy" the contacts are.
     * @val (0..1) 0 = "hard" contact with water, 1 = soggy toilet paper contact
     */
   void SetCFM(float val);
   float GetCFM() const;

   ///Set the maximum buoyant force possible
   void SetMaxBuoyantForce(float force);
   float GetMaxBuoyantForce() const;

   ///Set the maximum planing force possible
   void SetMaxPlaningForce(float force);
   float GetMaxPlaningForce() const;

   /**
   * Sets the value of the current indexed float point.
   *
   * @param[in]  value  The new value.
   */
   void SetFloatPointValue(const osg::Vec3& value);

   /**
   * Gets the value of the current indexed float point.
   *
   * @return  The current float point.
   */
   osg::Vec3 GetFloatPointValue();

   /**
   * Sets the current float point index.
   */
   void SetFloatPointIndex(int index);

   /**
   * Get the default value for a new float point.
   */
   osg::Vec3 GetDefaultFloatPoint();

   /**
   * Get the float point array.
   */
   std::vector<osg::Vec3> GetFloatPointList();

   /**
   * Sets the float point array.
   */
   void SetFloatPointList(const std::vector<osg::Vec3>& value);


protected:
   virtual ~Buoy();

private:
   dtCore::RefPtr<dtCore::Physical> mObject;
   dtCore::RefPtr<IGetHeight> mGetHeight;
   dtCore::RefPtr<dtCore::ODEController> mController;
   std::vector<osg::Vec3>  mFloatPoints;
   int                     mFloatPointIndex;
   std::vector<dtCore::RefPtr<dtCore::Transformable> > mFloatXforms;

   osg::Vec3 mMu; ///<linear water drag, local coordinates
   osg::Vec3 mRotationDrag; ///<Rotational water drag, local coordinates
   float mERP; ///< Error reduction parameter, for water contacts
   float mCFM; ///< Constraint mixing force, for water contacts
   float mMaxBuoyantForce;
   float mMaxPlaningForce;
   float mCurrentPlaningForce;


   void UseContacts();

   /** Add linear and rotational drag on the body, velocity dependent.
    * @param mu linear coefficient of friction, local coordinate system
    * @param rotationalDrag Rotational friction, local coordinate system
    */
   void ApplyDragOnBody(const osg::Vec3& mu, const osg::Vec3& rotationalDrag);

   void ApplyAngularDrag(const osg::Vec3& localLinearVelocity, const osg::Vec3& rotationalDrag);
   void ApplyLinearDrag(const osg::Vec3& localLinearVelocity, const osg::Vec3& mu);
   void CalcPlaningForce();
   void SetupDebugPoints();

};
#endif // Buoy_h__
