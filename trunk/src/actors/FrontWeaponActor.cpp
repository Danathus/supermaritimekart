#include <actors/FrontWeaponActor.h>

#include <messages/NetworkMessages.h>

#include <dtGame/gamemanager.h>

//////////////////////////////////////////////////////////////////////////
FrontWeaponActor::FrontWeaponActor(const std::string& filename /*= ""*/)
: WeaponActor(filename)
{
}

//////////////////////////////////////////////////////////////////////////
FrontWeaponActor::~FrontWeaponActor()
{
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeaponActor::FireWeapon()
{
   WeaponActor::FireWeapon();

   if (mpSMKBoatActorProxy.valid() && !mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      dtCore::RefPtr<dtGame::Message> fireMessage;
      mpSMKBoatActorProxy->GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_FRONT_WEAPON_FIRED, fireMessage);
      fireMessage->SetAboutActorId(mpSMKBoatActorProxy->GetGameActor().GetUniqueId());
      mpSMKBoatActorProxy->GetGameManager()->SendNetworkMessage(*fireMessage);
   }
}

////////////////////////////////////////////////////////////////////////////////
