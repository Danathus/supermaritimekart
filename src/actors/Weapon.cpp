#include <actors/Weapon.h>

#include <dtAudio/audiomanager.h>
#include <dtUtil/log.h>

////////////////////////////////////////////////////////////////////////////////
const std::string Weapon::WEAPON_ACTOR_TYPE = "Weapon";

//////////////////////////////////////////////////////////////////////////
Weapon::Weapon(const std::string& filename /*= ""*/)
   : dtCore::Object(WEAPON_ACTOR_TYPE)
   , mIsFiring(false)
   , mFiringRate(1.0f)
   , mpStartFiringSound(NULL)
   , mpFireSound(NULL)
   , mpStopFiringSound(NULL)
{
   // Load our mesh
   if (!filename.empty())
   {
      if (LoadFile(filename) == NULL)
      {
         LOG_ERROR("Unable to load mesh: " + filename);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
Weapon::~Weapon()
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
void Weapon::SetFiringRate(float rate)
{
   mFiringRate = rate;
   if (mFiringRate != 0)
   {
      mFiringCounter = (1.0f / mFiringRate);
   }
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::Update(float deltaTime)
{
   mFiringCounter += deltaTime;

   if (mIsFiring)
   {
      // Convert firing rate from number of shots per second to number of seconds per shot
      if (mFiringRate == 0.0f || mFiringCounter > (1.0f / mFiringRate))
      {
         mFiringCounter = 0.0f;
         FireWeapon();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::StartWeaponFiring()
{
   // Play weapon start sounds and effects
   if (mpStartFiringSound.valid())
   {
      mpStartFiringSound->Play();
   }

   SetIsFiring(true);
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::FireWeapon()
{
   // Play weapon fire sounds and effects
   if (mpFireSound.valid())
   {
      mpFireSound->Play();
   }
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::StopWeaponFiring()
{
   // Play weapon stop sounds and effects
   if (mpStopFiringSound.valid())
   {
      mpStopFiringSound->Play();
   }

   SetIsFiring(false);
}

///////////////////////////////////////////////////////////////////////////////
void Weapon::SetIsFiring(bool isFiring)
{
   mIsFiring = isFiring;
}

///////////////////////////////////////////////////////////////////////////////
dtAudio::Sound* Weapon::LoadSound(const std::string& file)
{
   if (!file.empty())
   {
      dtAudio::Sound* sound = dtAudio::AudioManager::GetInstance().NewSound();
      sound->LoadFile(file.c_str());
      // we want this sound to follow the listener, so it is not attenuated
      sound->SetListenerRelative(true);

      return sound;
   }

   return NULL;
}

////////////////////////////////////////////////////////////////////////////////