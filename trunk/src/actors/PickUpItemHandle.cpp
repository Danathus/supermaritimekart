#include <actors/PickUpItemHandle.h>
#include <actors/SMKBoatActor.h>

#include <dtCore/transform.h>
#include <dtDAL/actortype.h>
#include <dtDAL/propertymacros.h> //for dtDAL::PropertyRegHelper
#include <dtDAL/project.h>//for loading resources
#include <dtGame/basemessages.h>
#include <dtGame/gamemanager.h>
#include <dtGame/messagetype.h>

#ifdef BUILD_WITH_DTOCEAN
# include <dtOcean/actorregistry.h>
# include <dtOcean/oceanactor.h>
#endif

#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/Geode>

#include <cassert>
using namespace SMK;

//borrowed from SimCore to hide a node
class HideNodeCallback : public osg::NodeCallback
{
public:

   HideNodeCallback()
   {}

   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
   {
      // We're hiding the node.  The point is to NOT traverse.
      // traverse(node, nv);
   }
};
static dtCore::RefPtr<HideNodeCallback> HIDE_NODE_CALLBACK(new HideNodeCallback);

////////////////////////////////////////////////////////////////////////////////
static const float PICKUP_RESPAWN_TIME = 5.0f;

////////////////////////////////////////////////////////////////////////////////
DT_IMPLEMENT_ACCESSOR(PickUpItemHandle, std::string, Type);
DT_IMPLEMENT_ACCESSOR_GETTER(PickUpItemHandle, dtDAL::ResourceDescriptor, IconImage);
DT_IMPLEMENT_ACCESSOR(PickUpItemHandle, dtUtil::EnumerationPointer<PickupCategoryEnum>, PickupCategory);

