#include <BoatKeyboardListener.h>
#include <BoatActors/Outboard.h>

#include <dtCore/odebodywrap.h>
#include <dtCore/transform.h>
#include <dtCore/physical.h>
#include <dtCore/timer.h>

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

   case 'r':
   case 'R':
      {
         dtCore::Physical* boat = mpOutboard->GetObject();  

         dtCore::Transform boatTransform;
         boat->GetTransform(boatTransform);

         float yaw, pitch, roll;
         boatTransform.GetRotation(yaw, pitch, roll);

         // Only allow applying a torque when the boat is flipped
         if (fabs(roll) > 90)
         {
            static dtCore::Timer_t lastTime = dtCore::Timer::Instance()->Tick();

            const dtCore::Timer* timer = dtCore::Timer::Instance();
            dtCore::Timer_t currentTime = timer->Tick();

            if (timer->DeltaSec(lastTime, currentTime) > 2.0f)
            {          
               boat->GetBodyWrapper()->SetLinearVelocity(osg::Vec3());
               boat->GetBodyWrapper()->SetAngularVelocity(osg::Vec3());
               boat->GetBodyWrapper()->ApplyRelTorque(osg::Vec3(0.0f, 200000.0f, 0.0f));
               lastTime = currentTime;
            }         

            /*float h, p, r;
            boatTransform.GetRotation(h, p, r);
            boatTransform.SetRotation(h, p, r + 180.0f);

            mpOutboard->GetObject()->SetTransform(boatTransform);*/
         }
      }
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
