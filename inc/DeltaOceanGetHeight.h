#ifndef DeltaOceanGetHeight_h__
#define DeltaOceanGetHeight_h__

#include <BoatActors/IGetHeight.h>

#include <dtOcean/oceanactor.h>
#include <dtCore/refptr.h>

class DeltaOceanGetHeight : public IGetHeight
{
public:
   DeltaOceanGetHeight(dtOcean::OceanActor& oceanActor);

   virtual float GetHeight(float x, float y);

protected:
   virtual ~DeltaOceanGetHeight(void);

private:
   dtCore::RefPtr<dtOcean::OceanActor> mOceanActor;

};

#endif // DeltaOceanGetHeight_h__
