#ifndef FloaterController_h__
#define FloaterController_h__

#include <dtGame/gmcomponent.h>

////////////////////////////////////////////////////////////////////////////////
class FloatingActor;

namespace dtOcean
{
   class OceanActor;
}

////////////////////////////////////////////////////////////////////////////////

class FloaterController : public dtGame::GMComponent
{
public:
   FloaterController();

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~FloaterController();

private:
   void SetupFloater(FloatingActor* floater, dtOcean::OceanActor* ocean);
   void CleanupFloater(FloatingActor* floater);

   std::vector<FloatingActor*> GetFloatingActors() const;
   dtOcean::OceanActor* GetOcean() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif //FloaterController_h__
