#ifndef PICKUPITEMBASE_h__
#define PICKUPITEMBASE_h__

#include <SMKActorExport.h>
#include <BoatActors/FloatingActor.h>
#include <dtUtil/getsetmacros.h>
#include <dtDAL/resourcedescriptor.h>

namespace SMK
{
   /** The PickUpItemHandle are used to place and render
     * where pickup-able items should be in the world.  When the player collides with these,
     * the Type and Category
     * can be queried.  If PickUp is useful, then the concrete PickUp can be 
     * instantiated with the PickUpFactory.
     */
   class SMK_ACTOR_EXPORT PickUpItemHandle : public FloatingActor
   {
   public:
      PickUpItemHandle(FloatingActorProxy& proxy);

      virtual void TickLocal(const dtGame::Message& msg);

   protected:
      virtual ~PickUpItemHandle();
   private:
      osg::ref_ptr<osg::Node> CreateGeometry();

      virtual void Initialize();
   };

   //////////////////////////////////////////////////////////////////////////
   class SMK_ACTOR_EXPORT PickUpItemBaseProxy : public FloatingActorProxy
   {
   public:
      PickUpItemBaseProxy();
      virtual ~PickUpItemBaseProxy();
      virtual void BuildPropertyMap();
      DT_DECLARE_ACCESSOR(std::string, Type); ///<The pickup type ("MachineGun", "Health", etc)
      DT_DECLARE_ACCESSOR(dtDAL::ResourceDescriptor, IconImage); ///<The texture used for the rendered PickUp geometry
      
      //TODO category (bow turret, rear dropper, etc)

   protected:
      virtual void CreateActor();
   private:
   };
}
#endif // PICKUPITEMBASE_h__
