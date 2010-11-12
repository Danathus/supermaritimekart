#ifndef Outboard_h__
#define Outboard_h__

#include <BoatActors/ActorLibraryExport.h>

#include <osg/Referenced>
#include <osg/Vec3>
#include <dtCore/refptr.h>

#include <osgSim/DOFTransform>

namespace dtCore
{
   class Physical;
}

namespace dtDAL
{
   class BaseActorObject;
}

namespace dtUtil
{
   class RefString;
}

class IGetHeight;

///Represents an outboard motor.  Will move the supplied Physical
class BOAT_ACTORS_LIBRARY_EXPORT Outboard : public osg::Referenced
{
public:

   enum EngineEnum
   {
      ENGINE_RIGHT,
      ENGINE_LEFT,
      ENGINE_MAX,
   };

   struct EngineData
   {
      dtCore::RefPtr<osgSim::DOFTransform> EngineMount;
      dtCore::RefPtr<osgSim::DOFTransform> EnginePropeller;
   };

   Outboard();

   /**
   * Builds the property map for STAGE
   */
   void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   /**
   * Activates thrust in either forward or reverse.
   *
   * @param[in]  forward  True if the thrust is forward thrust.
   */
   void ActivateThrust(bool forward = true);

   /**
   * Deactivates thrust.
   */
   void DeactivateThrust();

   /**
   * Activates turning to the left or right.
   *
   * @param[in]  left  True if the turn is to the left.
   */
   void ActivateTurning(bool left = true);

   /**
   * Deactivates turning.
   */
   void DeactivateTurning();

   /**
   * Cuts the engine and returns it back to 0 thrust.
   */
   void CutEngine();

   /**
   * The object to push around
   */
   void SetObject(dtCore::Physical& obj);

   /**
   * Sets the GetHeight class.
   */
   void SetGetHeight(IGetHeight* getHeight);

   ///-100..0..100
   void SetAnglePercent(float angle);
   float GetAnglePercent() const;

   ///-100..100
   void SetThrustPercent(float thrust);
   float GetThrustPercent() const;

   ///The relative position of where the thrust comes from
   osg::Vec3 GetThrustPosition() const;
   void SetThrustPosition(const osg::Vec3& val);

   void SetThrustDecayEnabled(bool enabled);
   void SetTurnDecayEnabled(bool enabled);

   ///Update every physics time step
   void Update(float dt);
   void UpdatePhysics();

   ///Property accessors.
   void SetMinThrust(float minThrust) {mMinThrust = minThrust;}
   float GetMinThrust() {return mMinThrust;}

   void SetMaxThrust(float maxThrust) {mMaxThrust = maxThrust;}
   float GetMaxThrust() {return mMaxThrust;}

   void SetMinAngleDeg(float minAngle) {mMinAngleDeg = minAngle;}
   float GetMinAngleDeg() {return mMinAngleDeg;}

   void SetMaxAngleDeg(float maxAngle) {mMaxAngleDeg = maxAngle;}
   float GetMaxAngleDeg() {return mMaxAngleDeg;}

   void SetSpeed(float speed) {mSpeed = speed;}
   float GetSpeed() {return mSpeed;}

   void SetTurnRate(float turnRate) {mTurnRate = turnRate;}
   float GetTurnRate() {return mTurnRate;}

   void SetSpeedDecay(float speedDecay) {mSpeedDecay = speedDecay;}
   float GetSpeedDecay() {return mSpeedDecay;}

   void SetTurnDecay(float turnDecay) {mTurnDecay = turnDecay;}
   float GetTurnDecay() {return mTurnDecay;}

   void SetEngineCutSpeed(float speed) {mEngineCutSpeed = speed;}
   float GetEngineCutSpeed() {return mEngineCutSpeed;}

protected:
   virtual ~Outboard();

private:
   void UpdateGeometry(float angle);

   dtCore::RefPtr<dtCore::Physical>  mObject;
   float mAnglePercent;
   float mThrustPercent;

   float mMinAngleDeg;
   float mMaxAngleDeg;
   float mMinThrust;
   float mMaxThrust;

   float mSpeed;
   float mTurnRate;

   float mSpeedDecay;
   float mTurnDecay;

   bool  mIsSpeedDecaying;
   bool  mIsTurnDecaying;

   osg::Vec3 mThrustPosition;
   float mPropSpinAngle;

   float    mForwardMovement;
   float    mTurnMovement;

   bool     mCuttingEngine;
   float    mEngineCutSpeed;

   EngineData                 mEngineDOF[ENGINE_MAX];
   dtCore::RefPtr<IGetHeight> mGetHeight;
};
#endif // Outboard_h__
