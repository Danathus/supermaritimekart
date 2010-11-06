#ifndef OceanWindowResize_h__
#define OceanWindowResize_h__

#include <dtCore/deltawin.h>
#include <dtCore/windowresizecallback.h>

namespace dtOcean
{
   class OceanActor;
}

///Little class used to catch DeltaWin resize events and tell dtOcean about them
class OceanWindowResize : public dtCore::WindowResizeCallback
{
public:
   OceanWindowResize(dtOcean::OceanActor* ocean = NULL);

   virtual void operator () (const dtCore::DeltaWin& win, int x, int y,
      int width, int height);

   void SetOceanActor(dtOcean::OceanActor* ocean);

protected:
   virtual ~OceanWindowResize();

private:
   dtCore::RefPtr<dtOcean::OceanActor> mOcean;
};
#endif // OceanWindowResize_h__