#ifndef PICKUPITEMBASE_h__
#define PICKUPITEMBASE_h__

#include <actors/SMKActorExport.h>
#include <dtGame/gameactorproxy.h>
#include <dtGame/gameactor.h>
#include <dtUtil/getsetmacros.h>
#include <dtDAL/resourcedescriptor.h>
#include <actors/PickupCategoryEnum.h>
namespace SMK
{
   /** The PickUpItemHandle are used to place and render
     * where pickup-able items should be in the world.  When the player collides with these,
     * the Type and Category
     * can be queried.  If PickUp is useful, then the concrete PickUp can be 
     * instantiated with the PickUpFactory.
     */
   class SMK_ACTOR_EXPORT PickUpItemHandle : public dtGame::GameActor
   {
   public:
      PickUpItemHandle(dtGame::GameActorProxy& proxy);
      //Accessors
      DT_DECLARE_ACCESSOR(std::string, Type); ///<The pickup type ("MachineGun", "Health", etc)
      DT_DECLARE_ACCESSOR(dtDAL::ResourceDescriptor, IconImage); ///<The texture used for the rendered PickUp geometry
      DT_DECLARE_ACCESSOR(dtUtil::EnumerationPointer<PickupCategoryEnum>, PickupCategory);  ///<what category this pickup is

      ///overwriting from DeltaDrawable which doesn't seem to work(?)
      virtual void SetActive(bool enable);

      /**
       * @return true if active, false if not active
       */
      virtual bool GetActive() const;


   protected:
      virtual ~PickUpItemHandle();
   private:
      osg::ref_ptr<osg::Node> CreateGeometry();
      bool mIsAvailable; ///<Is this PickUpItemHandle available for picking up?
   };

   //////////////////////////////////////////////////////////////////////////
   class SMK_ACTOR_EXPORT PickUpItemBaseProxy : public dtGame::GameActorProxy
   {
   public:
      PickUpItemBaseProxy();
      virtual ~PickUpItemBaseProxy();
      virtual void BuildPropertyMap();
      

   protected:
      virtual void CreateActor();
   private:
   };
}
#endif // PICKUPITEMBASE_h__
