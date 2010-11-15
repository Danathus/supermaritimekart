/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2008, MOVES Institute
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * @author Jeffrey Houde
 */

#ifndef _FLOATING_ACTOR_H_
#define _FLOATING_ACTOR_H_


class Buoy;
class IGetHeight;
class FloatingActorProxy;

////////////////////////////////////////////////////////////////////////////////

#include <dtActors/gamemeshactor.h>
#include <BoatActors/ActorLibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

class BOAT_ACTORS_LIBRARY_EXPORT FloatingActor : public dtActors::GameMeshActor
{
public:

   /**
   * Constructor
   */
   FloatingActor(FloatingActorProxy& proxy);

   /**
   * Standard update.
   */
   virtual void TickLocal(const dtGame::Message& msg);

   /**
   * Message handler.
   */
   virtual void OnMessage(MessageData* data);

   /**
   * Physics update
   */
   virtual void OnPhysicsStep();

   /**
   * Sets the water height function callback.
   *
   * @param[in]  GetHeight  Water height class.
   */
   virtual void SetGetHeight(IGetHeight* getHeight);

   /** 
   * Set an alternate collision mesh filename to use instead of the
   * inherited functionality.  Note, this doesn't actually set the collision 
   * mesh, just stores it for something external to query and load.
   *@param meshFile the mesh file resource to use for collision detection
   */
   void SetAlternateCollisionMesh(const std::string& meshFile);

   std::string GetAlternateCollisionMesh() const;

   /**
   * Get the Buoy of the actor.
   */
   Buoy* GetBuoy() const;

   /** 
   * Called by our Proxy when we're being removed from the world.  This is a
   * good place to unreference stuff and un-initialize things.
   */
   virtual void OnRemovedFromWorld();

protected:

   /**
   * Destructor
   */
   virtual ~FloatingActor();

   /**
   * Initialize the actor.
   */
   virtual void Initialize();

   /**
   * Setup the Buoyancy of the floating actor.
   */
   virtual void SetupBuoyancy(float totalMass, float lx, float ly, float lz);

   /**
   * Callback called when the actor enters the world.
   */
   virtual void OnEnteredWorld();

   bool                          mIsInitialized;

   dtCore::RefPtr<Buoy>          mBuoyPhysics;
   dtCore::RefPtr<IGetHeight>    mGetHeight;
   std::string                   mAltCollisionMesh; ///<experimental collision resource
};

class BOAT_ACTORS_LIBRARY_EXPORT FloatingActorProxy: public dtActors::GameMeshActorProxy
{
public:

   // Constructor
   FloatingActorProxy();

   // Adds the properties associated with this actor
   virtual void BuildPropertyMap();

   virtual void BuildInvokables();

   // Creates the actor
   virtual void CreateActor();

   // Insert into game
   virtual void RegisterWithGameManager(dtGame::GameManager* gm, bool useDefaultModel = true);

   virtual void OnEnteredWorld();

   /**
   * Called when the GM deletes the actor in a NORMAL way, such as DeleteActor().
   * This may not get called from every possible path that an actor can be removed
   * or deleted. For instance, calling DeleteAllActors(true) on the GM would bypass
   * the 'normal' actor delete process. Note, this is sent at the end of the tick and
   * at the end of a 'normal' map change. If you want to find out immediately that you
   * are being deleted, register for the INFO_ACTOR_DELETED with RegisterForMessagesAboutSelf().
   */
   virtual void OnRemovedFromWorld();

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

protected:

   // Destructor
   virtual ~FloatingActorProxy();
};

////////////////////////////////////////////////////////////////////////////////

#endif
