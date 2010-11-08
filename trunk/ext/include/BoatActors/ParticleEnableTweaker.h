#ifndef ParticleEnableTweaker_h__
#define ParticleEnableTweaker_h__

#include <osg/Referenced>
#include <dtCore/refptr.h>
#include <vector>
#include <dtCore/particlesystem.h>

class ParticleEnableTweaker : public osg::Referenced
{
public:
	ParticleEnableTweaker();
   void AddParticleSystem(dtCore::ParticleSystem& psSystem);
   void Clear();

   void SetEnabled(bool enabled) const;

protected:
   virtual ~ParticleEnableTweaker();
	
private:
   typedef std::vector<dtCore::RefPtr<dtCore::ParticleSystem> > ParticleSystemContainer;
   ParticleSystemContainer mEffects;
};
#endif // ParticleEnableTweaker_h__