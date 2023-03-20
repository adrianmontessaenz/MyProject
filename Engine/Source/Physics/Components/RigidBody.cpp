/*// -----------------------------------------------------------------
*  File:		RigidBody.cpp
*  Brief:		Implementation of RigidBody
*  Creation:	19/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "RigidBody.hpp"
#include <Physics/PhysicsManager.hpp>
#include <Core/Time/TimeSystem.hpp>

/// -----------------------------------------------------------------
/// Initialize rigidbody
/// -----------------------------------------------------------------
void Engine::RigidBody::Initialize()
{
	RTTI::AddParentedType<RigidBody, EngineComp>();
	if (GetOwner())
		gPhysics->AddRigidBody(this);
	AddForce({ 0.f, -9.8f,0.f });
}

/// -----------------------------------------------------------------
/// Shutdown rigid body
/// -----------------------------------------------------------------
void Engine::RigidBody::Shutdown()
{
	gPhysics->RemoveRigidBody(this);
}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::RigidBody::ToJson(nlohmann::ordered_json& data)
{
	data["Is Static"] = mStatic;
	data["Has Gravity"] = mGravity;
	data["Mass"] = mMass;
	data["Drag"] = mDrag;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::RigidBody::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Is Static") != data.end())
		mStatic = data["Is Static"];
	if (data.find("Has Gravity") != data.end())
		mGravity = data["Has Gravity"];
	if (data.find("Mass") != data.end())
		mMass = data["Mass"];
	if (data.find("Drag") != data.end())
		mDrag = data["Drag"];
}

/// -----------------------------------------------------------------
/// Update physics of rigid body
/// -----------------------------------------------------------------
void Engine::RigidBody::ApplyForces()
{
	//If no mass or static, dont update
	if (mStatic || mMass == 0.f)
		return;

	//If kinematic, don't apply forces
	float dt = gTimeSys->GetDeltaTime();

	//Get forces and compute velocity of object
	glm::vec3 totalForces = glm::vec3(0.f);
	for (auto it : mForces)
		totalForces += it;
	totalForces += totalForces * mMass;
	totalForces /= mMass;

	//Compute velocity and update position
	mVelocity += totalForces * dt;
	mVelocity *= mDrag;

	//Update position with velocity
	glm::vec3 newPos = GetOwner()->GetTransform()->GetWorldPos() + mVelocity * dt;
	GetOwner()->GetTransform()->SetWorldPos(newPos);
}

/// -----------------------------------------------------------------
/// Add non existing force
/// -----------------------------------------------------------------
void Engine::RigidBody::AddForce(const glm::vec3& force)
{
	if (std::find(mForces.begin(), mForces.end(), force) == mForces.end())
		mForces.push_back(force);
}

/// -----------------------------------------------------------------
/// Remove existing force
/// -----------------------------------------------------------------
void Engine::RigidBody::RemoveForce(const glm::vec3& force)
{
	auto it = std::find(mForces.begin(), mForces.end(), force);
	if (it != mForces.end())
		mForces.erase(it);
}

/// -----------------------------------------------------------------
/// Set static mode
/// -----------------------------------------------------------------
void Engine::RigidBody::SetStatic(const bool stc)
{
	mStatic = stc;
	gPhysics->UpdateCollider(GetOwner()->GetEngineComp<Collider>());
}

/// -----------------------------------------------------------------
/// Check if it is static
/// -----------------------------------------------------------------
bool Engine::RigidBody::IsStatic() const
{
	return mStatic;
}

/// -----------------------------------------------------------------
/// Set gravity mode and update forces
/// -----------------------------------------------------------------
void Engine::RigidBody::SetGravity(const bool gravity)
{
	//Add or remove gravity, then set
	if (!mGravity && gravity)
		AddForce({ 0.f, -9.8f, 0.f });
	else if (mGravity && !gravity)
		RemoveForce({ 0.f, -9.8f, 0.f });
	mGravity = gravity;
}

/// -----------------------------------------------------------------
/// Check if it has gravity
/// -----------------------------------------------------------------
bool Engine::RigidBody::HasGravity() const
{
	return mGravity;
}

/// -----------------------------------------------------------------
/// Set mass value
/// -----------------------------------------------------------------
void Engine::RigidBody::SetMass(const float mass)
{
	mMass = mass;
}

/// -----------------------------------------------------------------
/// Get mass value
/// -----------------------------------------------------------------
float Engine::RigidBody::GetMass() const
{
	return mMass;
}

/// -----------------------------------------------------------------
/// Set velocity value
/// -----------------------------------------------------------------
void Engine::RigidBody::SetVelocity(const glm::vec3& vel)
{
	mVelocity = vel;
}

/// -----------------------------------------------------------------
/// Get velocity value
/// -----------------------------------------------------------------
glm::vec3 Engine::RigidBody::GetVelocity() const
{
	return mVelocity;
}

/// -----------------------------------------------------------------
/// Set drag value
/// -----------------------------------------------------------------
void Engine::RigidBody::SetDrag(const float drag)
{
	mDrag = drag;
}

/// -----------------------------------------------------------------
/// Get drag value
/// -----------------------------------------------------------------
float Engine::RigidBody::GetDrag() const
{
	return mDrag;
}
