#include <actors/FrontWeapon.h>

#include <messages/NetworkMessages.h>

#include <dtGame/gamemanager.h>

//////////////////////////////////////////////////////////////////////////
FrontWeapon::FrontWeapon(const std::string& filename /*= ""*/)
: Weapon(filename)
{
}

//////////////////////////////////////////////////////////////////////////
FrontWeapon::~FrontWeapon()
{
}

///////////////////////////////////////////////////////////////////////////////
void FrontWeapon::FireWeapon()
{
   Weapon::FireWeapon();

   if (mpSMKBoatActorProxy.valid() && !mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      dtCore::RefPtr<dtGame::Message> fireMessage;
      mpSMKBoatActorProxy->GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_FRONT_WEAPON_FIRED, fireMessage);
      fireMessage->SetAboutActorId(mpSMKBoatActorProxy->GetGameActor().GetUniqueId());
      mpSMKBoatActorProxy->GetGameManager()->SendNetworkMessage(*fireMessage);
   }
}

////////////////////////////////////////////////////////////////////////////////
