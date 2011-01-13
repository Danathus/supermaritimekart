#include <BoatInputHandler.h>
#include <BoatActors/Outboard.h>
#include <actors/WeaponSlot.h>

#include <dtCore/odebodywrap.h>
#include <dtCore/transform.h>
#include <dtCore/physical.h>
#include <dtCore/timer.h>

#include <dtCore/keyboard.h>
#include <dtCore/mouse.h>

#include <dtCore/logicalinputdevice.h>
#include <dtCore/inputmapper.h>
#include <dtInputPLIB/joystick.h>

#include <sstream>

enum ButtonNames
{
   kFirstButtonName     = 0xAAAA,
   kCutEnginesButton    = kFirstButtonName,
   kRollButton,
   kFirePrimaryWeapon,
   kFireSecondaryWeapon,

   kMaxButtons
};

// 0 is triangle
// 1 is circle
// 2 engines is X
// 3 is square
// 4 is L2
// 5 is R2
// 6 is L1
// 7 is R1
// 8 is select
// 9 is start
// 10 is left stick in
// 11 is right stick in
static const struct
{
   int firePrimary, fireSecondary, cutEngines, roll;
} sgkControllerButtonMappings =
{
   5, // firePrimary   (formerly 12)
   4, // fireSecondary (formerly 11)
   1, // cutEngines    (formerly 14)
   0  // roll          (formerly 13)
};

#define TEST_CONTROLLER 0

//////////////////////////////////////////////////////////////////////////
BoatInputHandler::BoatInputHandler(dtCore::Keyboard* keyboard, dtCore::Mouse* mouse)
   : mpOutboard(NULL)
   , mpKeyboard(keyboard)
{
   // setup keyboard axis input device
   {
      mKeyboardMouseInputDevice = new dtCore::LogicalInputDevice;

      mKeyboardMouseInputDevice->AddAxis(
         "w/s",
         new dtCore::ButtonsToAxis(
            mpKeyboard->GetButton('w'),
            mpKeyboard->GetButton('s')
         )
      );

      mKeyboardMouseInputDevice->AddAxis(
         "a/d",
         new dtCore::ButtonsToAxis(
            mpKeyboard->GetButton('a'),
            mpKeyboard->GetButton('d')
         )
      );

      mKeyboardMouseInputDevice->AddButton(
         "cut engines",
         keyboard->GetButton(' '),
         kCutEnginesButton
      );

      mKeyboardMouseInputDevice->AddButton(
         "roll",
         keyboard->GetButton('r'),
         kRollButton
      );

      mKeyboardMouseInputDevice->AddButton(
         "fire primary",
         mouse->GetButton(0),
         kFirePrimaryWeapon
      );

      mKeyboardMouseInputDevice->AddButton(
         "fire secondary",
         mouse->GetButton(2),
         kFireSecondaryWeapon
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
         "roll",
         keyboard->GetButton('2'),
         kRollButton
      );

      mApplicationInputDevice->AddButton(
         "fire primary",
         keyboard->GetButton('3'),
         kFirePrimaryWeapon
      );

      mApplicationInputDevice->AddButton(
         "fire secondary",
         keyboard->GetButton('4'),
         kFireSecondaryWeapon
      );

      //*
      mApplicationInputDevice->AddAxis(
         "axis 1",
         mKeyboardMouseInputDevice->AddAxis(
            "up/down",
            new dtCore::ButtonsToAxis(
               keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Up),
               keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Down)
            )
         )
      );

      mApplicationInputDevice->AddAxis(
         "axis 2",
         mKeyboardMouseInputDevice->AddAxis(
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
   mInputMapper->AddDevice(mKeyboardMouseInputDevice.get());
   mInputMapper->AddDevice(mouse);

   mInputMapper->SetCancelButton(keyboard->GetButton(osgGA::GUIEventAdapter::KEY_Escape));

   // initialize input
   dtInputPLIB::Joystick::CreateInstances();

   if (dtInputPLIB::Joystick::GetInstanceCount() > 0)
   {
      MapGamepadControls();
   }
   else
   {
      MapKeyboardControls();
   }
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
      dtCore::AxisMapping* mapping = new dtCore::AxisToAxis(mKeyboardMouseInputDevice->GetAxis(axisIdx));
      axis->SetMapping(mapping);
   }

   for (size_t buttonID = kFirstButtonName; buttonID < kMaxButtons; ++buttonID)
   {
      dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(buttonID));
      dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(mKeyboardMouseInputDevice->GetButton(buttonID));
      button->SetMapping(mapping);
   }
}

