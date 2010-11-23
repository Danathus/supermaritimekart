#include <ImpactEffectsComponent.h>
#include <messages/DamageMessage.h>
#include <messages/NetworkMessages.h>
#include <util/Damage.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/scene.h>
#include <dtCore/transform.h>
#include <dtGame/messagetype.h>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
static const int MAX_NUMBER_OF_EXPLOSION_EFFECTS = 5;
static const int MAX_NUMBER_OF_PROJECTILE_EFFECTS = 20;
static const int MAX_NUMBER_OF_IMPACT_EFFECTS = 10;

////////////////////////////////////////////////////////////////////////////////
ImpactEffectsComponent::ImpactEffectsComponent()
: dtGame::GMComponent("ImpactEffectsComponent")
, mCurrentExplosionIndex(0)
, mCurrentProjectileIndex(0)
, mCurrentImpactIndex(0)
{
}

////////////////////////////////////////////////////////////////////////////////
ImpactEffectsComponent::~ImpactEffectsComponent()
{
}

////////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == SMK::SMKNetworkMessages::ACTION_PROJECTILE_EXPLODED ||
       message.GetMessageType() == SMK::SMKNetworkMessages::ACTION_BOAT_HIT)
   {
      const SMK::DamageMessage& damageMsg = static_cast<const SMK::DamageMessage&>(message);
      PlayEffects(damageMsg.GetDamage());
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      AddParticlesToScene();
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_UNLOAD_BEGIN)
   {
      RemoveParticlesFromScene();
   }
}
///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::OnAddedToGM()
{
   dtGame::GMComponent::OnAddedToGM();
   InitializeImpactEffects();
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::InitializeImpactEffects()
{
   LoadEffects("/sounds/explosion.wav", mpExplosionSounds, "particles/projectile_explosion.osg", mpExplosionParticles, MAX_NUMBER_OF_EXPLOSION_EFFECTS);
   LoadEffects("/sounds/impact.wav", mpProjectileSounds, "particles/impactPuff.osg", mpProjectileParticles, MAX_NUMBER_OF_PROJECTILE_EFFECTS);
   LoadEffects("/sounds/impact.wav", mpImpactSounds, "particles/impactPuff.osg", mpImpactParticles, MAX_NUMBER_OF_IMPACT_EFFECTS);
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::LoadEffects(const std::string& sounds, std::vector< dtCore::RefPtr<dtAudio::Sound> >& loadedSounds,
   const std::string& particles, std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& loadedParticles,
   int maxAmountOfSimilarEffects)
{
   for (int i = 0; i < maxAmountOfSimilarEffects; ++i)
   {
      loadedSounds.push_back(LoadSound(sounds));
      loadedParticles.push_back(LoadParticles(particles));
   }
}

///////////////////////////////////////////////////////////////////////////////
dtAudio::Sound* ImpactEffectsComponent::LoadSound(const std::string& file)
{
   if (!file.empty())
   {
      dtAudio::Sound* sound = dtAudio::AudioManager::GetInstance().NewSound();
      sound->LoadFile(file.c_str());

      return sound;
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
dtCore::ParticleSystem* ImpactEffectsComponent::LoadParticles(const std::string& file)
{
   if (!file.empty())
   {
      dtCore::ParticleSystem* newParticles = new dtCore::ParticleSystem;
      newParticles->LoadFile(file);
      newParticles->SetEnabled(false);
      return newParticles;
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::AddParticlesToScene()
{
   AddParticlesToScene(mpExplosionParticles);
   AddParticlesToScene(mpProjectileParticles);
   AddParticlesToScene(mpImpactParticles);
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::AddParticlesToScene(const std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& particles)
{
   for (size_t effectIndex = 0; effectIndex < particles.size(); ++effectIndex)
   {
      GetGameManager()->GetScene().AddDrawable(particles[effectIndex]);
   }
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::RemoveParticlesFromScene()
{
   RemoveParticlesFromScene(mpExplosionParticles);
   RemoveParticlesFromScene(mpProjectileParticles);
   RemoveParticlesFromScene(mpImpactParticles);
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::RemoveParticlesFromScene(const std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& particles)
{
   for (size_t effectIndex = 0; effectIndex < particles.size(); ++effectIndex)
   {
      GetGameManager()->GetScene().RemoveDrawable(particles[effectIndex]);
   }
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::PlayEffects(const SMK::Damage& damage)
{
   switch (damage.GetDamageType())
   {
      case SMK::Damage::DAMAGE_BLAST:
      {
         PlayNextEffect(damage.GetLocation(), mpExplosionSounds, mpExplosionParticles, mCurrentExplosionIndex);
         break;
      }
      case SMK::Damage::DAMAGE_PROJECTILE:
      {
         PlayNextEffect(damage.GetLocation(), mpProjectileSounds, mpProjectileParticles, mCurrentProjectileIndex);
         break;
      }
      case SMK::Damage::DAMAGE_IMPACT:
      {
         PlayNextEffect(damage.GetLocation(), mpImpactSounds, mpImpactParticles, mCurrentImpactIndex);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void ImpactEffectsComponent::PlayNextEffect(const osg::Vec3& position,
   const std::vector< dtCore::RefPtr<dtAudio::Sound> >& sounds,
   const std::vector< dtCore::RefPtr<dtCore::ParticleSystem> >& particles, int& effectIndex)
{
   assert(effectIndex < int(sounds.size()) && effectIndex < int(particles.size()));

   // Play sound
   sounds[effectIndex]->SetPosition(position);
   sounds[effectIndex]->Play();

   // Show particle
   dtCore::Transform currentTransform;
   currentTransform.SetTranslation(position);
   particles[effectIndex]->SetTransform(currentTransform);
   particles[effectIndex]->ResetTime();
   particles[effectIndex]->SetEnabled(true);

   // Increment index to use next stored effect next play
   ++effectIndex;
   effectIndex %= int(sounds.size());
}

////////////////////////////////////////////////////////////////////////////////
