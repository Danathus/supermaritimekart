#include <messages/DamageMessage.h>

#include <dtUtil/datastream.h>

////////////////////////////////////////////////////////////////////////////////
SMK::DamageMessage::DamageMessage()
: dtGame::Message()
{
}

////////////////////////////////////////////////////////////////////////////////
void SMK::DamageMessage::ToString(std::string& toFill) const
{
   dtGame::Message::ToString(toFill);
   std::stringstream strstrm;
   strstrm << mDamage.GetAmount() << mDamage.GetLocation().x() << 
      mDamage.GetLocation().y() << mDamage.GetLocation().z() <<
      mDamage.GetRadius() << mDamage.GetDamageType();
   toFill = toFill + strstrm.str();
}

////////////////////////////////////////////////////////////////////////////////
bool SMK::DamageMessage::FromString(const std::string& source)
{
   bool okay = true;

   okay = okay && dtGame::Message::FromString(source);
   if (okay)
   {
      std::stringstream stream;
      SMK::DamageAmountType amount;
      float x, y, z, radius;
      int type;
      stream << source;
      stream >> amount >> x >> y >> z >> radius >> type;
      mDamage.SetAmount(amount);
      mDamage.SetLocation(osg::Vec3(x, y, z));
      mDamage.SetRadius(radius);
      mDamage.SetDamageType(SMK::Damage::DamageType(type));
   }

   return okay;
}

////////////////////////////////////////////////////////////////////////////////
void SMK::DamageMessage::ToDataStream(dtUtil::DataStream& stream) const
{
   dtGame::Message::ToDataStream(stream);
   stream << mDamage.GetAmount() << mDamage.GetLocation().x() << 
      mDamage.GetLocation().y() << mDamage.GetLocation().z() <<
      mDamage.GetRadius() << mDamage.GetDamageType();
}

////////////////////////////////////////////////////////////////////////////////
bool SMK::DamageMessage::FromDataStream(dtUtil::DataStream& stream)
{
   bool okay = true;

   okay = okay && dtGame::Message::FromDataStream(stream);
   if (okay)
   {
      SMK::DamageAmountType amount;
      float x, y, z, radius;
      int type;
      stream >> amount >> x >> y >> z >> radius >> type;
      mDamage.SetAmount(amount);
      mDamage.SetLocation(osg::Vec3(x, y, z));
      mDamage.SetRadius(radius);
      mDamage.SetDamageType(SMK::Damage::DamageType(type));
   }

   return okay;
}

////////////////////////////////////////////////////////////////////////////////
