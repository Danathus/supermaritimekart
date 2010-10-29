#ifndef BoatFloaterComponent_h__
#define BoatFloaterComponent_h__

#include <dtGame/gmcomponent.h>

class FloatingActor;

class BoatFloaterComponent : public dtGame::GMComponent
{
public:
	BoatFloaterComponent();

   virtual void ProcessMessage(const dtGame::Message& message);

protected:
   virtual ~BoatFloaterComponent();

private:
   std::vector<FloatingActor*> GetFloatingActors() const;
};
#endif // BoatFloaterComponent_h__

