/*// -----------------------------------------------------------------
*  File:		RigidBody.cpp
*  Brief:		Implementation of RigidBody
*  Creation:	19/03/2023
*  Last Update:	28/04/2023
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
	if(mGravity)
		AddForce("Gravity", { 0.f, -9.8f, 0.f });
}

/// -----------------------------------------------------------------
/// Shutdown rigid body
/// -----------------------------------------------------------------
void Engine::RigidBody::Shutdown()
{
	gPhysics->RemoveRigidBody(this);
	SetShutdown(true);
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
	glm::vec3 totalForces = GetTotalForces();
	totalForces += totalForces * mMass;
	totalForces /= mMass;

	//Compute velocity and update position
	mVelocity += totalForces * dt * mDrag;

	//Update position with velocity
	glm::vec3 newPos = GetOwner()->GetTransform()->GetWorldPos() + mVelocity * dt;
	GetOwner()->GetTransform()->SetWorldPos(newPos);
}

/// -----------------------------------------------------------------
/// Add non existing force
/// -----------------------------------------------------------------
void Engine::RigidBody::AddForce(const std::string& name, const glm::vec3& force)
{
	//If exists, update and return
	size_t forceCount = mForces.size();
	for (size_t idx = 0; idx < forceCount; idx++)
	{
		std::pair<std::string, glm::vec3>& myForce = mForces[idx];
		if (myForce.first == name)
		{
			myForce.second = force;
			return;
		}
	}

	//Otherwise just create it
	mForces.push_back({ name, force });
}

/// -----------------------------------------------------------------
/// Remove existing force
/// -----------------------------------------------------------------
void Engine::RigidBody::RemoveForce(const std::string& name)
{
	size_t forceCount = mForces.size();
	for (size_t idx = 0; idx < forceCount; idx++)
	{
		std::pair<std::string, glm::vec3>& myForce = mForces[idx];
		if (myForce.first == name)
		{
			mForces.erase(mForces.begin() + idx);
			return;
		}
	}
}

/// -----------------------------------------------------------------
/// Checks if force exists
/// -----------------------------------------------------------------
bool Engine::RigidBody::HasForce(const std::string& name)
{
	size_t forceCount = mForces.size();
	for (size_t idx = 0; idx < forceCount; idx++)
		if (mForces[idx].first == name)
			return true;
	return false;
}

/// -----------------------------------------------------------------
/// Sum all forces
/// -----------------------------------------------------------------
glm::vec3 Engine::RigidBody::GetTotalForces()
{
	glm::vec3 result = glm::vec3(0.f);
	size_t forceCount = mForces.size();
	for (size_t idx = 0; idx < forceCount; idx++)
		result += mForces[idx].second;
	return result;
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
		AddForce("Gravity", { 0.f, -9.8f, 0.f });
	else if (mGravity && !gravity)
		RemoveForce("Gravity");
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
