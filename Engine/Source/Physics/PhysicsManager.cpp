/*// -----------------------------------------------------------------
*  File:		PhysicsManager.cpp
*  Brief:		Implementation of physics manager
*  Creation:	19/03/2023
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "PhysicsManager.hpp"
#include <Graphics/GraphicsManager.hpp>
#include <Graphics/Camera/Camera.hpp>

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
	size_t rigidBodyCount = mRigidBodies.size();
	for (size_t rb = 0; rb < rigidBodyCount; rb++)
		mRigidBodies[rb]->ApplyForces();

	//Setup contact variables
	mPreviousContact = mCurrentContact;
	mCurrentContact.clear();
	Contact contact;

	//Setup collider variables
	Collider* sObj = nullptr;
	Collider* dObj1 = nullptr;
	Collider* dObj2 = nullptr;

	//Update collisions per space
	for (auto& space : mDynamicObjs)
	{
		//Get dynamic and static objects of space
		std::vector<Collider*>& dynamicObjs = space.second;
		std::vector<Collider*>& staticObjs = mStaticObjs[space.first];
		size_t staticObjCount = staticObjs.size();
		size_t dynamicObjCount = dynamicObjs.size();

		//Update dynamic objects
		for (size_t dIdx1 = 0; dIdx1 < dynamicObjCount; dIdx1++)
		{
			//Update dynamic vs dynamic
			dObj1 = dynamicObjs[dIdx1];
			for (size_t dIdx2 = dIdx1 + 1; dIdx2 < dynamicObjCount; dIdx2++)
			{
				//If not active or enabled skip
				dObj2 = dynamicObjs[dIdx2];
				if (!dObj1->GetOwner()->IsEnabled() || !dObj2->GetOwner()->IsEnabled())
					continue;
				if (!dObj1->IsActive() || !dObj2->IsActive())
					continue;

				//If collided, add contact to collision
				uint8_t collFuncID = static_cast<uint8_t>(dObj1->GetColliderType() | dObj2->GetColliderType());
				CollisionFn func = mCollisionFns.find(collFuncID)->second;
				if (func(dObj1, dObj2, &contact))
				{
					contact.mP1 = dObj1;
					contact.mP2 = dObj2;
					mCurrentContact.push_back(contact);
				}
			}

			//Update dynamic vs static
			for (size_t sIdx = 0; sIdx < staticObjCount; sIdx++)
			{
				sObj = staticObjs[sIdx];
				//If not active or enabled skip
				if (!dObj1->GetOwner()->IsEnabled() || !sObj->GetOwner()->IsEnabled())
					continue;
				if (!dObj1->IsActive() || !sObj->IsActive())
					continue;

				uint8_t collFuncID = static_cast<uint8_t>(dObj1->GetColliderType() | sObj->GetColliderType());
				CollisionFn func = mCollisionFns.find(collFuncID)->second;
				if (func(dObj1, sObj, &contact))
				{
					contact.mP1 = dObj1;
					contact.mP2 = sObj;
					mCurrentContact.push_back(contact);
				}
			}
		}
	}

	//Update contact collisions (resolve and call events)
	size_t currentContactCount = mCurrentContact.size();
	for (size_t idx = 0; idx < currentContactCount; idx++)
	{
		//Check if it's still in contact or if new collision
		auto isInPrev = std::find(mPreviousContact.begin(), mPreviousContact.end(), mCurrentContact[idx]);
		
		//Continuous collision
		//----When collision events, add collision persisted here----//
		if (isInPrev != mPreviousContact.end())
			mPreviousContact.erase(isInPrev);
		//----When collision events, add collision started here----//

		if (mCurrentContact[idx].mP1->IsGhost() || mCurrentContact[idx].mP2->IsGhost())
			continue;

		ResolveContactPenetration(mCurrentContact[idx].mP1, mCurrentContact[idx].mP2, &mCurrentContact[idx]);
		ResolveContactVelocity(mCurrentContact[idx].mP1, mCurrentContact[idx].mP2, &mCurrentContact[idx]);
	}

	//Remove contact collision from the ones not colliding anymore
	//----When collision events, add collision ended here----//
	size_t previousContactCount = mPreviousContact.size();
	for (size_t idx = 0; idx < previousContactCount; idx++)
	{
		mPreviousContact[idx].mP1->GetOwner()->GetEngineComp<RigidBody>()->RemoveForce("CollisionContact");
		mPreviousContact[idx].mP2->GetOwner()->GetEngineComp<RigidBody>()->RemoveForce("CollisionContact");
	}
}

/// -----------------------------------------------------------------
/// Render colliders
/// -----------------------------------------------------------------
void Engine::PhysicsManager::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Shader* shader = gGfxMgr->GetShader("Default");
	shader->Activate();
	Collider* sObj = nullptr;
	Collider* dObj = nullptr;

	for (const auto& space : mStaticObjs)
	{
		//Set shader values
		Camera* cam = space.first->GetObjectByName("Main Camera")->GetEngineComp<Camera>();
		shader->UniformMat4(cam->GetProj(), "proj");
		shader->UniformMat4(cam->GetView(), "view");

		//Get dynamic and static objects of space
		const std::vector<Collider*>& dynamicObjs = space.second;
		const std::vector<Collider*>& staticObjs = mStaticObjs[space.first];
		size_t staticObjCount = staticObjs.size();
		size_t dynamicObjCount = dynamicObjs.size();

		//iterate on static objects
		for (size_t sIdx = 0; sIdx < staticObjCount; sIdx++)
		{
			sObj = staticObjs[sIdx];
			if (sObj->IsColliderDrawn())
				sObj->Render();
		}

		//iterate on dynamic objects
		for (size_t dIdx = 0; dIdx < dynamicObjCount; dIdx++)
		{
			dObj = dynamicObjs[dIdx];
			if (dObj->IsColliderDrawn())
				dObj->Render();
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/// -----------------------------------------------------------------
/// Shutdown physics manager
/// -----------------------------------------------------------------
void Engine::PhysicsManager::Shutdown()
{
	mStaticObjs.clear();
	mDynamicObjs.clear();
	mCurrentContact.clear();
	mPreviousContact.clear();
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
	glm::vec3 pos1 = t1->GetWorldPos();
	glm::vec3 pos2 = t2->GetWorldPos();

	//Only update when penetration value is significant
	if (contact->mPen >= 0.02f)
	{
		pos1 -= contact->mNormal * contact->mPen * massInfluence1;
		pos2 += contact->mNormal * contact->mPen * massInfluence2;
	}

	glm::vec3 invNorm = glm::abs(glm::abs(contact->mNormal) - glm::vec3(1.f));
	//Update if they are dynamic
	if (rb1 && !rb1->IsStatic())
	{
		t1->SetWorldPos(pos1);
		rb1->RemoveForce("CollisionContact");
		rb1->AddForce("CollisionContact", rb1->GetTotalForces() * contact->mNormal);
	}
	if (rb2 && !rb2->IsStatic())
	{
		t2->SetWorldPos(pos2);
		rb2->RemoveForce("CollisionContact");
		rb2->AddForce("CollisionContact", rb2->GetTotalForces() * contact->mNormal);
	}
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
	float massInfSq1 = massInfluence1;
	float massInfSq2 = massInfluence2;

	// Compute the new velocities of the objects and set to them if not static
	if (contact->mPen >= 0.02f)
	{
		glm::vec3 newvel1 = -contact->mNormal * -sepV * massInfSq2;
		glm::vec3 newvel2 = contact->mNormal * -sepV * massInfSq1;

		if (rb1 && !rb1->IsStatic())
			rb1->SetVelocity(newvel1);
		if (rb2 && !rb2->IsStatic())
			rb2->SetVelocity(newvel2);
	}
}
