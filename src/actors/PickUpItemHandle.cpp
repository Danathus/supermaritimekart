#include <actors/PickUpItemHandle.h>
#include <actors/SMKBoatActor.h>

#include <dtDAL/actortype.h>
#include <dtDAL/propertymacros.h> //for dtDAL::PropertyRegHelper
#include <dtDAL/project.h>//for loading resources

#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/Geode>
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
PickUpItemBaseProxy::PickUpItemBaseProxy():
dtGame::GameActorProxy()
{
   SetClassName("PickUpItemBase");
}

////////////////////////////////////////////////////////////////////////////////
PickUpItemBaseProxy::~PickUpItemBaseProxy()
{

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
////////////////////////////////////////////////////////////////////////////////

DT_IMPLEMENT_ACCESSOR(PickUpItemHandle, std::string, Type);
DT_IMPLEMENT_ACCESSOR_GETTER(PickUpItemHandle, dtDAL::ResourceDescriptor, IconImage);
DT_IMPLEMENT_ACCESSOR(PickUpItemHandle, dtUtil::EnumerationPointer<PickupCategoryEnum>, PickupCategory);

PickUpItemHandle::PickUpItemHandle(dtGame::GameActorProxy& proxy)
: dtGame::GameActor(proxy)
, mIsAvailable(true)
, mPickupCategory(&PickupCategoryEnum::PICKUP_NONE)
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
//////////////////////////////////////////////////////////////////////////
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
osg::ref_ptr<osg::Node> PickUpItemHandle::CreateGeometry()
{
   const osg::Vec3 center(0.f, 0.f, 0.f);
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

////////////////////////////////////////////////////////////////////////////////
void SMK::PickUpItemHandle::SetActive(bool enable)
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
bool SMK::PickUpItemHandle::GetActive() const
{
   return mIsAvailable;
}
