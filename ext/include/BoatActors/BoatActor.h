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

#ifndef _BOAT_ACTOR_H_
#define _BOAT_ACTOR_H_

////////////////////////////////////////////////////////////////////////////////

#include <BoatActors/FloatingActor.h>
#include <BoatActors/ActorLibraryExport.h>

////////////////////////////////////////////////////////////////////////////////

class Outboard;
class BoatEffects;
class BoatActorProxy;

class BOAT_ACTORS_LIBRARY_EXPORT BoatActor : public FloatingActor
{
public:  

   /**
   * Constructor
   */
   BoatActor(BoatActorProxy& proxy);

   virtual void TickLocal(const dtGame::Message& msg);

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
   * Get the outboard.
   */
   Outboard* GetOutBoard();
   const Outboard* GetOutBoard() const;

   /**
   * Get the effects.
   */
   BoatEffects* GetEffects();
   const BoatEffects* GetEffects() const;

   /** 
   * Inherited from FloatingActor.  Called when this actor is about to be nuked.
   */
   virtual void OnRemovedFromWorld();

protected:

   /**
   * Destructor
   */
   virtual ~BoatActor();

   /**
   * Initialize the actor.
   */
   virtual void Initialize();

private:

   dtCore::RefPtr<Outboard>      mOutboard;
   dtCore::RefPtr<BoatEffects>   mEffects;
};

class BOAT_ACTORS_LIBRARY_EXPORT BoatActorProxy: public FloatingActorProxy
{
public:

   /**
   * Constructor
   */
   BoatActorProxy();

   /**
   * Adds the properties associated with this actor
   */
   virtual void BuildPropertyMap();

   /**
   * Build invokables
   */
   virtual void BuildInvokables();

   /**
   * Creates the actor
   */
   virtual void CreateActor();

   /**
   * Get the outboard.
   */
   Outboard* GetOutBoard();
   const Outboard* GetOutBoard() const;

   /**
   * Get the effects.
   */
   BoatEffects* GetEffects();
   const BoatEffects* GetEffects() const;

   /** 
   * Called when this ActorProxy is being removed from the world.  Time to clean
   * up our junk.
   */
   virtual void OnRemovedFromWorld();

   virtual void GetPartialUpdateProperties(std::vector<dtUtil::RefString>& propNamesToFill);

   void RemoveProperty(const std::string& nameToRemove);

protected:

   // Destructor
   virtual ~BoatActorProxy();
};

////////////////////////////////////////////////////////////////////////////////

#endif
