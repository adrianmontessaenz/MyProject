/*// -----------------------------------------------------------------
*  File:		Transform.cpp
*  Brief:		Implementation of Transform Component
*  Creation:	04/11/2022
*  Last Update:	15/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Transform.hpp"
#include <Core/Entity-Component/Object.hpp>

/// -----------------------------------------------------------------
/// Update if it has a parent
/// -----------------------------------------------------------------
void Engine::Transform::Initialize()
{
	RTTI::AddParentedType<Transform, EngineComp>();
	if (GetOwner() == nullptr || GetOwner()->GetParent() == nullptr)
		return;

	UpdateWorld();
	UpdateLocal();
}

/// -----------------------------------------------------------------
/// Empty shutdown
/// -----------------------------------------------------------------
void Engine::Transform::Shutdown(){}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::Transform::ToJson(nlohmann::ordered_json& data)
{
	data["Component"] = TYPE()->GetTypeName();
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
			data["Local Coords"]["Pos"] >> mWPos;
		if (data["Local Coords"].find("Sca") != data["Local Coords"].end())
			data["Local Coords"]["Sca"] >> mWScale;
		if (data["Local Coords"].find("Rot") != data["Local Coords"].end())
			data["Local Coords"]["Rot"] >> mWRot;
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
	ComputeLocalMat();
	UpdateWorld();
}

/// -----------------------------------------------------------------
/// Sets local scale
/// -----------------------------------------------------------------
void Engine::Transform::SetLocalScale(const vec3 sca)
{
	mLScale = sca;
	ComputeLocalMat();
	UpdateWorld();
}

/// -----------------------------------------------------------------
/// Sets local rotation
/// -----------------------------------------------------------------
void Engine::Transform::SetLocalRot(const vec3 rot)
{
	mLRot = rot;
	ComputeLocalMat();
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
/// Gets local matrix
/// -----------------------------------------------------------------
const mat4& Engine::Transform::GetLocalMat() const
{
	return mLMat;
}

/// -----------------------------------------------------------------
/// Updates world variables depending on local variables
/// -----------------------------------------------------------------
void Engine::Transform::UpdateWorld()
{
	//If no parent, set locals as world too
	Object* parent = GetOwner()->GetParent();
	if (parent == nullptr)
	{
		mWPos = mLPos;
		mWScale = mLScale;
		mWRot = mLRot;
	}

	//Update world with parent
	else
	{
		int debug = 0;
		Transform* pTrans = parent->GetEngineComp<Transform>(&debug);
		assert(debug == 1); //Objects by default should have transform
		if (pTrans != nullptr)
		{
			mWPos = pTrans->mWMat*glm::vec4(mLPos, 1.f);
			mWScale = mLScale * pTrans->mWScale;
			mWRot = mLRot + pTrans->mWRot;
		}
	}
	ComputeWorldMat();

	//Update children world
	for (auto child : GetOwner()->GetChildren())
	{
		int debug = 0;
		Transform* cTrans = child->GetEngineComp<Transform>(&debug);
		assert(debug == 1); //Objects by default should have transform
		cTrans->UpdateWorld();
	}
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
	Object* parent = GetOwner()->GetParent();
	if (parent == nullptr)
	{
		mLPos = mWPos;
		mLScale = mWScale;
		mLRot = mWRot;
	}
	//Update local with parent world too
	else
	{
		int debug = 0;
		Transform* pTrans = parent->GetEngineComp<Transform>(&debug);
		assert(debug == 1); //Objects by default should have transform
		if (pTrans != nullptr)
		{
			mLPos = pTrans->mWIMat * glm::vec4(mWPos, 1.f);
			mLScale = mWScale / pTrans->mWScale;
			mLRot = mWRot - pTrans->mWRot;
		}
	}
	ComputeLocalMat();

	//Update children local
	for (auto child : GetOwner()->GetChildren())
	{
		int debug = 0;
		Transform* cTrans = child->GetEngineComp<Transform>(&debug);
		assert(debug == 1); //Objects by default should have transform
		cTrans->UpdateWorld();
	}
}

/// -----------------------------------------------------------------
/// Computes local matrix
/// -----------------------------------------------------------------
void Engine::Transform::ComputeLocalMat()
{
	mLMat = glm::identity<mat4>();
	mLMat = glm::translate(mLMat, mLPos);
	mLMat = glm::rotate(mLMat, glm::radians(mLRot.x), glm::vec3(0.f, 1.f, 0.f));
	mLMat = glm::rotate(mLMat, glm::radians(mLRot.y), glm::vec3(0.f, 0.f, 1.f));
	mLMat = glm::rotate(mLMat, glm::radians(mLRot.z), glm::vec3(1.f, 0.f, 0.f));
	mLMat = glm::scale(mLMat, mLScale);
}
