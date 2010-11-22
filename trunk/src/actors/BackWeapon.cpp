#include <actors/BackWeapon.h>

#include <messages/NetworkMessages.h>

#include <dtGame/gamemanager.h>

//////////////////////////////////////////////////////////////////////////
BackWeapon::BackWeapon(const dtDAL::ResourceDescriptor& resource)
: Weapon(resource)
{
}

//////////////////////////////////////////////////////////////////////////
BackWeapon::~BackWeapon()
{
}

///////////////////////////////////////////////////////////////////////////////
void BackWeapon::FireWeapon()
{
   Weapon::FireWeapon();

   if (mpSMKBoatActorProxy.valid() && !mpSMKBoatActorProxy->GetGameActor().IsRemote())
   {
      dtCore::RefPtr<dtGame::Message> fireMessage;
      mpSMKBoatActorProxy->GetGameManager()->GetMessageFactory().CreateMessage(SMK::SMKNetworkMessages::ACTION_BACK_WEAPON_FIRED, fireMessage);
      fireMessage->SetAboutActorId(mpSMKBoatActorProxy->GetGameActor().GetUniqueId());
      mpSMKBoatActorProxy->GetGameManager()->SendNetworkMessage(*fireMessage);
   }
}

////////////////////////////////////////////////////////////////////////////////
