#ifndef ParticleSpeedTweaker_h__
#define ParticleSpeedTweaker_h__

#include <float.h>

#include <osg/Referenced>
#include <dtCore/refptr.h>
#include <osgParticle/RadialShooter>
#include <osgParticle/range>
#include <vector>

namespace dtCore
{
   class ParticleSystem;
}

/** Used to tweak the particle velocities of a ParticleSystem.
  * Assumes a osgParticle::RadialShoot is in the ParticleSystem.
  */
class ParticleSpeedTweaker : public osg::Referenced
{
public:
   /** @param psSystem The ParticleSystem to control
     * @param minCap The min velocity to cap to
     * @param maxCap The max velocity to cap to
     */
	ParticleSpeedTweaker(dtCore::ParticleSystem& psSystem,
                        float minCap=0.f,
                        float maxCap=FLT_MAX);
   
   /** Set the particle velocity scale
     * @param scale Amount to scale velocities by.  
     *              0 = no velocity, 1 = initial velocity
     */
   void SetScale(float scale);

protected:
   virtual ~ParticleSpeedTweaker();
	
private:
   typedef std::vector<dtCore::RefPtr<osgParticle::RadialShooter> > ShooterVector;
   ShooterVector mShooterVec;
   typedef std::vector<osgParticle::rangef> RangeVec;
   RangeVec mInitialSpeedRangeVec;
   osg::Vec2 mMinMaxCap; ///<cap the speeds to these values

};
#endif // ParticleSpeedTweaker_h__
