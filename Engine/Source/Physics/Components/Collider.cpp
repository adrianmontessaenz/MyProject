/*// -----------------------------------------------------------------
*  File:		Collider.cpp
*  Brief:		Implementation of collider
*  Creation:	19/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Collider.hpp"
#include <Physics/PhysicsManager.hpp>

/// -----------------------------------------------------------------
/// Collider initialize
/// -----------------------------------------------------------------
void Engine::Collider::Initialize()
{
	RTTI::AddParentedType<Collider, EngineComp>();
	ComputeMyMatrix();
	gPhysics->AddCollider(this);
}

/// -----------------------------------------------------------------
/// Empty shutdown
/// -----------------------------------------------------------------
void Engine::Collider::Shutdown()
{
	gPhysics->RemoveCollider(this);
}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::Collider::ToJson(nlohmann::ordered_json& data)
{
	//Copy offsets
	data["Offset Position"] << mPos;
	data["Offset Scale"] << mSca;
	data["Offset Rotation"] << mRot;

	//Copy other data
	data["Is Ghost"] = mGhost;
	data["Type"] = COLLIDER_AABB;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Collider::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Offset Position") != data.end())
		data["Offset Position"] >> mPos;
	if (data.find("Offset Scale") != data.end())
		data["Offset Scale"] >> mSca;
	if (data.find("Offset Rotation") != data.end())
		data["Offset Rotation"] >> mRot;
	if (data.find("Is Ghost") != data.end())
		mGhost = data["Is Ghost"];
	if (data.find("Type") != data.end())
		mType = data["Type"];
}

/// -----------------------------------------------------------------
/// Set collider offset position
/// -----------------------------------------------------------------
void Engine::Collider::SetOffsetPosition(const glm::vec3& pos)
{
	mPos = pos;
	ComputeMyMatrix();
}

/// -----------------------------------------------------------------
/// Get collider offset position
/// -----------------------------------------------------------------
glm::vec3 Engine::Collider::GetOffsetPosition() const
{
	return mPos;
}

/// -----------------------------------------------------------------
/// Set collider offset scale
/// -----------------------------------------------------------------
void Engine::Collider::SetOffsetScale(const glm::vec3& sca)
{
	mSca = sca;
	ComputeMyMatrix();
}

/// -----------------------------------------------------------------
/// Get collider offset scale
/// -----------------------------------------------------------------
glm::vec3 Engine::Collider::GetOffsetScale() const
{
	return mSca;
}

/// -----------------------------------------------------------------
/// Set collider offset rotation
/// -----------------------------------------------------------------
void Engine::Collider::SetOffsetRotation(const glm::vec3& rot)
{
	mRot = rot;
}

/// -----------------------------------------------------------------
/// Get collider offset rotation
/// -----------------------------------------------------------------
glm::vec3 Engine::Collider::GetOffsetRotation() const
{
	return mRot;
}

/// -----------------------------------------------------------------
/// Get collider offset matrix
/// -----------------------------------------------------------------
glm::mat4 Engine::Collider::GetOffsetMatrix() const
{
	return mMat;
}

/// -----------------------------------------------------------------
/// Set collider ghost
/// -----------------------------------------------------------------
void Engine::Collider::SetGhost(const bool ghost)
{
	mGhost = ghost;
}

/// -----------------------------------------------------------------
/// Check if collider is ghost
/// -----------------------------------------------------------------
bool Engine::Collider::IsGhost() const
{
	return mGhost;
}

/// -----------------------------------------------------------------
/// Set collider drawn state
/// -----------------------------------------------------------------
void Engine::Collider::SetDrawCollider(const bool draw)
{
	mDraw = draw;
}

/// -----------------------------------------------------------------
/// Check if collider is drawn
/// -----------------------------------------------------------------
bool Engine::Collider::IsColliderDrawn() const
{
	return mDraw;
}

/// -----------------------------------------------------------------
/// Sets collider type
/// -----------------------------------------------------------------
void Engine::Collider::SetColliderType(ColliderType type)
{
	mType = type;
}

/// -----------------------------------------------------------------
/// Gets collider type
/// -----------------------------------------------------------------
int Engine::Collider::GetColliderType() const
{
	return mType;
}

/// -----------------------------------------------------------------
/// Compute collider offset matrix
/// -----------------------------------------------------------------
void Engine::Collider::ComputeMyMatrix()
{
	mMat = glm::translate(mPos);
	mMat *= glm::rotate(glm::radians(mRot.x), glm::vec3(0.f, 1.f, 0.f));
	mMat *= glm::rotate(glm::radians(mRot.y), glm::vec3(0.f, 0.f, 1.f));
	mMat *= glm::rotate(glm::radians(mRot.z), glm::vec3(1.f, 0.f, 0.f));
	mMat *= glm::scale(mSca);
}
