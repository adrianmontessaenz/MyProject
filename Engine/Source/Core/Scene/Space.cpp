/*// -----------------------------------------------------------------
*  File:		Space.cpp
*  Brief:		Implementation of Space class
*  Creation:	07/11/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Space.hpp"
#include <Core/Entity-Component/Object.hpp>

/// -----------------------------------------------------------------
/// Space initialization.
/// -----------------------------------------------------------------
void Engine::Space::Initialize()
{
	for (auto obj : mObjects)
		obj->Initialize();
}

/// -----------------------------------------------------------------
/// Space update. Updates all objects
/// -----------------------------------------------------------------
void Engine::Space::Update()
{
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	//Update objects
	for (int i = 0; i < mObjects.size();)
	{
		//Remove object if shutdown
		if (mObjects[i]->IsShutdown())
		{
			DeleteObject(mObjects[i]);
			continue;
		}
		else if(mObjects[i]->IsEnabled())
			mObjects[i]->Update();
		i++;
	}
}

/// -----------------------------------------------------------------
/// Space logic update. Logic Updates all objects
/// -----------------------------------------------------------------
void Engine::Space::LogicUpdate()
{
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	//Update objects
	for (unsigned i = 0; i < mObjects.size();)
	{
		//Remove object if shutdown
		if (mObjects[i]->IsShutdown())
		{
			DeleteObject(mObjects[i]);
			continue;
		}
		else if (mObjects[i]->IsEnabled())
			mObjects[i]->LogicUpdate();
		i++;
	}
}

/// -----------------------------------------------------------------
/// Space shutdown. Deletes all objects
/// -----------------------------------------------------------------
void Engine::Space::Shutdown()
{
	//Delete objects and components
	while (mObjects.empty() == false)
	{
		auto obj = *(mObjects.begin());
		obj->Shutdown();
		DeleteObject(obj);
	}
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::Space::ToJson(nlohmann::ordered_json& data)
{
	data["Space"] = GetName();
	nlohmann::ordered_json objs;
	for (auto obj : mObjects)
	{
		nlohmann::ordered_json objData;
		obj->ToJson(objData);
		objs.push_back(objData);
	}
	data["Objects"] = objs;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Space::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Space") != data.end())		//Remove in future
		SetName(data["Space"]);
	if (data.find("Objects") != data.end())		//Remove in future
	{
		for (auto obj : data["Objects"])
		{
			Object* tmp = AddObject();
			tmp->FromJson(obj);
		}
	}
}

/// -----------------------------------------------------------------
/// Set enable state
/// -----------------------------------------------------------------
void Engine::Space::SetEnabled(const bool& enable) noexcept
{
	mEnabled = enable;
}

/// -----------------------------------------------------------------
/// Check enable state
/// -----------------------------------------------------------------
bool Engine::Space::IsEnabled() const noexcept
{
	return mEnabled;
}

/// -----------------------------------------------------------------
/// Adds empty object to space
/// -----------------------------------------------------------------
Engine::Object* Engine::Space::AddObject()
{
	Object* obj = new Object;
	obj->Initialize();
	AddObject(obj);
	return obj;
}

/// -----------------------------------------------------------------
/// Adds specific object to space. If it was in another space, remove from prev space
/// -----------------------------------------------------------------
void Engine::Space::AddObject(Object* obj_)
{
	//Check if object is already on space
	if (obj_->GetSpace() == this)
		return;
	else if (obj_->GetSpace() != nullptr)
		obj_->GetSpace()->RemoveObject(obj_);

	//Set space and add to list
	obj_->SetSpace(this);
	mObjects.push_back(obj_);

	//Add children to space too
	for (auto child : obj_->GetChildren())
		AddObjectRecursive(child);
}

/// -----------------------------------------------------------------
/// Removes object from space
/// -----------------------------------------------------------------
void Engine::Space::RemoveObject(Object* obj_)
{
	//If not the same space, return
	if (obj_->GetSpace() != this)
		return;

	//First remove children
	for (auto child : obj_->GetChildren())
		RemoveObject(child);

	//Remove from list
	auto it = std::find(mObjects.begin(), mObjects.end(), obj_);
	mObjects.erase(it);
	obj_->SetSpace(nullptr);
}

/// -----------------------------------------------------------------
/// Removes object from space and deletes it
/// -----------------------------------------------------------------
void Engine::Space::DeleteObject(Object* obj_)
{
	//If not same space, return
	if (obj_->GetSpace() != this)
		return;

	//First delete children
	for (auto child : obj_->GetChildren())
		DeleteObjectRecursive(child);
	
	//Then remove from parent
	if (obj_->GetParent())
		obj_->GetParent()->RemoveChild(obj_);

	//If parent, update from parent and remove object from parent
	if (obj_->IsShutdown() == false)
		obj_->Shutdown();

	//Delete and remove from list
	auto it = std::find(mObjects.begin(), mObjects.end(), obj_);
	mObjects.erase(it);
	delete obj_;
}

/// -----------------------------------------------------------------
/// Swap objects in space array
/// -----------------------------------------------------------------
void Engine::Space::SwapObjects(const size_t& l_idx, const size_t& r_idx)
{
	Object* tmp = mObjects[l_idx];
	mObjects[l_idx] = mObjects[r_idx];
	mObjects[r_idx] = tmp;
}

/// -----------------------------------------------------------------
/// Finds object by name
/// -----------------------------------------------------------------
Engine::Object* Engine::Space::GetObjectByName(const std::string name_)
{
	for (auto obj : mObjects)
	{
		if (strcmp(name_.c_str(), obj->GetName().c_str()) == 0)
			return obj;
	}
	return nullptr;
}

/// -----------------------------------------------------------------
/// Finds list of objects with equal name
/// -----------------------------------------------------------------
std::vector<Engine::Object*> Engine::Space::GetObjectsByName(const std::string name_)
{
	std::vector<Object*> result;
	for (auto obj : mObjects)
	{
		if (strcmp(name_.c_str(), obj->GetName().c_str()) == 0)
			result.push_back(obj);
	}
	return result;
}

/// -----------------------------------------------------------------
/// Finds object with unique id
/// -----------------------------------------------------------------
Engine::Object* Engine::Space::GetObjectByID(const unsigned id_)
{
	for (auto obj : mObjects)
	{
		if (obj->GetUniqueID() == id_)
			return obj;
	}
	return nullptr;
}

/// -----------------------------------------------------------------
/// Gets all space objects
/// -----------------------------------------------------------------
std::vector<Engine::Object*> Engine::Space::GetObjects() const
{
	return mObjects;
}

/// -----------------------------------------------------------------
/// Add objects recursively to space (for adding children next to parent)
/// -----------------------------------------------------------------
void Engine::Space::AddObjectRecursive(Object* obj_)
{
	//Set space and add to list
	obj_->SetSpace(this);
	mObjects.push_back(obj_);

	//Add children to space too
	for (auto child : obj_->GetChildren())
		AddObjectRecursive(child);
}

/// -----------------------------------------------------------------
/// Deletes objects recursively from space
/// -----------------------------------------------------------------
void Engine::Space::DeleteObjectRecursive(Object* obj_)
{
	//First remove children
	for (auto child : obj_->GetChildren())
		DeleteObjectRecursive(child);

	//If parent, update from parent and remove object from parent
	if (obj_->IsShutdown() == false)
		obj_->Shutdown();

	//Remove from list and delete
	auto it = std::find(mObjects.begin(), mObjects.end(), obj_);
	mObjects.erase(it);
	delete obj_;
}
