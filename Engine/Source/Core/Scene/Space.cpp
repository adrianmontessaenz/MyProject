/*// -----------------------------------------------------------------
*  File:		Space.cpp
*  Brief:		Implementation of Space class
*  Creation:	07/11/2022
*  Last Update:	15/12/2022
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
			DeleteObject(mObjects[i], i);
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
			DeleteObject(mObjects[i], i);
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
		DeleteObject(obj, 0);
	}
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Adds empty object to space
/// -----------------------------------------------------------------
Engine::Object* Engine::Space::AddObject()
{
	Object* obj = new Object;
	obj->Initialize();
	AddObject(obj, mObjects.size());
	return obj;
}

/// -----------------------------------------------------------------
/// Adds specific object to space. If it was in another space, remove from prev space
/// -----------------------------------------------------------------
void Engine::Space::AddObject(Object* obj_, const size_t& idx)
{
	//Only on the first call. Check if it was already added or if it was in another space
	if (obj_->GetSpace() == this)
		return;
	else if (obj_->GetSpace() != nullptr && obj_->GetSpace() != this)
		obj_->GetSpace()->RemoveObject(obj_, obj_->GetSpaceIdx());

	//Set space and add to list
	obj_->SetSpace(this);
	if (idx >= mObjects.size())
		mObjects.push_back(obj_);
	else
		mObjects.insert(mObjects.begin() + idx, obj_);
	obj_->SetSpaceIdx(static_cast<int>(idx));

	//Add children to space too
	size_t upd_idx = idx + 1;
	for (auto child : obj_->GetChildren())
		upd_idx = AddObjectRecursive(child, upd_idx);
}

/// -----------------------------------------------------------------
/// Removes object from space
/// -----------------------------------------------------------------
void Engine::Space::RemoveObject(Object* obj_, const size_t& idx)
{
	if (obj_->GetSpace() != this)
		return;

	//First remove children
	for (auto child : obj_->GetChildren())
		RemoveObject(child, idx + child->GetParentIdx());

	//Remove from list
	mObjects.erase(mObjects.begin() + idx);
	obj_->SetSpace(nullptr);
	obj_->SetSpaceIdx(-1);
}

/// -----------------------------------------------------------------
/// Removes object from space and deletes it
/// -----------------------------------------------------------------
void Engine::Space::DeleteObject(Object* obj_, const size_t& idx)
{
	if (obj_->GetSpace() != this)
		return;

	//First remove children
	for (auto child : obj_->GetChildren())
		DeleteObjectRecursive(child, idx + child->GetParentIdx());
	
	//Then remove from parent
	if (obj_->GetParent())
		obj_->GetParent()->RemoveChild(obj_);

	//If parent, update from parent and remove object from parent
	if (obj_->IsShutdown() == false)
		obj_->Shutdown();

	//Remove from list
	mObjects.erase(mObjects.begin() + obj_->GetSpaceIdx());
	delete obj_;
	UpdateObjectIdx(idx >= 1 ? idx - 1 : 0);
}

/// -----------------------------------------------------------------
/// Swap objects in space array
/// -----------------------------------------------------------------
void Engine::Space::SwapObjects(const size_t& l_idx, const size_t& r_idx)
{
	Object* tmp = mObjects[l_idx];
	mObjects[l_idx] = mObjects[r_idx];
	mObjects[l_idx]->SetSpaceIdx(static_cast<int>(l_idx));

	mObjects[r_idx] = tmp;
	mObjects[r_idx]->SetSpaceIdx(static_cast<int>(r_idx));
}

/// -----------------------------------------------------------------
/// Moves object to given index and updates index list
/// -----------------------------------------------------------------
void Engine::Space::MoveObject(Object* obj, const size_t& idx)
{
	size_t prev_idx = obj->GetSpaceIdx();
	if (prev_idx > idx)
	{
		RemoveObject(obj, prev_idx);
		AddObject(obj, idx);
		UpdateObjectIdx(idx);
	}
	else if (prev_idx < idx)
	{
		size_t last_idx = prev_idx;
		Engine::Object* last_child = obj;
		while (last_child->GetChildren().empty() == false)
		{
			last_child = last_child->GetChildren().back();
			last_idx = last_child->GetSpaceIdx();
		}
		RemoveObject(obj, prev_idx);
		AddObject(obj, idx - (last_idx - prev_idx) - 1);
		UpdateObjectIdx(prev_idx);
	}
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
/// Sets index on scene list of spaces
/// -----------------------------------------------------------------
void Engine::Space::SetSceneIdx(const int idx_)
{
	mSceneIdx = idx_;
}

/// -----------------------------------------------------------------
/// Gets index on scene list of spaces
/// -----------------------------------------------------------------
const int Engine::Space::GetSceneIdx() const
{
	return mSceneIdx;
}

/// -----------------------------------------------------------------
/// Updates object's indexes from space
/// -----------------------------------------------------------------
void Engine::Space::UpdateObjectIdx(const size_t& idx_)
{
	for (size_t it = idx_; it < mObjects.size(); it++)
		mObjects[it]->SetSpaceIdx(static_cast<int>(it));
}

/// -----------------------------------------------------------------
/// Add objects recursively to space (for adding children next to parent)
/// -----------------------------------------------------------------
size_t Engine::Space::AddObjectRecursive(Object* obj_, const size_t& upd_idx_)
{
	//Set space and add to list
	obj_->SetSpace(this);
	if (upd_idx_ >= mObjects.size())
		mObjects.push_back(obj_);
	else
		mObjects.insert(mObjects.begin() + upd_idx_, obj_);
	obj_->SetSpaceIdx(static_cast<int>(upd_idx_));

	//Add children to space too
	size_t upd_idx = upd_idx_ + 1;
	for (auto child : obj_->GetChildren())
		upd_idx = AddObjectRecursive(child, upd_idx);
	return upd_idx;
}

/// -----------------------------------------------------------------
/// Deletes objects recursively from space
/// -----------------------------------------------------------------
void Engine::Space::DeleteObjectRecursive(Object* obj_, const size_t& idx)
{
	//First remove children
	for (auto child : obj_->GetChildren())
		DeleteObjectRecursive(child, idx + child->GetParentIdx());

	//If parent, update from parent and remove object from parent
	if (obj_->IsShutdown() == false)
		obj_->Shutdown();

	//Remove from list
	mObjects.erase(mObjects.begin() + obj_->GetSpaceIdx());
	delete obj_;
}
