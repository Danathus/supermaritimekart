#include "CameraMouseListener.h"
#include <dtUtil/mathdefines.h>
#include <iostream>

const int kMIN_ZOOM = 1;
const int kMAX_ZOOM = 4;

const float kMIN_SIDE_OFFSET = -10.f;
const float kMAX_SIDE_OFFSET = 10.f;

////////////////////////////////////////////////////////////////////////////////
CameraMouseListener::CameraMouseListener():
mZoomFactor(0),
mSideOffset(0.f),
mLastMouseX(FLT_MAX)
{
   Reset();
}

////////////////////////////////////////////////////////////////////////////////
CameraMouseListener::~CameraMouseListener()
{

}

////////////////////////////////////////////////////////////////////////////////
bool CameraMouseListener::HandleMouseScrolled(const  dtCore::Mouse* mouse, int delta)
{
   mZoomFactor += delta;
   dtUtil::Clamp(mZoomFactor, kMIN_ZOOM, kMAX_ZOOM);

   return true;
}

////////////////////////////////////////////////////////////////////////////////
bool CameraMouseListener::HandleButtonPressed(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button)
{
   return false;  
}

////////////////////////////////////////////////////////////////////////////////
bool CameraMouseListener::HandleButtonReleased(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button)
{
   return false;  
}

////////////////////////////////////////////////////////////////////////////////
bool CameraMouseListener::HandleButtonClicked(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button, int clickCount)
{
   return false;  
}

////////////////////////////////////////////////////////////////////////////////
bool CameraMouseListener::HandleMouseMoved(const dtCore::Mouse* mouse, float x, float y)
{
   ///TODO kinda dorky - shouldn't be using the pixels coords directly
   UpdateLastMousePosition(x, y);

   return true;  
}

////////////////////////////////////////////////////////////////////////////////
bool CameraMouseListener::HandleMouseDragged(const dtCore::Mouse* mouse, float x, float y)
{
   ///TODO kinda dorky - shouldn't be using the pixels coords directly
   UpdateLastMousePosition(x, y);

   return true;  
}

////////////////////////////////////////////////////////////////////////////////
int CameraMouseListener::GetZoomFactor() const
{
   return mZoomFactor;
}

////////////////////////////////////////////////////////////////////////////////
void CameraMouseListener::SetZoomFactor(int val)
{
   mZoomFactor = val;
   dtUtil::Clamp(mZoomFactor, kMIN_ZOOM, kMAX_ZOOM);
}

////////////////////////////////////////////////////////////////////////////////
float CameraMouseListener::GetSideOffset() const
{
   return mSideOffset;
}

////////////////////////////////////////////////////////////////////////////////
void CameraMouseListener::SetSideOffset(float val)
{
   mSideOffset = val;
}

////////////////////////////////////////////////////////////////////////////////
void CameraMouseListener::Reset()
{
   mZoomFactor = 1;
   mSideOffset = 0.f;
   mLastMouseX = FLT_MAX;
}

///////////////////////////////////////////////////////////////////////////////
void CameraMouseListener::UpdateLastMousePosition(float x, float y)
{
   if (mLastMouseX == FLT_MAX)
   {
      mLastMouseX = x;
   }

   mSideOffset += (x - mLastMouseX);
   dtUtil::Clamp(mSideOffset, kMIN_SIDE_OFFSET, kMAX_SIDE_OFFSET);

   mLastMouseX = x;
}

///////////////////////////////////////////////////////////////////////////////
