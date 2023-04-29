/*// -----------------------------------------------------------------
*  File:		Transform.cpp
*  Brief:		Implementation of Transform Component
*  Creation:	04/11/2022
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Transform.hpp"
#include <Core/Entity-Component/Object.hpp>

/// -----------------------------------------------------------------
/// Initialize matrices
/// -----------------------------------------------------------------
void Engine::Transform::Initialize()
{
	RTTI::AddParentedType<Transform, Serialized>();
	UpdateWorld();
	UpdateLocal();
}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::Transform::ToJson(nlohmann::ordered_json& data)
{
	data["World Coords"]["Pos"] << mWPos;
	data["World Coords"]["Sca"] << mWScale;
	data["World Coords"]["Rot"] << mWRot;

	data["Local Coords"]["Pos"] << mLPos;
	data["Local Coords"]["Sca"] << mLScale;
	data["Local Coords"]["Rot"] << mLRot;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Transform::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("World Coords") != data.end())	//Remove in future
	{
		if (data["World Coords"].find("Pos") != data["World Coords"].end())
			data["World Coords"]["Pos"] >> mWPos;
		if (data["World Coords"].find("Sca") != data["World Coords"].end())
			data["World Coords"]["Sca"] >> mWScale;
		if (data["World Coords"].find("Rot") != data["World Coords"].end())
			data["World Coords"]["Rot"] >> mWRot;
	}
	if (data.find("Local Coords") != data.end())	//Remove in future
	{
		if (data["Local Coords"].find("Pos") != data["Local Coords"].end())
			data["Local Coords"]["Pos"] >> mLPos;
		if (data["Local Coords"].find("Sca") != data["Local Coords"].end())
			data["Local Coords"]["Sca"] >> mLScale;
		if (data["Local Coords"].find("Rot") != data["Local Coords"].end())
			data["Local Coords"]["Rot"] >> mLRot;
	}
}

/// -----------------------------------------------------------------
/// Sets world position
/// -----------------------------------------------------------------
void Engine::Transform::SetWorldPos(const vec3 pos)
{
	mWPos = pos;
	ComputeWorldMat();
	UpdateLocal();
}

/// -----------------------------------------------------------------
/// Sets world scale
/// -----------------------------------------------------------------
void Engine::Transform::SetWorldScale(const vec3 sca)
{
	mWScale = sca;
	ComputeWorldMat();
	UpdateLocal();
}

/// -----------------------------------------------------------------
/// Sets world rotation
/// -----------------------------------------------------------------
void Engine::Transform::SetWorldRot(const vec3 rot)
{
	mWRot = rot;
	ComputeWorldMat();
	UpdateLocal();
}

/// -----------------------------------------------------------------
/// Gets world position
/// -----------------------------------------------------------------
const vec3& Engine::Transform::GetWorldPos() const
{
	return mWPos;
}

/// -----------------------------------------------------------------
/// Gets world scale
/// -----------------------------------------------------------------
const vec3& Engine::Transform::GetWorldScale() const
{
	return mWScale;
}

/// -----------------------------------------------------------------
/// Gets world rotation
/// -----------------------------------------------------------------
const vec3& Engine::Transform::GetWorldRot() const
{
	return mWRot;
}

/// -----------------------------------------------------------------
/// Gets world matrix
/// -----------------------------------------------------------------
const mat4& Engine::Transform::GetWorldMat() const
{
	return mWMat;
}

/// -----------------------------------------------------------------
/// Sets local position
/// -----------------------------------------------------------------
void Engine::Transform::SetLocalPos(const vec3 pos)
{
	mLPos = pos;
	UpdateWorld();
}

/// -----------------------------------------------------------------
/// Sets local scale
/// -----------------------------------------------------------------
void Engine::Transform::SetLocalScale(const vec3 sca)
{
	mLScale = sca;
	UpdateWorld();
}

/// -----------------------------------------------------------------
/// Sets local rotation
/// -----------------------------------------------------------------
void Engine::Transform::SetLocalRot(const vec3 rot)
{
	mLRot = rot;
	UpdateWorld();
}

/// -----------------------------------------------------------------
/// Gets local position
/// -----------------------------------------------------------------
const vec3& Engine::Transform::GetLocalPos() const
{
	return mLPos;
}

/// -----------------------------------------------------------------
/// Gets local scale
/// -----------------------------------------------------------------
const vec3& Engine::Transform::GetLocalScale() const
{
	return mLScale;
}

/// -----------------------------------------------------------------
/// Gets local rotation
/// -----------------------------------------------------------------
const vec3& Engine::Transform::GetLocalRot() const
{
	return mLRot;
}

/// -----------------------------------------------------------------
/// Adds child transform
/// -----------------------------------------------------------------
void Engine::Transform::AddChild(Transform* child)
{
	//Don't add twice
	if (child->mParent == this)
		return;
	mChildren.push_back(child);
	child->mParent = this;
}

/// -----------------------------------------------------------------
/// Remove child
/// -----------------------------------------------------------------
void Engine::Transform::RemoveChild(Transform* child)
{
	if (child->mParent != this)
		return;
	mChildren.erase(std::find(mChildren.begin(), mChildren.end(), child));
	child->mParent = nullptr;
}

/// -----------------------------------------------------------------
/// Sets parent
/// -----------------------------------------------------------------
void Engine::Transform::SetParent(Transform* parent)
{
	if (mParent)
		mParent->RemoveChild(this);
	if (parent)
		parent->AddChild(this);
}

/// -----------------------------------------------------------------
/// Gets parent
/// -----------------------------------------------------------------
Engine::Transform* Engine::Transform::GetParent() const
{
	return mParent;
}

/// -----------------------------------------------------------------
/// Updates world variables depending on local variables
/// -----------------------------------------------------------------
void Engine::Transform::UpdateWorld()
{
	//If no parent, set locals as world too
	if (!mParent)
	{
		mWPos = mLPos;
		mWScale = mLScale;
		mWRot = mLRot;
	}

	//Update world with parent
	else
	{
		mWPos = mParent->mWMat*glm::vec4(mLPos, 1.f);
		mWScale = mLScale * mParent->mWScale;
		mWRot = mLRot + mParent->mWRot;
	}
	ComputeWorldMat();

	//Update children world
	size_t childCount = mChildren.size();
	for (size_t idx = 0; idx < childCount; idx++)
		mChildren[idx]->UpdateWorld();
}

/// -----------------------------------------------------------------
/// Computes world matrix
/// -----------------------------------------------------------------
void Engine::Transform::ComputeWorldMat()
{
	mWMat = glm::identity<mat4>();
	mWMat = glm::translate(mWMat, mWPos);
	mWMat = glm::rotate(mWMat, glm::radians(mWRot.x), glm::vec3(0.f, 1.f, 0.f));
	mWMat = glm::rotate(mWMat, glm::radians(mWRot.y), glm::vec3(0.f, 0.f, 1.f));
	mWMat = glm::rotate(mWMat, glm::radians(mWRot.z), glm::vec3(1.f, 0.f, 0.f));
	mWMat = glm::scale(mWMat, mWScale);
	mWIMat = glm::inverse(mWMat);
}

/// -----------------------------------------------------------------
/// Updates local variables based on world variables
/// -----------------------------------------------------------------
void Engine::Transform::UpdateLocal()
{
	//If no parent, set worlds as locals too
	if (!mParent)
	{
		mLPos = mWPos;
		mLScale = mWScale;
		mLRot = mWRot;
	}
	//Update local with parent world too
	else
	{
		mLPos = mParent->mWIMat * glm::vec4(mWPos, 1.f);
		mLScale = mWScale / mParent->mWScale;
		mLRot = mWRot - mParent->mWRot;
	}

	//Update children world
	size_t childCount = mChildren.size();
	for (size_t idx = 0; idx < childCount; idx++)
		mChildren[idx]->UpdateWorld();
}
