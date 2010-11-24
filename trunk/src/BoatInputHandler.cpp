#include <BoatInputHandler.h>
#include <BoatActors/Outboard.h>

#include <dtCore/odebodywrap.h>
#include <dtCore/transform.h>
#include <dtCore/physical.h>
#include <dtCore/timer.h>

#include <dtCore/keyboard.h>
#include <dtCore/mouse.h>

#include <dtCore/logicalinputdevice.h>
#include <dtCore/inputmapper.h>
#include <dtInputPLIB/joystick.h>

static const int kCutEnginesButton = 0xAAAA;
static const int kRollButton       = 0xAAAB;
static const int KBUTTON3 = 0xAAAC;

static const int KAXIS1 = 0xBBBA;

//////////////////////////////////////////////////////////////////////////
BoatInputHandler::BoatInputHandler(dtCore::Keyboard* keyboard, dtCore::Mouse* mouse)
   : mpOutboard(NULL)
   , mpKeyboard(keyboard)
{
   // setup keyboard axis input device
   {
      mKeyboardAxisInputDevice = new dtCore::LogicalInputDevice;

      mKeyboardAxisInputDevice->AddAxis(
         "w/s",
         new dtCore::ButtonsToAxis(
            mpKeyboard->GetButton('w'),
            mpKeyboard->GetButton('s')
         )
      );

      mKeyboardAxisInputDevice->AddAxis(
         "a/d",
         new dtCore::ButtonsToAxis(
            mpKeyboard->GetButton('a'),
            mpKeyboard->GetButton('d')
         )
      );

      mKeyboardAxisInputDevice->AddButton(
         "cut engines",
         keyboard->GetButton(' '),
         kCutEnginesButton
      );
   }

   // setup indirect input device
   {
      mApplicationInputDevice = new dtCore::LogicalInputDevice;

      mApplicationInputDevice->AddButton(
         "cut engines",
         keyboard->GetButton('1'),
         kCutEnginesButton
      );

      mApplicationInputDevice->AddButton(
         "action 2",
         keyboard->GetButton('2'),
         kRollButton
      );

      //*
      mApplicationInputDevice->AddAxis(
         "axis 1",
         mKeyboardAxisInputDevice->AddAxis(
            "up/down",
            new dtCore::ButtonsToAxis(
               keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Up),
               keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Down)
            )
         )
      );

      mApplicationInputDevice->AddAxis(
         "axis 2",
         mKeyboardAxisInputDevice->AddAxis(
            "left/right",
            new dtCore::ButtonsToAxis(
               keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Left),
               keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Right)
            )
         )
      );
      //*/
   }
   mInputMapper = new dtCore::InputMapper;

   mInputMapper->AddDevice(keyboard);
   mInputMapper->AddDevice(mKeyboardAxisInputDevice.get());
   mInputMapper->AddDevice(mouse);

   mInputMapper->SetCancelButton(keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Escape));

   // initialize input
   dtInputPLIB::Joystick::CreateInstances();

   //*
   MapKeyboardControls();
   /*/
   MapGamepadControls();
   //*/
}

//////////////////////////////////////////////////////////////////////////
BoatInputHandler::~BoatInputHandler()
{
}

void BoatInputHandler::MapKeyboardControls()
{
   // map keyboard controls
   for (size_t axisIdx = 0; axisIdx < 2; ++axisIdx)
   {
      dtCore::LogicalAxis* axis = static_cast<dtCore::LogicalAxis*>(mApplicationInputDevice->GetAxis(axisIdx));
      dtCore::AxisMapping* mapping = new dtCore::AxisToAxis(mKeyboardAxisInputDevice->GetAxis(axisIdx));
      axis->SetMapping(mapping);
   }

   // cut engines button
   {
      dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kCutEnginesButton));
      dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(mKeyboardAxisInputDevice->GetButton(kCutEnginesButton));
      button->SetMapping(mapping);
   }

   // roll button
   {
      dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kRollButton));
      dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(mKeyboardAxisInputDevice->GetButton(kRollButton));
      button->SetMapping(mapping);
   }
}

void BoatInputHandler::MapGamepadControls()
{
   for (int i = 0; i < dtInputPLIB::Joystick::GetInstanceCount(); i++)
   {
      mInputMapper->AddDevice(dtInputPLIB::Joystick::GetInstance(i));
   }

   // map gamepad controls
   {
      dtCore::LogicalAxis* axis;
      dtCore::AxisMapping* mapping;

      axis = static_cast<dtCore::LogicalAxis*>(mApplicationInputDevice->GetAxis(0));
      mapping = new dtCore::AxisToAxis(dtInputPLIB::Joystick::GetInstance(0)->GetAxis(1));
      axis->SetMapping(mapping);

      axis = static_cast<dtCore::LogicalAxis*>(mApplicationInputDevice->GetAxis(1));
      mapping = new dtCore::AxisToAxis(dtInputPLIB::Joystick::GetInstance(0)->GetAxis(0));
      axis->SetMapping(mapping);

      // cut engines button (X)
      {
         dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kCutEnginesButton));
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(dtInputPLIB::Joystick::GetInstance(0)->GetButton(14));
         button->SetMapping(mapping);
      }

      // roll button (O)
      {
         dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kRollButton));
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(dtInputPLIB::Joystick::GetInstance(0)->GetButton(13));
         button->SetMapping(mapping);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
bool BoatInputHandler::Update()
{
   const dtCore::Keyboard* keyboard = mpKeyboard;
   int key = 0;

   if (!mpOutboard.valid())
   {
      return false;
   }

   dtInputPLIB::Joystick::PollInstances();

   bool handled = true;

   const float forwardAxis = mApplicationInputDevice->GetAxis(0)->GetState();
   if (forwardAxis > 0.5f)
   {
      mpOutboard->ActivateThrust(false);
   }
   else if (forwardAxis < -0.5f)
   {
      mpOutboard->ActivateThrust(true);
   }
   else
   {
      mpOutboard->DeactivateThrust();
   }

   const float turningAxis = mApplicationInputDevice->GetAxis(1)->GetState();
   if (turningAxis > 0.5f)
   {
      mpOutboard->ActivateTurning(false);
   }
   else if (turningAxis < -0.5f)
   {
      mpOutboard->ActivateTurning(true);
   }
   else
   {
      mpOutboard->DeactivateTurning();
   }

   if (mApplicationInputDevice->GetButton(kCutEnginesButton)->GetState())
   {
      mpOutboard->CutEngine();
   }

   if (mApplicationInputDevice->GetButton(kRollButton)->GetState())
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

   return handled;
}

//////////////////////////////////////////////////////////////////////////
void BoatInputHandler::SetOutboard(Outboard* outboard)
{
   mpOutboard = outboard;
}

////////////////////////////////////////////////////////////////////////////////