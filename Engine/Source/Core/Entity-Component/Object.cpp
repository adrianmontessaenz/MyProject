/*// -----------------------------------------------------------------
*  File:		Object.cpp
*  Brief:		Implementation of Object class
*  Creation:	21/10/2022
*  Last Update:	14/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Object.hpp"

/// -----------------------------------------------------------------
/// Object destructor
/// -----------------------------------------------------------------
Engine::Object::~Object()
{
	for (auto comp : mEngineComps)
		delete comp;
	for (auto comp : mLogicComps)
		delete comp;
}

/// -----------------------------------------------------------------
/// Object initialization.
/// -----------------------------------------------------------------
void Engine::Object::Initialize()
{
	//Add to RTTI
	RTTI::AddParentedType<Object, RunTime>();
	RTTI::AddParentedType<Object, Identity>();

	//If no components, add transform to object
	if (mEngineComps.empty())
		AddEngineComp<Transform>();

	for (auto comp : mEngineComps)
		comp->Initialize();
	for (auto comp : mLogicComps)
		comp->Initialize();
}

/// -----------------------------------------------------------------
/// Updates objects' components
/// -----------------------------------------------------------------
void Engine::Object::Update()
{
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	for (auto comp : mEngineComps)
		comp->Update();
}

/// -----------------------------------------------------------------
/// Updates objects' logic
/// -----------------------------------------------------------------
void Engine::Object::LogicUpdate()
{
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	for (auto comp : mLogicComps)
		comp->LogicUpdate();
}

/// -----------------------------------------------------------------
///	Shutdowns object
/// -----------------------------------------------------------------
void Engine::Object::Shutdown()
{
	//If it has children, shutdown children
	for (auto child : mChildren)
		child->Shutdown();

	for (auto comp : mEngineComps)
		comp->Shutdown();
	for (auto comp : mLogicComps)
		comp->Shutdown();
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Adds children to parent list
/// -----------------------------------------------------------------
void Engine::Object::AddChild(Object* child)
{
	//If this parent was already assigned, don't add again
	Object* parent_ = child->GetParent();
	if (parent_ == this)
		return;
	//If it had another parent, remove from previous parent
	else if (parent_ != nullptr)
		parent_->RemoveChild(child);

	//Set index on list and add it
	child->SetParentIdx(static_cast<const int>(mChildren.size()));
	mChildren.push_back(child);
	child->mParent = this;
	mSpace->MoveObject(child, child->mParent->GetSpaceIdx() + child->mParentIdx + 1);
}

/// -----------------------------------------------------------------
/// Removes child from parent list
/// -----------------------------------------------------------------
void Engine::Object::RemoveChild(Object* child)
{
	//If not the same parent, don't remove
	if (child->GetParent() != this)
		return;

	Engine::Object* last_child = GetChildren().back();
	while (last_child->GetChildren().empty() == false)
		last_child = last_child->GetChildren().back();

	mSpace->MoveObject(child, last_child->GetSpaceIdx() + 1);
	child->mParent = nullptr;
	mChildren.erase(mChildren.begin() + child->GetParentIdx());
	UpdateParentIdx(child->GetParentIdx() == 0 ? 0 : child->GetParentIdx() - 1);
	child->SetParentIdx(-1);
}

/// -----------------------------------------------------------------
/// Gets children by name
/// -----------------------------------------------------------------
Engine::Object* Engine::Object::GetChildByName(const std::string name) const
{
	//Find
	for (auto child : mChildren)
	{
		if (strcmp(child->GetName().c_str(), name.c_str()) == 0)
			return child;
	}

	//not found
	return nullptr;
}

/// -----------------------------------------------------------------
/// Gets list of children that share the same name
/// -----------------------------------------------------------------
std::vector<Engine::Object*> Engine::Object::GetChildrenByName(const std::string name) const
{
	//Find
	std::vector<Object*> list;
	for (auto child : mChildren)
	{
		if (strcmp(child->GetName().c_str(), name.c_str()) == 0)
			list.push_back(child);
	}

	//List filled
	return list;
}

/// -----------------------------------------------------------------
/// Gets Children list
/// -----------------------------------------------------------------
const std::vector<Engine::Object*> Engine::Object::GetChildren() const
{
	return mChildren;
}

/// -----------------------------------------------------------------
/// Swaps two children on parent's children array
/// -----------------------------------------------------------------
void Engine::Object::SwapChildren(const size_t& l_idx, const size_t& r_idx)
{
	Object* tmp = mChildren[l_idx];
	mChildren[l_idx] = mChildren[r_idx];
	mChildren[l_idx]->SetParentIdx(static_cast<int>(l_idx));

	mChildren[r_idx] = tmp;
	mChildren[r_idx]->SetParentIdx(static_cast<int>(r_idx));

	mSpace->SwapObjects(r_idx, l_idx);
}

/// -----------------------------------------------------------------
/// Sets parent to object
/// -----------------------------------------------------------------
void Engine::Object::SetParent(Object* parent)
{
	//If parent remove. Then set and check if parent to add child to its list
	if (mParent)
		mParent->RemoveChild(this);
	if (parent)
		parent->AddChild(this);
}

/// -----------------------------------------------------------------
/// Gets an object's parent
/// -----------------------------------------------------------------
Engine::Object* Engine::Object::GetParent() const
{
	return mParent;
}

/// -----------------------------------------------------------------
/// Sets index on parent array
/// -----------------------------------------------------------------
void Engine::Object::SetParentIdx(const int idx_)
{
	mParentIdx = idx_;
}

/// -----------------------------------------------------------------
/// Gets index on parent array
/// -----------------------------------------------------------------
const int Engine::Object::GetParentIdx() const
{
	return mParentIdx;
}

/// -----------------------------------------------------------------
/// Sets space of object
/// -----------------------------------------------------------------
void Engine::Object::SetSpace(Engine::Space* space_)
{
	mSpace = space_;
}

/// -----------------------------------------------------------------
/// Gets an object's space
/// -----------------------------------------------------------------
Engine::Space* Engine::Object::GetSpace() const
{
	return mSpace;
}

/// -----------------------------------------------------------------
/// Sets an object's index on space
/// -----------------------------------------------------------------
void Engine::Object::SetSpaceIdx(const int idx_)
{
	mSpaceIdx = idx_;
}

/// -----------------------------------------------------------------
/// Gets an object's index on space
/// -----------------------------------------------------------------
const int Engine::Object::GetSpaceIdx() const
{
	return mSpaceIdx;
}

/// -----------------------------------------------------------------
/// Update index of objects in parent list
/// -----------------------------------------------------------------
void Engine::Object::UpdateParentIdx(unsigned idx_)
{
	for (unsigned it = idx_; it < mChildren.size(); it++)
		mChildren[it]->SetParentIdx(it);
}
