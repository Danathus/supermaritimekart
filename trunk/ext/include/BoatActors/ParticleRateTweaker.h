#ifndef ParticleRateTweaker_h__
#define ParticleRateTweaker_h__

#include <float.h>

#include <osg/Referenced>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/range>

#include <dtCore/refptr.h>
#include <vector>

namespace dtCore
{
   class ParticleSystem;
}


class ParticleRateTweaker : public osg::Referenced
{
public:
   /** 
    * @param psSystem The ParticleSystem to control - needs to have a 
    *                  osgParticle::RandomRateCounter in it.
    * @param minCap The min value to clamp the particle rates to
    * @param maxCap The max value to clamp the particle rates to
    */
	ParticleRateTweaker(dtCore::ParticleSystem& psSystem,
                       float minCap = 0.f,
                       float maxCap = FLT_MAX);

   /** Scale the minimum and maximum rates 
    * @param scale (0.f = no particles, 1.f = stock rates)
    */
   void SetScale(float scale);

protected:
   virtual ~ParticleRateTweaker();
	
private:
   typedef std::vector<dtCore::RefPtr<osgParticle::RandomRateCounter> > CounterVec;
   CounterVec mRateCounterVec;

   typedef std::vector<osgParticle::rangef> RangeVec;
   RangeVec mInitialRateVec;

   osg::Vec2 mMinMaxCap; ///<cap the rates to these values
};
#endif // ParticleRateTweaker_h__
