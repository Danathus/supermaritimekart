#ifndef ParticleDirectionTweaker_h__
#define ParticleDirectionTweaker_h__

#include <osg/Referenced>
#include <dtCore/refptr.h>
#include <osgParticle/RadialShooter>
#include <osgParticle/range>

namespace dtCore
{
   class ParticleSystem;
}

/** Used to tweak the direction the particles get emitted from
  * a ParticleSystem.  Assumes a osgParticle::RadialShooter is
  * in the ParticleSystem.
  */
class ParticleDirectionTweaker : public osg::Referenced
{
public:
   ParticleDirectionTweaker(dtCore::ParticleSystem& psSystem);

   /** 
    * @param direction The number of degrees to rotate the 
    * shooter by. 0=no change, + = rotate CCW, - = rotate CW
    */
   void SetAmountToRotateBy(float direction);

protected:
   virtual ~ParticleDirectionTweaker();
	
private:
   dtCore::RefPtr<osgParticle::RadialShooter> mShooter;
   osgParticle::rangef mInitialPhiRange; ///< a.k.a initial "heading" values

};
#endif // ParticleDirectionTweaker_h__
