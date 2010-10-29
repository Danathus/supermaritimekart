#include "BoatKeyboardListener.h"
#include <BoatActors/Outboard.h>

//////////////////////////////////////////////////////////////////////////
BoatKeyboardListener::BoatKeyboardListener()
   : mOutboard(NULL)
{
}

//////////////////////////////////////////////////////////////////////////
BoatKeyboardListener::~BoatKeyboardListener()
{
}

//////////////////////////////////////////////////////////////////////////
bool BoatKeyboardListener::HandleKeyPressed(const dtCore::Keyboard* keyboard, int key)
{
   if (!mOutboard.valid())
   {
      return false;
   }

   bool handled = true;

   switch(key)
   {
   case 'w':
   case 'W':
      mOutboard->ActivateThrust(true);
   	break;

   case 's':
   case 'S':
      mOutboard->ActivateThrust(false);
      break;

   case 'a':
   case 'A':
      mOutboard->ActivateTurning(true);
      break;

   case 'd':
   case 'D':
      mOutboard->ActivateTurning(false);
      break;

   case ' ':
      mOutboard->CutEngine();
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
   if (!mOutboard.valid())
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
      mOutboard->DeactivateThrust();
      break;

   case 'a':
   case 'A':
   case 'd':
   case 'D':
      mOutboard->DeactivateTurning();
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
   mOutboard = outboard;
}

////////////////////////////////////////////////////////////////////////////////
