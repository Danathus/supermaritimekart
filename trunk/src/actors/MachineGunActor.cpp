#include <actors/MachineGunActor.h>

#include <dtAudio/audiomanager.h>
#include <dtCore/shadermanager.h>
#include <dtCore/shaderparamoscillator.h>
#include <dtCore/timer.h>
#include <dtCore/transform.h>
#include <dtUtil/log.h>

#include <osg/Billboard>
#include <osg/BlendEquation>
#include <osg/BlendFunc>
#include <osg/Texture2D>

#include <osgDB/ReadFile>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
const std::string MachineGunActor::MACHINE_GUN_ACTOR_TYPE = "MachineGun";
const float BULLET_TRAIL_HEIGHT = 0.5f;

//////////////////////////////////////////////////////////////////////////
MachineGunActor::MachineGunActor(const std::string& filename /*= ""*/)
: WeaponActor(filename)
{
   mFiringRate = 10.0f;

   // Load any sounds we have
   mpFireSound = LoadSound("/sounds/exp57.wav");
   mpFireSound->SetGain(0.5f);

   CreateBulletTrail();
}

//////////////////////////////////////////////////////////////////////////
MachineGunActor::~MachineGunActor()
{
   if (mpQuad.valid())
   {
      mpBeam->removeDrawable(mpQuad.get());
      mpQuad = NULL;
   }

   if (mpBeam.valid())
   {
      GetMatrixNode()->removeChild(mpBeam);
      mpBeam = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
void MachineGunActor::FireWeapon()
{
   WeaponActor::FireWeapon();

   dtCore::Transform currentTransform;
   GetTransform(currentTransform, dtCore::Transformable::REL_CS);
   osg::Vec3 right, up, forward;
   currentTransform.GetOrientation(right, up, forward);
   ShowBulletTrail(forward * 1000.0f);
}

///////////////////////////////////////////////////////////////////////////////
void MachineGunActor::CreateBulletTrail()
{
   // Orient beam so that it exposes as much of its face as possible
   mpBeam = new osg::Billboard;
   mpBeam->setMode(osg::Billboard::AXIAL_ROT);
   mpBeam->setAxis(osg::X_AXIS);
   mpBeam->setNormal(osg::Z_AXIS);
   mpBeam->setName("Beam");
   GetMatrixNode()->addChild(mpBeam);

   mpBeamStateSet = new osg::StateSet;

   osg::Image* beamImage       = osgDB::readImageFile("textures/bulletTrail.png");
   osg::Texture2D* beamTexture = new osg::Texture2D(beamImage);

   osg::BlendEquation* blendEquation = new osg::BlendEquation(osg::BlendEquation::FUNC_ADD);

   osg::BlendFunc* blendFunc = new osg::BlendFunc;
   blendFunc->setFunction(osg::BlendFunc::ONE, osg::BlendFunc::ONE);

   // Set the blending parameters
   mpBeamStateSet->setDataVariance(osg::Object::DYNAMIC);
   mpBeamStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
   mpBeamStateSet->setBinNumber(100);
   mpBeamStateSet->setTextureAttributeAndModes(0, beamTexture, osg::StateAttribute::ON);
   mpBeamStateSet->setAttributeAndModes(blendEquation, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
   mpBeamStateSet->setAttributeAndModes(blendFunc, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
   mpBeamStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

   // Retrieve the shader from the shader manager and assign it to this stateset
   dtCore::ShaderManager&       shaderManager    = dtCore::ShaderManager::GetInstance();
   const dtCore::ShaderProgram* prototypeProgram = shaderManager.FindShaderPrototype("BulletTrailShader");
   dtCore::ShaderProgram*       program          = shaderManager.AssignShaderFromPrototype(*prototypeProgram, *mpBeam.get());
   assert(program);

   mpTimeParam = dynamic_cast<dtCore::ShaderParamOscillator*>(program->FindParameter("time"));
   assert(mpTimeParam.valid());
   mpTimeParam->SetUseRealTime(false);}

///////////////////////////////////////////////////////////////////////////////
void MachineGunActor::ShowBulletTrail(osg::Vec3 target)
{
   osg::Vec3 heightVec(0.0f, 0.0f, BULLET_TRAIL_HEIGHT);
   osg::Vec3 corner   = osg::Vec3() - (heightVec * 0.5f);
   osg::Vec3 widthVec = target;
   osg::Vec3 rightVec = heightVec ^ widthVec;

   heightVec = widthVec ^ rightVec;
   heightVec.normalize();
   heightVec *= BULLET_TRAIL_HEIGHT;

   if (mpQuad.valid())
   {
      mpBeam->removeDrawable(mpQuad.get());
      mpQuad = NULL;
   }

   mpQuad = osg::createTexturedQuadGeometry(-heightVec * 0.5f, widthVec, heightVec, 0.0f, 0.0f, 1.0f, 1.0f);
   mpQuad->setStateSet(mpBeamStateSet.get());

   widthVec.normalize();
   widthVec *= 0.025f;
   mpQuad->setStateSet(mpBeamStateSet.get());
   mpBeam->addDrawable(mpQuad.get());
   mpBeam->setPosition(0, widthVec);

   osg::Vec3 axis = widthVec;
   osg::Vec3 normal = rightVec;

   axis.normalize();
   normal.normalize();

   mpBeam->setAxis(axis);
   mpBeam->setNormal(normal);

   // Tell the oscillator to start updating the time uniform
   mpTimeParam->TriggerOscillationStart();
}

////////////////////////////////////////////////////////////////////////////////
