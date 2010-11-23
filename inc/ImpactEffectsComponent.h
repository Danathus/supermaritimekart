#ifndef ImpactEffectsComponent_h__
#define ImpactEffectsComponent_h__

#include <dtGame/gmcomponent.h>

#include <dtAudio/sound.h>
#include <dtCore/particlesystem.h>

////////////////////////////////////////////////////////////////////////////////
namespace SMK
{
   class Damage;
}

class ImpactEffectsComponent : public dtGame::GMComponent
{
public:
   ImpactEffectsComponent();

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~ImpactEffectsComponent();

   void OnAddedToGM();

private:
   void InitializeImpactEffects();
   void LoadEffects(const std::string& sounds, std::vector< dtCore::RefPtr<dtAudio::Sound> >& loadedSounds,
      const std::string& particles, std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& loadedParticles,
      int maxAmountOfSimilarEffects);
   dtAudio::Sound* LoadSound(const std::string& file);
   dtCore::ParticleSystem* LoadParticles(const std::string& file);

   void AddParticlesToScene();
   void AddParticlesToScene(const std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& particles);
   void RemoveParticlesFromScene();
   void RemoveParticlesFromScene(const std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& particles);

   void PlayEffects(const SMK::Damage& damage);
   void PlayNextEffect(const osg::Vec3& position,
      const std::vector< dtCore::RefPtr<dtAudio::Sound> >& sounds,
      const std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& particles, int& effectIndex);

   std::vector< dtCore::RefPtr<dtAudio::Sound> >         mpExplosionSounds;
   std::vector< dtCore::RefPtr<dtCore::ParticleSystem> > mpExplosionParticles;
   int mCurrentExplosionIndex;
   std::vector< dtCore::RefPtr<dtAudio::Sound> >         mpProjectileSounds;
   std::vector< dtCore::RefPtr<dtCore::ParticleSystem> > mpProjectileParticles;
   int mCurrentProjectileIndex;
   std::vector< dtCore::RefPtr<dtAudio::Sound> >         mpImpactSounds;
   std::vector< dtCore::RefPtr<dtCore::ParticleSystem> > mpImpactParticles;
   int mCurrentImpactIndex;
};

////////////////////////////////////////////////////////////////////////////////

#endif //ImpactEffectsComponent_h__
