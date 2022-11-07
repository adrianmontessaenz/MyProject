/*// -----------------------------------------------------------------
*  File:		Space.cpp
*  Brief:		Implementation of Space class
*  Creation:	07/11/2022
*  Last Update:	07/11/2022
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
	for (unsigned i = 0; i < mObjects.size(); i++)
	{
		//Remove object if shutdown
		if (mObjects[i]->IsShutdown())
		{
			DeleteObject(mObjects[i--]);
			continue;
		}
		else if (mObjects[i]->IsEnabled())
			mObjects[i]->LogicUpdate();
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
void Engine::Space::AddObject(Object* obj_, bool rec)
{
	//Only on the first call. Check if it was already added or if it was in another space
	if (!rec && obj_->GetSpace() == this)
		return;
	else if (!rec && obj_->GetSpace() != this && obj_->GetSpace() != nullptr)
		obj_->GetSpace()->RemoveObject(obj_);

	//Get parent and check
	obj_->SetSpace(this);
	Object* parent_ = obj_->GetParent();
	unsigned updateIdx = 0;
	if (parent_)
	{
		//If it has parent, insert children next to parent
		mObjects.insert(mObjects.begin() + parent_->GetSpaceIdx(), obj_);
		obj_->SetSpaceIdx(parent_->GetSpaceIdx() + obj_->GetParentIdx() + 1);
		updateIdx = parent_->GetSpaceIdx() + obj_->GetParentIdx() + 2;
	}
	else
	{
		//If no parent, insert at the end
		obj_->SetSpaceIdx(static_cast<const int>(mObjects.size()));
		mObjects.push_back(obj_);
		updateIdx = static_cast<unsigned>(mObjects.size());
	}

	//Add children to space too
	for (auto child : obj_->GetChildren())
		AddObject(child, true);

	//Last thing: Update object's indexes respect from where it was updated.
	if (!rec)
		UpdateObjectIdx(updateIdx);
}

/// -----------------------------------------------------------------
/// Removes object from space
/// -----------------------------------------------------------------
void Engine::Space::RemoveObject(Object* obj_, bool rec)
{
	//First remove children
	for (auto child : obj_->GetChildren())
		RemoveObject(child, true);

	//If parent, update from parent and remove object from parent
	Object* parent_ = obj_->GetParent();
	unsigned updateIdx = 0;
	if (parent_)
		updateIdx = parent_->GetSpaceIdx();
	else
		updateIdx = obj_->GetSpaceIdx() == 0 ? 0 : obj_->GetSpaceIdx() - 1;

	//Remove from list
	mObjects.erase(mObjects.begin() + obj_->GetSpaceIdx());
	obj_->SetSpace(nullptr);

	//Last thing: Update object's indexes respect from where it was updated.
	if (!rec)
		UpdateObjectIdx(updateIdx);
}

/// -----------------------------------------------------------------
/// Removes object from space and deletes it
/// -----------------------------------------------------------------
void Engine::Space::DeleteObject(Object* obj_, bool rec)
{
	//First remove children
	auto children = obj_->GetChildren();
	for (int idx = children.size() - 1; idx >= 0; idx--)
		DeleteObject(children[idx], true);

	//If parent, update from parent and remove object from parent
	if (obj_->IsShutdown() == false)
		obj_->Shutdown();

	Object* parent_ = obj_->GetParent();
	unsigned updateIdx = 0;
	if (parent_)
	{
		parent_->RemoveChild(obj_);
		updateIdx = parent_->GetSpaceIdx();
	}
	else
		updateIdx = obj_->GetSpaceIdx() == 0 ? 0 : obj_->GetSpaceIdx() - 1;

	//Remove from list
	mObjects.erase(mObjects.begin() + obj_->GetSpaceIdx());
	delete obj_;

	//Last thing: Update object's indexes respect from where it was updated.
	if (!rec)
		UpdateObjectIdx(updateIdx);
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
void Engine::Space::UpdateObjectIdx(unsigned idx_)
{
	for (unsigned it = idx_; it < mObjects.size(); it++)
		mObjects[it]->SetSpaceIdx(it);
}
