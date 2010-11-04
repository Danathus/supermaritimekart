#ifndef CameraMouseListener_h__
#define CameraMouseListener_h__

#include <dtCore/mouse.h>
namespace dtCore
{
   class Mouse;
}

class CameraMouseListener : public dtCore::MouseListener
{
public:
	CameraMouseListener();
   virtual bool HandleMouseScrolled(const dtCore::Mouse* mouse, int delta);
   virtual bool HandleButtonPressed(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button);
   virtual bool HandleButtonReleased(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button);
   virtual bool HandleButtonClicked(const dtCore::Mouse* mouse, dtCore::Mouse::MouseButton button, int clickCount);
   virtual bool HandleMouseMoved(const dtCore::Mouse* mouse, float x, float y);
   virtual bool HandleMouseDragged(const dtCore::Mouse* mouse, float x, float y);
   
   int GetZoomFactor() const;
   void SetZoomFactor(int val);

   float GetSideOffset() const;
   void SetSideOffset(float val);

   void Reset();

protected:
   virtual ~CameraMouseListener();
	
private:
   int mZoomFactor;
   float mSideOffset;
   float mLastMouseX;
};
#endif // CameraMouseListener_h__