/*// -----------------------------------------------------------------
*  File:		Object.cpp
*  Brief:		Implementation of Object class
*  Creation:	21/10/2022
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Object.hpp"
#include <Graphics/Renderable/Renderable.hpp>
#include <Graphics/Camera/Camera.hpp>
#include <Physics/Components/Collider.hpp>
#include <Physics/Components/RigidBody.hpp>

/// -----------------------------------------------------------------
/// Object destructor
/// -----------------------------------------------------------------
Engine::Object::~Object()
{
	size_t engineCompCount = mEngineComps.size();
	size_t logicCompCount = mLogicComps.size();
	for (size_t idx = 0; idx < engineCompCount; idx++)
		delete mEngineComps[idx];
	for (size_t idx = 0; idx < logicCompCount; idx++)
		delete mLogicComps[idx];
}

/// -----------------------------------------------------------------
/// Object initialization.
/// -----------------------------------------------------------------
void Engine::Object::Initialize()
{
	//Add to RTTI and create transform
	RTTI::AddParentedType<Object, RunTime>();
	RTTI::AddParentedType<Object, Identity>();
	if (!mTransform)
		mTransform = new Transform;
	mTransform->Initialize();

	//Initialize comps
	size_t engineCompCount = mEngineComps.size();
	size_t logicCompCount = mLogicComps.size();
	for (size_t idx = 0; idx < engineCompCount; idx++)
		mEngineComps[idx]->Initialize();
	for (size_t idx = 0; idx < logicCompCount; idx++)
		mLogicComps[idx]->Initialize();
}

/// -----------------------------------------------------------------
/// Updates objects' components
/// -----------------------------------------------------------------
void Engine::Object::Update()
{
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	size_t engineCompCount = mEngineComps.size();
	EngineComp* comp = nullptr;
	for (size_t idx = 0; idx < engineCompCount; idx++)
	{
		comp = mEngineComps[idx];
		if (comp->IsShutdown())
		{
			mEngineComps.erase(mEngineComps.begin() + idx);
			idx--;
		}
		else if(comp->IsActive())
			comp->Update();
	}
}

/// -----------------------------------------------------------------
/// Updates objects' logic
/// -----------------------------------------------------------------
void Engine::Object::LogicUpdate()
{
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	size_t logicCompCount = mLogicComps.size();
	LogicComp* comp = nullptr;
	for (size_t idx = 0; idx < logicCompCount; idx++)
	{
		comp = mLogicComps[idx];
		if (comp->IsShutdown())
		{
			mLogicComps.erase(mLogicComps.begin() + idx);
			idx--;
		}
		else if (comp->IsActive())
			comp->Update();
	}
}

/// -----------------------------------------------------------------
///	Shutdowns object
/// -----------------------------------------------------------------
void Engine::Object::Shutdown()
{
	//If it has children, shutdown children
	size_t engineCompCount = mEngineComps.size();
	size_t logicCompCount = mLogicComps.size();
	size_t childCount = mChildren.size();

	//Shutdown and set shutdown
	for (size_t idx = 0; idx < childCount; idx++)
		mChildren[idx]->Shutdown();
	for (size_t idx = 0; idx < engineCompCount; idx++)
		mEngineComps[idx]->Shutdown();
	for (size_t idx = 0; idx < logicCompCount; idx++)
		mLogicComps[idx]->Shutdown();
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Write to json
/// -----------------------------------------------------------------
void Engine::Object::ToJson(nlohmann::ordered_json& data)
{
	data["Object"] = GetName();
	nlohmann::ordered_json transform;
	mTransform->ToJson(transform);
	data["Transform"] = transform;

	nlohmann::ordered_json ecomps;
	for (auto ecomp : mEngineComps)
	{
		nlohmann::ordered_json ecompData;
		ecompData["Component"] = ecomp->TypeInfo()->GetTypeName();
		ecomp->ToJson(ecompData);
		ecomps.push_back(ecompData);
	}
	data["Engine Comps"] = ecomps;
	nlohmann::ordered_json lcomps;
	for (auto lcomp : mLogicComps)
	{
		nlohmann::ordered_json lcompData;
		lcompData["Component"] = lcomp->TypeInfo()->GetTypeName();
		lcomp->ToJson(lcompData);
		lcomps.push_back(lcompData);
	}
	data["Logic Comps"] = lcomps;
	nlohmann::ordered_json children;
	for (auto child : mChildren)
	{
		nlohmann::ordered_json childData;
		child->ToJson(childData);
		children.push_back(childData);
	}
	data["Children"] = children;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::Object::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Object") != data.end())
		SetName(data["Object"]);
	if (data.find("Transform") != data.end())
	{
		if(!mTransform)
			mTransform = new Transform;
		mTransform->FromJson(data["Transform"]);
	}
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
/// Enable or disable object
/// -----------------------------------------------------------------
void Engine::Object::SetEnabled(const bool& enable) noexcept
{
	mEnabled = enable;
}

/// -----------------------------------------------------------------
/// Check if enable
/// -----------------------------------------------------------------
bool Engine::Object::IsEnabled() const noexcept
{
	return mEnabled;
}

/// -----------------------------------------------------------------
///	Add engine comp manually
/// -----------------------------------------------------------------
Engine::EngineComp* Engine::Object::AddEngineComp(std::string name)
{
	if (name == "Renderable")
		return AddEngineComp<Renderable>();
	if (name == "Camera")
		return AddEngineComp<Camera>();
	if (name == "Collider")
		return AddEngineComp<Collider>();
	if (name == "RigidBody")
		return AddEngineComp<RigidBody>();
	return nullptr;
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
/// Add logic comp manually
/// -----------------------------------------------------------------
Engine::Transform* Engine::Object::GetTransform() const
{
	return mTransform;
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
	mTransform->AddChild(child->mTransform);
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
	mTransform->RemoveChild(child->mTransform);
}

/// -----------------------------------------------------------------
/// Gets children by name
/// -----------------------------------------------------------------
Engine::Object* Engine::Object::GetChildByName(const std::string name) const
{
	//Find child
	size_t childCount = mChildren.size();
	Object* child = nullptr;
	for (size_t idx = 0; idx < childCount; idx++)
	{
		child = mChildren[idx];
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
	//Find child
	size_t childCount = mChildren.size();
	Object* child = nullptr;
	std::vector<Object*> list;
	for (size_t idx = 0; idx < childCount; idx++)
	{
		child = mChildren[idx];
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

/// -----------------------------------------------------------------
/// Set tag to object
/// -----------------------------------------------------------------
void Engine::Object::SetTag(const std::string tag)
{
	mTag = tag;
}

/// -----------------------------------------------------------------
/// Get object tag
/// -----------------------------------------------------------------
const std::string Engine::Object::GetTag() const
{
	return mTag;
}
