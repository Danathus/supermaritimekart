#ifndef OsgOceanGetHeight_h__
#define OsgOceanGetHeight_h__

#include <BoatActors/IGetHeight.h>
#include <dtOcean/oceanactor.h>
#include <dtCore/refptr.h>

class OsgOceanGetHeight : public IGetHeight
{
public:
   OsgOceanGetHeight(dtOcean::OceanActor& oceanActor);

   virtual float GetHeight(float x, float y);

protected:
   virtual ~OsgOceanGetHeight(void);

private:
   dtCore::RefPtr<dtOcean::OceanActor> mOceanActor;

};
 
#endif // OsgOceanGetHeight_h__
