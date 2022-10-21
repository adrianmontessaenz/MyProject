/*// -----------------------------------------------------------------
*  File:		Object.cpp
*  Brief:		Implementation of Object class
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Object.hpp"

void Engine::Object::Initialize()
{
	//Add to RTTI
	RTTI::AddParentedType<Object, RunTime>();
	RTTI::AddParentedType<Object, Identity>();

	for (auto comp : mEngineComps)
		comp->Initialize();
	for (auto comp : mLogicComps)
		comp->Initialize();
}

void Engine::Object::Update()
{
	for (auto comp : mEngineComps)
		comp->Update();
}

void Engine::Object::LogicUpdate()
{
	for (auto comp : mLogicComps)
		comp->LogicUpdate();
}

void Engine::Object::Shutdown()
{
	for (auto comp : mEngineComps)
		comp->Shutdown();
	for (auto comp : mLogicComps)
		comp->Shutdown();
}

void Engine::Object::AddChildren(Object* child)
{
	//Find if it is on list. If not, add it
	if (std::find(mChildren.begin(), mChildren.end(), child) != mChildren.end())
		return;
	mChildren.push_back(child);
}

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

void Engine::Object::SetParent(Object* parent)
{
	mParent = parent;
}

Engine::Object* Engine::Object::GetParent() const
{
	return mParent;
}
