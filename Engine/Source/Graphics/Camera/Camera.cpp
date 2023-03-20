/*// -----------------------------------------------------------------
*  File:		Camera.cpp
*  Brief:		Implementation for camera
*  Creation:	06/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Camera.hpp"

/// -----------------------------------------------------------------
/// Initialize camera
/// -----------------------------------------------------------------
void Engine::Camera::Initialize() noexcept
{
	RTTI::AddParentedType<Camera, EngineComp>();
	if(mSize == glm::vec<2,int>(0,0))
		mSize = gWindow->GetSize();
	if (GetOwner() != nullptr)
	{
		mOwnerTransform = GetOwner()->GetTransform();
		mOwnerTransform->SetWorldPos({ 0.f,0.f,2.f });
		mOwnerTransform->SetWorldRot({ 0.f,0.f,-1.f });
	}
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
	data["Transform"] = transform;

	//Store planes
	nlohmann::ordered_json planes;
	planes["FOV"] = mFOV;
	planes["Near"] = mSize[0];
	planes["Far"] = mSize[1];
	data["Planes"] = planes;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Camera::FromJson(const nlohmann::ordered_json& data)
{	
	if (data.find("Transform") != data.end())
	{
		auto transform = data["Transform"];
		mSize[0] = transform["Width"];
		mSize[1] = transform["Height"];
	}
	if (data.find("Planes") != data.end())
	{
		auto planes = data["Planes"];
		mFOV = planes["FOV"];
		mPlanes[0] = planes["Near"];
		mPlanes[1] = planes["Far"];
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
/// Get up vector
/// -----------------------------------------------------------------
glm::vec3 Engine::Camera::GetUp() const
{
	return mUp;
}

/// -----------------------------------------------------------------
/// Get front vector
/// -----------------------------------------------------------------
glm::vec3 Engine::Camera::GetFront() const
{
	return mFront;
}

/// -----------------------------------------------------------------
/// Get right vector
/// -----------------------------------------------------------------
glm::vec3 Engine::Camera::GetRight() const
{
	return glm::cross(mOwnerTransform->GetWorldRot(), mUp);
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
	//Get position and rotation of camera
	glm::vec3 pos = mOwnerTransform->GetWorldPos();
	glm::vec3 rot = mOwnerTransform->GetWorldRot();

	//Compute right and up vectors
	mFront = glm::normalize(rot);
	mRight = glm::normalize(glm::cross(mFront, { 0.f,1.f,0.f }));
	mUp = glm::normalize(glm::cross(mRight, mFront));

	//Compute matrices
	mView = glm::lookAt(pos, pos + mFront, mUp);
	mProj = glm::perspective(glm::radians(mFOV), (float)mSize[0] / (float)mSize[1], mPlanes[0], mPlanes[1]);
}
