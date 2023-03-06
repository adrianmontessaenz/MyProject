/*// -----------------------------------------------------------------
*  File:		Camera.cpp
*  Brief:		Implementation for camera
*  Creation:	06/03/2023
*  Last Update:	06/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Camera.hpp"
#include <Core/Entity-Component/Object.hpp>

/// -----------------------------------------------------------------
/// Initialize camera
/// -----------------------------------------------------------------
void Engine::Camera::Initialize() noexcept
{
	RTTI::AddParentedType<Camera, EngineComp>();
	if(mSize == glm::vec<2,int>(0,0))
		mSize = gWindow->GetSize();
	if(GetOwner() != nullptr)
		mOwnerTransform = GetOwner()->GetTransform();
}

/// -----------------------------------------------------------------
/// Update camera if it changed
/// -----------------------------------------------------------------
void Engine::Camera::Update() noexcept
{
	UpdateMatrices();
}

/// -----------------------------------------------------------------
/// Empty shutdown
/// -----------------------------------------------------------------
void Engine::Camera::Shutdown() noexcept
{}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::Camera::ToJson(nlohmann::ordered_json& data)
{
	//Store transform
	nlohmann::ordered_json transform;
	transform["Width"] = mSize[0];
	transform["Height"] = mSize[1];
	data["Transform"].push_back(transform);

	//Store planes
	nlohmann::ordered_json planes;
	planes["FOV"] = mFOV;
	planes["Near"] = mSize[0];
	planes["Far"] = mSize[1];
	data["Planes"].push_back(planes);

	//Store movement
	nlohmann::ordered_json movement;
	movement["Speed"] = mSpeed;
	movement["Sensitivity"] = mSensitiviy;
	data["Movement"].push_back(movement);
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Camera::FromJson(const nlohmann::ordered_json& data)
{	
	if (data.find("Transform") != data.end())
	{
		mSize[0] = data["Transform"]["Width"];
		mSize[1] = data["Transform"]["Height"];
	}
	if (data.find("Planes") != data.end())
	{
		mFOV = data["Planes"]["FOV"];
		mPlanes[0] = data["Planes"]["Near"];
		mPlanes[1] = data["Planes"]["Far"];
	}
	if (data.find("Movement") != data.end())
	{
		mSpeed = data["Movement"]["Speed"];
		mSensitiviy = data["Movement"]["Sensitivity"];
	}
}

/// -----------------------------------------------------------------
/// Set camera width
/// -----------------------------------------------------------------
void Engine::Camera::SetWidth(const int width)
{
	mSize[0] = width;
}

/// -----------------------------------------------------------------
/// Set camera height
/// -----------------------------------------------------------------
void Engine::Camera::SetHeight(const int height)
{
	mSize[1] = height;
}

/// -----------------------------------------------------------------
/// Set camera size
/// -----------------------------------------------------------------
void Engine::Camera::SetSize(const glm::vec2& size)
{
	mSize = size;
}

/// -----------------------------------------------------------------
/// Get size of camera
/// -----------------------------------------------------------------
glm::vec2 Engine::Camera::GetSize() const
{
	return mSize;
}

/// -----------------------------------------------------------------
/// Set field of view
/// -----------------------------------------------------------------
void Engine::Camera::SetFOV(const float fov)
{
	mFOV = fov;
}

/// -----------------------------------------------------------------
/// Get field of view
/// -----------------------------------------------------------------
float Engine::Camera::GetFOV() const
{
	return mFOV;
}

/// -----------------------------------------------------------------
/// Set near plane
/// -----------------------------------------------------------------
void Engine::Camera::SetNear(const float near)
{
	mPlanes[0] = near;
}

/// -----------------------------------------------------------------
/// Set far plane
/// -----------------------------------------------------------------
void Engine::Camera::SetFar(const float far)
{
	mPlanes[1] = far;
}

/// -----------------------------------------------------------------
/// Get both planes
/// -----------------------------------------------------------------
glm::vec2 Engine::Camera::GetPlanes() const
{
	return mPlanes;
}

/// -----------------------------------------------------------------
/// Set speed of camera
/// -----------------------------------------------------------------
void Engine::Camera::SetSpeed(const float speed)
{
	mSpeed = speed;
}

/// -----------------------------------------------------------------
/// Get speed
/// -----------------------------------------------------------------
float Engine::Camera::GetSpeed() const
{
	return mSpeed;
}

/// -----------------------------------------------------------------
///	Set sensitivity
/// -----------------------------------------------------------------
void Engine::Camera::SetSensitivity(const float sens)
{
	mSensitiviy = sens;
}

/// -----------------------------------------------------------------
/// Get sensitivity
/// -----------------------------------------------------------------
float Engine::Camera::GetSensitivity() const
{
	return mSensitiviy;
}

/// -----------------------------------------------------------------
/// Get view matrix
/// -----------------------------------------------------------------
glm::mat4 Engine::Camera::GetView() const
{
	return mView;
}

/// -----------------------------------------------------------------
/// Get projection matrix
/// -----------------------------------------------------------------
glm::mat4 Engine::Camera::GetProj() const
{
	return mProj;
}

/// -----------------------------------------------------------------
/// Update matrices if any change
/// -----------------------------------------------------------------
void Engine::Camera::UpdateMatrices()
{
	//Setup view and projection
	glm::vec3 pos = mOwnerTransform->GetWorldPos();
	glm::vec3 rot = mOwnerTransform->GetWorldRot();
	mView = glm::lookAt(pos, pos + rot, mUp);
	mProj = glm::perspective(glm::radians(mFOV), (float)(mSize[0] / mSize[1]), mPlanes[0], mPlanes[1]);	
}
