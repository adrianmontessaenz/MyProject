/*// -----------------------------------------------------------------
*  File:		Object.cpp
*  Brief:		Implementation of Object class
*  Creation:	21/10/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Object.hpp"
#include <Graphics/Renderable.hpp>

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
/// Write to json
/// -----------------------------------------------------------------
void Engine::Object::ToJson(nlohmann::ordered_json& data)
{
	data["Name"] = GetName();
	nlohmann::ordered_json ecomps;
	for (auto ecomp : mEngineComps)
		ecomp->ToJson(ecomps);
	data["Engine Comps"] = ecomps;
	nlohmann::ordered_json lcomps;
	for (auto lcomp : mLogicComps)
		lcomp->ToJson(lcomps);
	data["Logic Comps"] = lcomps;
	nlohmann::ordered_json children;
	for (auto child : mChildren)
		child->ToJson(children);
	data["Children"] = children;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Object::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Name") != data.end())
		SetName(data["Name"]);
	if (data.find("Engine Comps") != data.end())
	{
		for (auto ecomp : data["Engine Comps"])
		{
			std::string name = ecomp["Component"];
			EngineComp* cmp = AddEngineComp(name);
			cmp->FromJson(ecomp);
		}
	}
	if (data.find("Logic Comps") != data.end())
	{
		for (auto lcomp : data["Logic Comps"])
		{
			std::string name = lcomp["Component"];
			LogicComp* cmp = AddLogicComp(name);
			cmp->FromJson(lcomp);
		}
	}
	if (data.find("Children") != data.end())
	{
		for (auto child : data["Children"])
		{
			Object* ch = new Object;
			AddChild(ch);
			ch->FromJson(child);
		}
	}
}

/// -----------------------------------------------------------------
///	Add engine comp manually
/// -----------------------------------------------------------------
Engine::EngineComp* Engine::Object::AddEngineComp(std::string name)
{
	if (name == "Transform")
		return AddEngineComp<Transform>();
	else if (name == "Renderable")
		return AddEngineComp<Renderable>();
}

/// -----------------------------------------------------------------
/// Gets engine components
/// -----------------------------------------------------------------
std::vector<Engine::EngineComp*> Engine::Object::GetEngineComps() const
{
	return mEngineComps;
}

/// -----------------------------------------------------------------
/// Add logic comp manually
/// -----------------------------------------------------------------
Engine::LogicComp* Engine::Object::AddLogicComp(std::string name)
{
	return nullptr;
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
	mChildren.push_back(child);
	child->mParent = this;
	child->mSpace = mSpace;
}

/// -----------------------------------------------------------------
/// Removes child from parent list
/// -----------------------------------------------------------------
void Engine::Object::RemoveChild(Object* child)
{
	//If not the same parent, don't remove
	if (child->GetParent() != this)
		return;

	//Erase child from list and remove its parent
	auto it = std::find(mChildren.begin(), mChildren.end(), child);
	mChildren.erase(it);
	child->mParent = nullptr;
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
	mChildren[r_idx] = tmp;
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
