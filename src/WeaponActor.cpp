#include <WeaponActor.h>

#include <dtAudio/audiomanager.h>
#include <dtUtil/log.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
const std::string WeaponActor::WEAPON_ACTOR_TYPE = "Weapon";

//////////////////////////////////////////////////////////////////////////
WeaponActor::WeaponActor(const std::string& filename /*= ""*/)
   : dtCore::Object(WEAPON_ACTOR_TYPE)
   , mIsFiring(false)
   , mFiringRate(1.0f)
{
   // Load our mesh
   if (!filename.empty())
   {
      if (LoadFile(filename) == NULL)
      {
         LOG_ERROR("Unable to load mesh: " + filename);
      }
   }

   // Load any sounds we have
   //mpStartFiringSound = LoadSound(start);
   //mpFireSound = LoadSound(fire);
   //mpStopFiringSound = LoadSound(end);
}

//////////////////////////////////////////////////////////////////////////
WeaponActor::~WeaponActor()
{
   if (mpStartFiringSound)
   {
      dtAudio::AudioManager::GetInstance().FreeSound(mpStartFiringSound);
      mpStartFiringSound = NULL;
   }
   if (mpFireSound)
   {
      dtAudio::AudioManager::GetInstance().FreeSound(mpFireSound);
      mpFireSound = NULL;
   }
   if (mpStopFiringSound)
   {
      dtAudio::AudioManager::GetInstance().FreeSound(mpStopFiringSound);
      mpStopFiringSound = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponActor::Update(float deltaTime)
{
   if (mIsFiring)
   {
      mFiringCounter += deltaTime;
      // Convert firing rate from number of shots per second to number of seconds per shot
      if (mFiringRate == 0.0f || mFiringCounter > (1.0f / mFiringRate))
      {
         mFiringCounter = 0.0f;
         FireWeapon();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponActor::StartWeaponFiring()
{
   // Play weapon start sounds and effects
   if (mpStartFiringSound.valid())
   {
      mpStartFiringSound->Play();
   }

   SetIsFiring(true);
}

///////////////////////////////////////////////////////////////////////////////
void WeaponActor::StopWeaponFiring()
{
   // Play weapon stop sounds and effects
   if (mpStopFiringSound.valid())
   {
      mpStopFiringSound->Play();
   }

   SetIsFiring(false);
}

///////////////////////////////////////////////////////////////////////////////
void WeaponActor::SetIsFiring(bool isFiring)
{
   mIsFiring = isFiring;
   if (mFiringRate != 0)
   {
      mFiringCounter = (1.0f / mFiringRate);
   }
}

///////////////////////////////////////////////////////////////////////////////
void WeaponActor::FireWeapon()
{
   // Play weapon fire sounds and effects
   if (mpFireSound.valid())
   {
      mpFireSound->Play();
   }

   std::cout << "Weapon fired" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
dtAudio::Sound* WeaponActor::LoadSound(const std::string& file)
{
   if (!file.empty())
   {
      dtAudio::Sound* sound = dtAudio::AudioManager::GetInstance().NewSound();
      sound->LoadFile(file.c_str());
      sound->SetGain(1.0f);
      sound->SetPitch(1.0f);
      // we want this sound to follow the listener, so it is not attenuated
      sound->SetListenerRelative(true);

      return sound;
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
dtCore::ParticleSystem* WeaponActor::LoadParticleSystem(const std::string& file)
{
   if (!file.empty())
   {
      dtCore::ParticleSystem* particleSystem = new dtCore::ParticleSystem();
      particleSystem->LoadFile(file);
      particleSystem->SetEnabled(GetActive());
      AddChild(particleSystem);
   }
   return NULL;
}

////////////////////////////////////////////////////////////////////////////////
