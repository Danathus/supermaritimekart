#include <BoatKeyboardListener.h>
#include <BoatActors/Outboard.h>

//////////////////////////////////////////////////////////////////////////
BoatKeyboardListener::BoatKeyboardListener()
   : mpOutboard(NULL)
{
}

//////////////////////////////////////////////////////////////////////////
BoatKeyboardListener::~BoatKeyboardListener()
{
}

//////////////////////////////////////////////////////////////////////////
bool BoatKeyboardListener::HandleKeyPressed(const dtCore::Keyboard* keyboard, int key)
{
   if (!mpOutboard.valid())
   {
      return false;
   }

   bool handled = true;

   switch(key)
   {
   case 'w':
   case 'W':
      mpOutboard->ActivateThrust(true);
        break;

   case 's':
   case 'S':
      mpOutboard->ActivateThrust(false);
      break;

   case 'a':
   case 'A':
      mpOutboard->ActivateTurning(true);
      break;

   case 'd':
   case 'D':
      mpOutboard->ActivateTurning(false);
      break;

   case ' ':
      mpOutboard->CutEngine();
      break;

   default:
      handled = false;
      break;
   }

   return handled;
}

//////////////////////////////////////////////////////////////////////////
bool BoatKeyboardListener::HandleKeyReleased(const dtCore::Keyboard* keyboard, int key)
{
   if (!mpOutboard.valid())
   {
      return false;
   }

   bool handled = true;

   switch(key)
   {
   case 'w':
   case 'W':
   case 's':
   case 'S':
      mpOutboard->DeactivateThrust();
      break;

   case 'a':
   case 'A':
   case 'd':
   case 'D':
      mpOutboard->DeactivateTurning();
      break;

   default:
      handled = false;
      break;
   }

   return handled;
}

//////////////////////////////////////////////////////////////////////////
bool BoatKeyboardListener::HandleKeyTyped(const dtCore::Keyboard* keyboard, int kc)
{
   return false;
}

//////////////////////////////////////////////////////////////////////////
void BoatKeyboardListener::SetOutboard(Outboard* outboard)
{
   mpOutboard = outboard;
}

////////////////////////////////////////////////////////////////////////////////
