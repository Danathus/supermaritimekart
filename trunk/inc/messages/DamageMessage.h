#ifndef DamageMessage_h__
#define DamageMessage_h__

////////////////////////////////////////////////////////////////////////////////

#include <dtGame/message.h>
#include <messages/MessagesExport.h>

#include <util/Damage.h>

////////////////////////////////////////////////////////////////////////////////

namespace SMK
{
   class SMK_MESSAGE_EXPORT DamageMessage : public dtGame::Message
   {
   public:
      /// Constructor
      DamageMessage();

      void SetDamage(const Damage& damage) { mDamage = damage; }
      const Damage& GetDamage() const    { return mDamage; }

      // implementation of serialization suite
      void ToString(std::string& toFill) const;
      bool FromString(const std::string& source);
      void ToDataStream(dtUtil::DataStream& stream) const;
      bool FromDataStream(dtUtil::DataStream& stream);

   protected:
      /// Destructor
      virtual ~DamageMessage() {}

   private:
      Damage mDamage;
   };
} // namespace SMK

////////////////////////////////////////////////////////////////////////////////

#endif // DamageMessage_h__
