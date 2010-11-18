#include <ode/ode.h>
#include <dtCore/transformable.h>
#include <dtCore/physical.h>
#include <dtCore/odecontroller.h>

//Coulomb friction coefficient. This must be in the range 0 to dInfinity.
//0 results in a frictionless contact, and dInfinity results in a contact that never slips. 
const double kMu = 100.0;

// Restitution parameter (0..1). 0 means the surfaces are not bouncy at all, 1 is maximum bouncyness.
const double kBounce = 0.01;

//The minimum incoming velocity necessary for bounce. Incoming velocities
//below this will effectively have a bounce parameter of 0.
const double kBounceVel = 0.01;

////////////////////////////////////////////////////////////////////////////////
//static
void nearCallback(void *data, dGeomID o1, dGeomID o2)
{

   if (o1 == 0 || o2 == 0)
   {
      //no geometry
      return;
   }

   dtCore::Transformable* c1 = static_cast<dtCore::Transformable*>(dGeomGetData(o1));
   dtCore::Transformable* c2 = static_cast<dtCore::Transformable*>(dGeomGetData(o2));
   dtCore::ODEController* odeControl = static_cast<dtCore::ODEController*>(data);

   const int N = 4;
   dContactGeom contactGeoms[N];
   
   int numContacts = dCollide (o1, o2, N, contactGeoms, sizeof(dContactGeom));

   if (numContacts > 0 && c1 != NULL && c2 != NULL) 
   {

      dContact contact;

      for (int i=0; i<numContacts; i++)
      {
         contact.surface.mode       = dContactBounce;
         contact.surface.mu         = (dReal)kMu;
         contact.surface.bounce     = (dReal)kBounce;
         contact.surface.bounce_vel = (dReal)kBounceVel;

         contact.geom = contactGeoms[i];

         // Make sure to call these both, because in the case of
         // Trigger, meaningful stuff happens even if the return
         // is false.

         if (c1->FilterContact(&contact, c2) && c2->FilterContact(&contact, c1))
         {
            dtCore::Physical* p1 = dynamic_cast<dtCore::Physical*>(c1);
            dtCore::Physical* p2 = dynamic_cast<dtCore::Physical*>(c2);

            if (p1 != NULL || p2 != NULL)
            {
               dJointID joint = dJointCreateContact(odeControl->GetWorldID(),
                                                      odeControl->GetContactJointGroupID(),
                                                      &contact);

               dJointAttach(joint,
                            p1->DynamicsEnabled() ? p1->GetBodyID() : 0,
                            p2->DynamicsEnabled() ? p2->GetBodyID() : 0);
            }
         }
      }
   }
}