void BoatInputHandler::MapGamepadControls()
{
   printf(">>> mapping gamepad controls -- founding %d joysticks\n", dtInputPLIB::Joystick::GetInstanceCount());
   for (int joyID = 0; joyID < dtInputPLIB::Joystick::GetInstanceCount(); joyID++)
   {
      dtInputPLIB::Joystick* joystick = dtInputPLIB::Joystick::GetInstance(joyID);
      printf(">>> controller %d has %d axes, %d buttons\n", joyID, joystick->GetAxisCount(), joystick->GetButtonCount());
      mInputMapper->AddDevice(joystick);

#if TEST_CONTROLLER
      for (size_t axisIdx = 0; axisIdx < joystick->GetAxisCount(); ++axisIdx)
      {
         while (axisIdx >= mApplicationInputDevice->GetAxisCount())
         {
            std::stringstream strstrm;
            strstrm << "axis";
            strstrm << int(axisIdx);
            mApplicationInputDevice->AddAxis(strstrm.str());
         }
         dtCore::LogicalAxis* axis = static_cast<dtCore::LogicalAxis*>(mApplicationInputDevice->GetAxis(axisIdx));
         dtCore::AxisMapping* mapping = new dtCore::AxisToAxis(joystick->GetAxis(axisIdx));
         axis->SetMapping(mapping);
      }
      /*
      for (size_t buttonIdx = 0; buttonIdx < joystick->GetButtonCount(); ++buttonIdx)
      {
         while (buttonIdx >= mApplicationInputDevice->GetButtonCount())
         {
            std::stringstream strstrm;
            strstrm << "button";
            strstrm << int(buttonIdx);
            mApplicationInputDevice->AddButton(strstrm.str(), buttonIdx);
         }
         dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(buttonIdx));
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(joystick->GetButton(buttonIdx));
         button->SetMapping(mapping);
      }
      //*/
#endif
   }

#if !TEST_CONTROLLER
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
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(dtInputPLIB::Joystick::GetInstance(0)->GetButton(sgkControllerButtonMappings.cutEngines));
         button->SetMapping(mapping);
      }

      // roll button (O)
      {
         dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kRollButton));
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(dtInputPLIB::Joystick::GetInstance(0)->GetButton(sgkControllerButtonMappings.roll));
         button->SetMapping(mapping);
      }

      // fire primary (/\) (todo: find correct button number)
      {
         dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kFirePrimaryWeapon));
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(dtInputPLIB::Joystick::GetInstance(0)->GetButton(sgkControllerButtonMappings.firePrimary));
         button->SetMapping(mapping);
      }

      // fire secondary ([]) (todo: find correct button number)
      {
         dtCore::LogicalButton* button = static_cast<dtCore::LogicalButton*>(mApplicationInputDevice->GetButton(kFireSecondaryWeapon));
         dtCore::ButtonMapping* mapping = new dtCore::ButtonToButton(dtInputPLIB::Joystick::GetInstance(0)->GetButton(sgkControllerButtonMappings.fireSecondary));
         button->SetMapping(mapping);
      }
   }
#endif
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

#if TEST_CONTROLLER
   dtInputPLIB::Joystick* joystick = dtInputPLIB::Joystick::GetInstance(0);
   for (size_t axisIdx = 0; axisIdx < joystick->GetAxisCount(); ++axisIdx)
   {
      const float axisValue = mApplicationInputDevice->GetAxis(axisIdx)->GetState();
      printf("%2.2f, ", axisValue);
   }
   printf("\n");
   return handled;
#endif

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
            boat->GetBodyWrapper()->SetLinearVelocity(osg::Vec3(0, 0, 0));
            boat->GetBodyWrapper()->SetAngularVelocity(osg::Vec3(0, 0, 0));
            //boat->GetBodyWrapper()->ApplyRelTorque(osg::Vec3(0.0f, 10000000.0f, 0.0f));
            boat->GetBodyWrapper()->ApplyRelTorque(osg::Vec3(0.0f, 100000.0f, 0.0f));
            lastTime = currentTime;
         }

         /*float h, p, r;
         boatTransform.GetRotation(h, p, r);
         boatTransform.SetRotation(h, p, r + 180.0f);

         mpOutboard->GetObject()->SetTransform(boatTransform);*/
      }
   }

   if (mpPrimaryWeapon->GetWeapon() != NULL)
   {
      if (mApplicationInputDevice->GetButton(kFirePrimaryWeapon)->GetState() &&
         !mpPrimaryWeapon->GetWeapon()->IsFiring())
      {
         mpPrimaryWeapon->StartWeaponFire();
      }
      else if (!mApplicationInputDevice->GetButton(kFirePrimaryWeapon)->GetState() &&
         mpPrimaryWeapon->GetWeapon()->IsFiring())
      {
         mpPrimaryWeapon->StopWeaponFire();
      }
   }

   if (mpSecondaryWeapon->GetWeapon() != NULL)
   {
      if (mApplicationInputDevice->GetButton(kFireSecondaryWeapon)->GetState() &&
         !mpSecondaryWeapon->GetWeapon()->IsFiring())
      {
         mpSecondaryWeapon->StartWeaponFire();
      }
      else if (!mApplicationInputDevice->GetButton(kFireSecondaryWeapon)->GetState() &&
         mpSecondaryWeapon->GetWeapon()->IsFiring())
      {
         mpSecondaryWeapon->StopWeaponFire();
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
