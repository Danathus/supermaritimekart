#ifndef BoatEffects_h__
#define BoatEffects_h__

#include <osg/Referenced>
#include <dtCore/refptr.h>

class IGetHeight;
class Outboard;
class ParticleRateTweaker;
class ParticleSpeedTweaker;
class ParticleDirectionTweaker;
 

namespace dtCore
{
   class ParticleSystem;
   class Physical;
}

namespace dtDAL
{
   class BaseActorObject;
}

namespace dtAudio
{
   class Sound;
}

///Class used to contain all the visual/audio effects related to the boat
class BoatEffects : public osg::Referenced
{
public:
   BoatEffects(dtCore::Physical& physical, Outboard* outboard);

   void Initialize();

   /**
   * Builds the property map for STAGE
   */
   void BuildPropertyMap(dtDAL::BaseActorObject* actorProxy);

   void Update();

   void SetGetHeight(IGetHeight* getHeight);

   void SetWakeEmitterName(const std::string &fileName) { mWakeResource = fileName;}
   void SetPropWashEmitterName(const std::string &fileName) { mPropWashResource = fileName;}
   void SetBowSplashEmitterName(const std::string &fileName) { mBowSplashResource = fileName;}
   void SetEngineSoundName(const std::string &fileName) { mEngineResource = fileName;}

   /** 
    * @enable True will position the sounds in 3D space using range attenuation.
    * False will not position them and cause the sounds to be centered.
    */
   void SetUsePositionalSounds(bool enable);

   bool GetUsePositionSounds() const;

protected:
   virtual ~BoatEffects();
	
private:
   void GatherParticleInitialConditions();
   void UpdateWakeParticles();
   void UpdatePropWashParticles();
   void MakeASplash();
   void UpdateEngineSound();

   void MoveParticles();
   std::string                            mWakeResource;
   dtCore::RefPtr<dtCore::ParticleSystem> mWakeEmitter;

   std::string                            mPropWashResource;
   dtCore::RefPtr<dtCore::ParticleSystem> mPropWashEmitter;

   std::string                            mBowSplashResource;
   dtCore::RefPtr<dtCore::ParticleSystem> mBowSplashEmitter;

   std::string                            mEngineResource;
   dtCore::RefPtr<dtAudio::Sound>         mEngineSound;
   bool                                   mUsePositionalSounds; ///<Move the sounds?

   dtCore::RefPtr<dtCore::Physical> mPhysical;
   dtCore::RefPtr<IGetHeight> mGetHeight;

   dtCore::RefPtr<Outboard> mOutboard;

   ///The max height above the water at which the wake should stop (no wake when airborne!).
   ///Based on the water height at mPhysic's position
   static const float kHeightAboveWaterToDisableWake;

   dtCore::RefPtr<ParticleRateTweaker>      mWakeRateTweaker;
   dtCore::RefPtr<ParticleRateTweaker>      mPropWashRateTweaker;
   dtCore::RefPtr<ParticleSpeedTweaker>     mPropWashSpeedTweaker;
   dtCore::RefPtr<ParticleDirectionTweaker> mPropWashDirTweaker;
   dtCore::RefPtr<ParticleSpeedTweaker>     mBowSpraySpeedTweaker;
   dtCore::RefPtr<ParticleRateTweaker>      mBowSprayRateTweaker;
};
#endif // BoatEffects_h__
