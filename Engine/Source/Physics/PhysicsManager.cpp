/*// -----------------------------------------------------------------
*  File:		PhysicsManager.cpp
*  Brief:		Implementation of physics manager
*  Creation:	19/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "PhysicsManager.hpp"

/// -----------------------------------------------------------------
/// Initialize physics manager
/// -----------------------------------------------------------------
void Engine::PhysicsManager::Initialize()
{
	RTTI::AddParentedType<Collider, EngineComp>();
	RTTI::AddParentedType<RigidBody, EngineComp>();

	mCollisionFns.insert(std::pair<uint8_t, CollisionFn>(Collider::COLLIDER_AABB | Collider::COLLIDER_AABB, AABBvsAABBEx));
}

/// -----------------------------------------------------------------
/// Update all the physics
/// -----------------------------------------------------------------
void Engine::PhysicsManager::Update()
{
	//Update rigid bodies first
	for (auto& rb : mRigidBodies)
		rb->ApplyForces();

	//Setup contact variables
	mCurrentContact.clear();
	Contact contact;

	//Update collisions per space
	for (auto& space : mDynamicObjs)
	{
		//Update dynamic objects
		for (auto dObjs = space.second.begin(); dObjs != space.second.end(); dObjs++)
		{
			//Update dynamic vs dynamic
			for (auto dObjs2 = dObjs; dObjs2 != space.second.end(); dObjs2++)
			{
				//If not active or enabled skip
				if (dObjs2 == dObjs)
					continue;
				if (!(*dObjs)->GetOwner()->IsEnabled() || !(*dObjs2)->GetOwner()->IsEnabled())
					continue;
				if (!(*dObjs)->IsActive() || !(*dObjs2)->IsActive())
					continue;

				//If collided, add contact to collision
				CollisionFn func = mCollisionFns.find((*dObjs)->GetColliderType() | (*dObjs2)->GetColliderType())->second;
				if (func(*dObjs, *dObjs2, &contact))
				{
					contact.mP1 = *dObjs;
					contact.mP2 = *dObjs2;
					mCurrentContact.push_back(contact);
				}
			}

			//Update dynamic vs static
			for (auto sObjs = mStaticObjs[space.first].begin(); sObjs != mStaticObjs[space.first].end(); sObjs++)
			{
				//If not active or enabled skip
				if (!(*dObjs)->GetOwner()->IsEnabled() || !(*sObjs)->GetOwner()->IsEnabled())
					continue;
				if (!(*dObjs)->IsActive() || !(*sObjs)->IsActive())
					continue;

				//If collided, add contact to collision
				CollisionFn func = mCollisionFns.find((*dObjs)->GetColliderType() | (*sObjs)->GetColliderType())->second;
				if (func(*dObjs, *sObjs, &contact))
				{
					contact.mP1 = *dObjs;
					contact.mP2 = *sObjs;
					mCurrentContact.push_back(contact);
				}
			}
		}
	}

	//Update contact collisions (resolve and call events)
	for (auto& contact : mCurrentContact)
	{
		if (contact.mP1->IsGhost() || contact.mP2->IsGhost())
			continue;

		ResolveContactPenetration(contact.mP1, contact.mP2, &contact);
		ResolveContactVelocity(contact.mP1, contact.mP2, &contact);
	}
}

/// -----------------------------------------------------------------
/// Render colliders
/// -----------------------------------------------------------------
void Engine::PhysicsManager::Render()
{
}

/// -----------------------------------------------------------------
/// Shutdown physics manager
/// -----------------------------------------------------------------
void Engine::PhysicsManager::Shutdown()
{
	mStaticObjs.clear();
	mDynamicObjs.clear();
}

/// -----------------------------------------------------------------
/// Add new collider
/// -----------------------------------------------------------------
void Engine::PhysicsManager::AddCollider(Collider* col)
{
	RigidBody* rb = col->GetOwner()->GetEngineComp<RigidBody>();
	Space* spa = col->GetOwner()->GetSpace();
	if (!rb || rb->IsStatic())
	{
		if(std::find(mStaticObjs[spa].begin(), mStaticObjs[spa].end(), col) == mStaticObjs[spa].end())
			mStaticObjs[spa].push_back(col);
	}
	else
	{
		if (std::find(mDynamicObjs[spa].begin(), mDynamicObjs[spa].end(), col) == mDynamicObjs[spa].end())
			mDynamicObjs[spa].push_back(col);
	}
}

/// -----------------------------------------------------------------
/// Remove collider
/// -----------------------------------------------------------------
void Engine::PhysicsManager::RemoveCollider(Collider* col)
{
	RigidBody* rb = col->GetOwner()->GetEngineComp<RigidBody>();
	Space* spa = col->GetOwner()->GetSpace();
	if (!rb || rb->IsStatic())
	{
		auto it = std::find(mStaticObjs[spa].begin(), mStaticObjs[spa].end(), col);
		if (it != mStaticObjs[spa].end())
		{
			mStaticObjs[spa].erase(it);
			if (mStaticObjs[spa].empty())
				mStaticObjs.erase(spa);
		}
	}
	else
	{
		auto it = std::find(mDynamicObjs[spa].begin(), mDynamicObjs[spa].end(), col);
		if (it != mDynamicObjs[spa].end())
		{
			mDynamicObjs[spa].erase(it);
			if (mDynamicObjs[spa].empty())
				mDynamicObjs.erase(spa);
		}
	}
}

/// -----------------------------------------------------------------
/// Updates collider list
/// -----------------------------------------------------------------
void Engine::PhysicsManager::UpdateCollider(Collider* col)
{
	RemoveCollider(col);
	AddCollider(col);
}

/// -----------------------------------------------------------------
/// Adds rigid body
/// -----------------------------------------------------------------
void Engine::PhysicsManager::AddRigidBody(RigidBody* rb)
{
	if (std::find(mRigidBodies.begin(), mRigidBodies.end(), rb) == mRigidBodies.end())
	{
		mRigidBodies.push_back(rb);
		auto col = rb->GetOwner()->GetEngineComp<Collider>();
		if (!col)
		{
			col = rb->GetOwner()->AddEngineComp<Collider>();
			col->Initialize();
		}
		else
			UpdateCollider(col);
	}
}

/// -----------------------------------------------------------------
/// Removes rigid body
/// -----------------------------------------------------------------
void Engine::PhysicsManager::RemoveRigidBody(RigidBody* rb)
{
	auto it = std::find(mRigidBodies.begin(), mRigidBodies.end(), rb);
	if (it != mRigidBodies.end())
	{
		mRigidBodies.erase(it);
		if (auto col = rb->GetOwner()->GetEngineComp<Collider>())
		{
			RemoveCollider(col);
			rb->SetStatic(true);
			AddCollider(col);
		}
	}
}

/// -----------------------------------------------------------------
/// Resolves contact penetration between two colliders
/// -----------------------------------------------------------------
void Engine::PhysicsManager::ResolveContactPenetration(Collider* p1, Collider* p2, Contact* contact)
{
	//Get rigid bodies
	RigidBody* rb1 = p1->GetOwner()->GetEngineComp<RigidBody>();
	RigidBody* rb2 = p2->GetOwner()->GetEngineComp<RigidBody>();
	float massInfluence1 = 1;
	float massInfluence2 = 1;

	//If they both have rigid bodies, compute the mass influence
	if (rb1 && rb2)
	{
		float totalMass = rb1->GetMass() + rb2->GetMass();
		massInfluence1 = rb1->GetMass() / totalMass;
		massInfluence2 = rb2->GetMass() / totalMass;
	}

	//compute the new positions
	Transform* t1 = p1->GetOwner()->GetTransform();
	Transform* t2 = p2->GetOwner()->GetTransform();
	glm::vec3 pos1 = t1->GetWorldPos() - contact->mNormal * contact->mPen * massInfluence1;
	glm::vec3 pos2 = t2->GetWorldPos() + contact->mNormal * contact->mPen * massInfluence2;

	//Update if they are dynamic
	if (rb1 && !rb1->IsStatic())
		t1->SetWorldPos(pos1);
	if (rb2 && !rb2->IsStatic())
		t2->SetWorldPos(pos2);
}

/// -----------------------------------------------------------------
/// Resolves contact velocity between two colliders
/// -----------------------------------------------------------------
void Engine::PhysicsManager::ResolveContactVelocity(Collider* p1, Collider* p2, Contact* contact)
{
	//Get rigid bodies
	RigidBody* rb1 = p1->GetOwner()->GetEngineComp<RigidBody>();
	RigidBody* rb2 = p2->GetOwner()->GetEngineComp<RigidBody>();
	float massInfluence1 = 1;
	float massInfluence2 = 1;
	glm::vec3 vel1 = glm::vec3(0.f);
	glm::vec3 vel2 = glm::vec3(0.f);

	//Get velocities and compute relative and separating velocities
	if (rb1)
		vel1 = rb1->GetVelocity();
	if (rb2)
		vel2 = rb2->GetVelocity();
	glm::vec3 relV = vel2 - vel1;
	float sepV = glm::dot(relV, contact->mNormal);
	
	//If they are too far from each other don't do anything
	if (sepV > 0)
		return;

	//If they both have rigid bodies, compute the mass influence
	if (rb1 && rb2)
	{
		float totalMass = rb1->GetMass() + rb2->GetMass();
		massInfluence1 = rb1->GetMass() / totalMass;
		massInfluence2 = rb2->GetMass() / totalMass;
	}

	// Compute the squares of the mass influences
	float massInfSq1 = massInfluence1 * massInfluence1;
	float massInfSq2 = massInfluence2 * massInfluence2;

	// Compute the new velocities of the objects and set to them if not static
	glm::vec3 newvel1 = vel1 - contact->mNormal * -sepV * massInfSq1;
	glm::vec3 newvel2 = vel2 + contact->mNormal * -sepV * massInfSq2;

	if (rb1 && !rb1->IsStatic())
		rb1->SetVelocity(newvel1);
	if (rb2 && !rb2->IsStatic())
		rb2->SetVelocity(newvel2);
}