PickUpItemHandle::PickUpItemHandle(dtGame::GameActorProxy& proxy)
: dtGame::GameActor(proxy)
, mIsAvailable(true)
, mPickupCategory(&PickupCategoryEnum::PICKUP_NONE)
, mReactivateCounter(0.0f)
{
   SetName("pick up item");   
   osg::Node* root = GetOSGNode();
   if (root)
   {
      if (root->asGroup())
      {
         osg::ref_ptr<osg::Node> geometry = CreateGeometry();
         root->asGroup()->addChild(geometry);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
PickUpItemHandle::~PickUpItemHandle()
{

}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::SetIconImage(dtUtil::TypeTraits<dtDAL::ResourceDescriptor>::param_type value)
{
   mIconImage = value;

   if (mIconImage.IsEmpty())
   {
      GetOSGNode()->getOrCreateStateSet()->removeTextureMode(0, osg::StateAttribute::ON);
      return;
   }

   const std::string textureFile = dtDAL::Project::GetInstance().GetResourcePath(mIconImage);

   if (!textureFile.empty())
   {
      osg::Image* image = osgDB::readImageFile(textureFile);
      if (image)
      {
         osg::Texture2D* texture = new osg::Texture2D(image);
         GetOSGNode()->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::SetActive(bool enable)
{
   if (mIsAvailable == enable)
   {
      return;
   }

   mIsAvailable = enable;
   if (enable)
   {
      GetOSGNode()->setCullCallback(NULL);
   }
   else
   {
      GetOSGNode()->setCullCallback(HIDE_NODE_CALLBACK.get());
   }
}

////////////////////////////////////////////////////////////////////////////////
bool PickUpItemHandle::GetActive() const
{
   return mIsAvailable;
}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::TickLocal(const dtGame::Message& msg)
{
   dtGame::GameActor::TickLocal(msg);

   SpinPickUpItem();

   // If we're not active, activate us after a certain amount of time has passed
   if (!GetActive())
   {
      const dtGame::TickMessage& tickMessage = static_cast<const dtGame::TickMessage&>(msg);
      float dt = tickMessage.GetDeltaSimTime();

      mReactivateCounter += dt;
      if (mReactivateCounter > PICKUP_RESPAWN_TIME)
      {
         mReactivateCounter = 0.0f;
         SetActive(true);
         SetCollisionDetection(true);

         //tell the world this pickup is now inactive
         std::vector<dtUtil::RefString> propsToSend;
         propsToSend.push_back("IsActive"); //DeltaDrawable
         propsToSend.push_back(dtDAL::TransformableActorProxy::PROPERTY_ENABLE_COLLISION);
         GetGameActorProxy().NotifyPartialActorUpdate(propsToSend);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::TickRemote(const dtGame::Message& msg)
{
   dtGame::GameActor::TickRemote(msg);

   SpinPickUpItem();
}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::OnEnteredWorld()
{
   SetupFloaterComponent();
}

////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> PickUpItemHandle::CreateGeometry()
{
   const osg::Vec3 center(0.f, 0.f, 0.5f);
   const  osg::Vec4 color(1.f, 1.f, 0.f, 1.f);

   osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
   hints->setCreateTextureCoords(true);

   osg::ref_ptr<osg::Geode> geode = new osg::Geode();

   osg::ref_ptr<osg::Box> box = new osg::Box(center, 1.f);

   osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box, hints);



   shape->setColor(color);
   geode->addDrawable(shape);

   return geode;
}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::SetupFloaterComponent()
{
#ifdef BUILD_WITH_DTOCEAN
   //find any OceanActors;
   dtOcean::OceanActorProxy* oceanActorProxy(NULL);
   GetGameActorProxy().GetGameManager()->FindActorByType(*OceanActorRegistry::OCEAN_ACTOR_TYPE, oceanActorProxy);
   dtOcean::OceanActor* oceanActor(NULL);
   oceanActorProxy->GetActor(oceanActor);
   assert(oceanActor != NULL);

   // Create a new floater component with the ocean
   mpFloaterComponent = new SimpleFloaterActorComponent(*oceanActor);
   AddComponent(*mpFloaterComponent);
#endif
}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemHandle::SpinPickUpItem()
{
   dtCore::Transform currentTransform;
   GetTransform(currentTransform);
   currentTransform.SetRotation(currentTransform.GetRotation() + osg::Vec3(1.0, 0.0, 0.0));
   SetTransform(currentTransform);
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
PickUpItemBaseProxy::PickUpItemBaseProxy()
: dtGame::GameActorProxy()
{
   SetClassName("PickUpItemBase");
}

////////////////////////////////////////////////////////////////////////////////
PickUpItemBaseProxy::~PickUpItemBaseProxy()
{

}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemBaseProxy::OnEnteredWorld()
{
   dtGame::GameActorProxy::OnEnteredWorld();

   if (!IsRemote())
   {
      RegisterForMessages(dtGame::MessageType::TICK_LOCAL, dtGame::GameActorProxy::TICK_LOCAL_INVOKABLE);
   }
   else
   {
      RegisterForMessages(dtGame::MessageType::TICK_REMOTE, dtGame::GameActorProxy::TICK_REMOTE_INVOKABLE);
   }
}

///////////////////////////////////////////////////////////////////////////////
void PickUpItemBaseProxy::OnRemovedFromWorld()
{
   if (!IsRemote())
   {
      UnregisterForMessages(dtGame::MessageType::TICK_LOCAL, dtGame::GameActorProxy::TICK_LOCAL_INVOKABLE);
   }
   else
   {
      UnregisterForMessages(dtGame::MessageType::TICK_REMOTE, dtGame::GameActorProxy::TICK_REMOTE_INVOKABLE);
   }

   dtGame::GameActorProxy::OnRemovedFromWorld();
}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemBaseProxy::CreateActor()
{
   PickUpItemHandle* pickup = new PickUpItemHandle(*this);
   SetActor(*pickup);
}

////////////////////////////////////////////////////////////////////////////////
void PickUpItemBaseProxy::BuildPropertyMap()
{
   dtGame::GameActorProxy::BuildPropertyMap();

   //some inherited API to ensure we default to a 1x1x1 collision box
   this->SetCollisionBoxDims(osg::Vec3(1.f, 1.f, 1.f));
   this->SetCollisionType(dtCore::Transformable::CollisionGeomType::CUBE);

   //Properties reside in the Proxy, but the get/set functors reside on the actor itself
   typedef dtDAL::PropertyRegHelper<dtDAL::PropertyContainer&, PickUpItemHandle> RegHelperType;
   PickUpItemHandle* actor = static_cast<PickUpItemHandle*>(GetActor());
   RegHelperType regHelper(*this, actor, "PickUp Properties");

   //add in additional properties
   DT_REGISTER_PROPERTY(Type, "Defines the type of PickUp; matches what's registered in the PickUp Factor", RegHelperType, regHelper);
   DT_REGISTER_RESOURCE_PROPERTY(dtDAL::DataType::TEXTURE, IconImage, "Icon Image", "Used on the rendered geometry", RegHelperType, regHelper);
   DT_REGISTER_PROPERTY(PickupCategory, "Which category of pickup item is this?", RegHelperType, regHelper);
}

//////////////////////////////////////////////////////////////////////////
