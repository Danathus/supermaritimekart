#ifndef DOFDrawable_h__
#define DOFDrawable_h__

////////////////////////////////////////////////////////////////////////////////
#include <SMKActorExport.h>
#include <dtCore/deltadrawable.h>

#include <osgSim/DOFTransform>

////////////////////////////////////////////////////////////////////////////////
class DOFDrawable : public dtCore::DeltaDrawable
{
public:
   DOFDrawable(const std::string& name = "DOF Drawable");

   virtual osg::Node* GetOSGNode();
   virtual const osg::Node* GetOSGNode() const;

   virtual bool AddChild(DeltaDrawable* child);
   virtual void RemoveChild(DeltaDrawable* child);

   // Rotation functions
   void SetMinRotation(const osg::Vec3& hpr) { mpDOFNode->setMinHPR(hpr);     }
   const osg::Vec3& GetMinRotation() const   { return mpDOFNode->getMinHPR(); }

   void SetMaxRotation(const osg::Vec3& hpr) { mpDOFNode->setMaxHPR(hpr);     }
   const osg::Vec3& GetMaxRotation() const   { return mpDOFNode->getMaxHPR(); }

   void SetIncrementRotation(const osg::Vec3& hpr) { mpDOFNode->setIncrementHPR(hpr);     }
   const osg::Vec3& GetIncrementRotation() const   { return mpDOFNode->getIncrementHPR(); }

   void SetCurrentRotation(const osg::Vec3& hpr) { mpDOFNode->setCurrentHPR(hpr);     }
   const osg::Vec3& GetCurrentRotation() const   { return mpDOFNode->getCurrentHPR(); }

   void UpdateCurrentRotation(const osg::Vec3& hpr) { mpDOFNode->updateCurrentHPR(hpr); }

   // Translation functions
   void SetMinTranslation(const osg::Vec3& translate) { mpDOFNode->setMinTranslate(translate); }
   const osg::Vec3& GetMinTranslation() const         { return mpDOFNode->getMinTranslate();   }

   void SetMaxTranslation(const osg::Vec3& translate) { mpDOFNode->setMaxTranslate(translate); }
   const osg::Vec3& GetMaxTranslation() const         { return mpDOFNode->getMaxTranslate();   }

   void SetIncrementTranslation(const osg::Vec3& translate) { mpDOFNode->setIncrementTranslate(translate); }
   const osg::Vec3& GetIncrementTranslation() const         { return mpDOFNode->getIncrementTranslate();   }

   void SetCurrentTranslation(const osg::Vec3& translate) { mpDOFNode->setCurrentTranslate(translate); }
   inline const osg::Vec3& GetCurrentTranslation() const  { return mpDOFNode->getCurrentTranslate();   }

   void UpdateCurrentTranslation(const osg::Vec3& translate) { mpDOFNode->updateCurrentTranslate(translate); }

   // Scale functions
   void SetMinScale(const osg::Vec3& scale) { mpDOFNode->setMinScale(scale);   }
   const osg::Vec3& GetMinScale() const     { return mpDOFNode->getMinScale(); }

   void SetMaxScale(const osg::Vec3& scale) { mpDOFNode->setMaxScale(scale);   }
   const osg::Vec3& GetMaxScale() const     { return mpDOFNode->getMaxScale(); }

   void SetIncrementScale(const osg::Vec3& scale) { mpDOFNode->setIncrementScale(scale);   }
   const osg::Vec3& GetIncrementScale() const     { return mpDOFNode->getIncrementScale(); }

   void SetCurrentScale(const osg::Vec3& scale)    { mpDOFNode->setCurrentScale(scale);   }
   inline const osg::Vec3& GetCurrentScale() const { return mpDOFNode->getCurrentScale(); }

   void UpdateCurrentScale(const osg::Vec3& scale) { mpDOFNode->updateCurrentScale(scale); }

protected:
   virtual ~DOFDrawable();

private:
   dtCore::RefPtr<osgSim::DOFTransform> mpDOFNode;
};

////////////////////////////////////////////////////////////////////////////////
#endif // DOFDrawable_h__
