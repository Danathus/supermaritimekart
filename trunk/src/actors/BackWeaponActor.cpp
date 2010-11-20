#include <actors/BackWeaponActor.h>

#include <messages/NetworkMessages.h>

#include <dtGame/gamemanager.h>

//////////////////////////////////////////////////////////////////////////
BackWeaponActor::BackWeaponActor(const std::string& filename /*= ""*/)
: WeaponActor(filename)
{
}

//////////////////////////////////////////////////////////////////////////
BackWeaponActor::~BackWeaponActor()
{
}

///////////////////////////////////////////////////////////////////////////////
void BackWeaponActor::FireWeapon()
{
   WeaponActor::FireWeapon();

   if (mpSMKBoatActorProxy.valid() && !mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      dtCore::RefPtr<dtGame::Message> fireMessage;
      mpSMKBoatActorProxy->GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_BACK_WEAPON_FIRED, fireMessage);
      fireMessage->SetAboutActorId(mpSMKBoatActorProxy->GetGameActor().GetUniqueId());
      mpSMKBoatActorProxy->GetGameManager()->SendNetworkMessage(*fireMessage);
   }
}

////////////////////////////////////////////////////////////////////////////////
