#ifndef BoatKeyboardListener_h__
#define BoatKeyboardListener_h__

#include <dtCore/keyboard.h>

class Outboard;

class BoatKeyboardListener : public dtCore::KeyboardListener
{
public:
   BoatKeyboardListener();
   void SetOutboard(Outboard* outboard);
   Outboard* GetOutboard() { return mpOutboard.get(); }

   bool HandleKeyPressed(const dtCore::Keyboard* keyboard, int key);
   virtual bool HandleKeyReleased(const dtCore::Keyboard* keyboard, int kc);
   virtual bool HandleKeyTyped(const dtCore::Keyboard* keyboard, int kc );

protected:
   virtual ~BoatKeyboardListener();

private:

   dtCore::RefPtr<Outboard> mpOutboard;
};
#endif // BoatKeyboardListener_h__