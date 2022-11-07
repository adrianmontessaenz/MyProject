/*// -----------------------------------------------------------------
*  File:		ObjectManager.cpp
*  Brief:		Implementation file of ObjectManager class
*  Creation:	07/11/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "ObjectManager.hpp"

/// -----------------------------------------------------------------
/// Scene initialization
/// -----------------------------------------------------------------
void Engine::ObjectManager::Initialize()
{
	if (mSpaces.empty())
		return;
	for (auto space : mSpaces)
		space->Initialize();
}

/// -----------------------------------------------------------------
/// Main update on objects
/// -----------------------------------------------------------------
void Engine::ObjectManager::Update()
{
	for (int i = 0; i < mSpaces.size(); i++)
	{
		if (mSpaces[i]->IsShutdown())
		{
			i--;
			DeleteSpace(mSpaces[i]);
			continue;
		}
		if (mSpaces[i]->IsEnabled())
			mSpaces[i]->Update();
	}
}

/// -----------------------------------------------------------------
/// Logic update on objects
/// -----------------------------------------------------------------
void Engine::ObjectManager::LogicUpdate()
{
	for (int i = 0; i < mSpaces.size(); i++)
	{
		if (mSpaces[i]->IsShutdown())
		{
			i--;
			DeleteSpace(mSpaces[i]);
			continue;
		}
		if (mSpaces[i]->IsEnabled())
			mSpaces[i]->LogicUpdate();
	}
}

/// -----------------------------------------------------------------
/// Removes all the spaces and deletes them
/// -----------------------------------------------------------------
void Engine::ObjectManager::Shutdown()
{
	while(mSpaces.empty() == false)
		DeleteSpace(mSpaces[0]);
	mSpaces.clear();
	mSpaces.shrink_to_fit();
}

/// -----------------------------------------------------------------
/// Creates and adds new space
/// -----------------------------------------------------------------
Engine::Space* Engine::ObjectManager::AddSpace()
{
	Space* tmp = new Space;
	tmp->Initialize();
	AddSpace(tmp);
	return tmp;
}

/// -----------------------------------------------------------------
/// Adds space to list
/// -----------------------------------------------------------------
void Engine::ObjectManager::AddSpace(Space* space_)
{
	mSpaces.push_back(space_);
	space_->SetSceneIdx(mSpaces.size() - 1);
}

/// -----------------------------------------------------------------
/// Removes space and deletes it
/// -----------------------------------------------------------------
void Engine::ObjectManager::DeleteSpace(Space* space_)
{
	if (space_->IsShutdown() == false)
		space_->Shutdown();
	mSpaces.erase(mSpaces.begin() + space_->GetSceneIdx());
	UpdateSpaceList(space_->GetSceneIdx());
	delete space_;
}

/// -----------------------------------------------------------------
/// Gets first space with given name
/// -----------------------------------------------------------------
Engine::Space* Engine::ObjectManager::GetSpaceByName(const std::string name_)
{
	for (auto space : mSpaces)
	{
		if (strcmp(space->GetName().c_str(), name_.c_str()) == 0)
			return space;
	}
	return nullptr;
}

/// -----------------------------------------------------------------
/// Gets list of spaces with given name
/// -----------------------------------------------------------------
std::vector<Engine::Space*> Engine::ObjectManager::GetSpacesByName(const std::string name_)
{
	std::vector<Space*> result;
	for (auto space : mSpaces)
	{
		if (strcmp(space->GetName().c_str(), name_.c_str()) == 0)
			result.push_back(space);
	}
	return result;
}

/// -----------------------------------------------------------------
/// Gets space by given id
/// -----------------------------------------------------------------
Engine::Space* Engine::ObjectManager::GetSpaceByID(const unsigned id_)
{
	for (auto space : mSpaces)
	{
		if (space->GetUniqueID() == id_)
			return space;
	}
	return nullptr;
}

/// -----------------------------------------------------------------
/// Gets list of spaces
/// -----------------------------------------------------------------
std::vector<Engine::Space*> Engine::ObjectManager::GetSpaces() const
{
	return mSpaces;
}

/// -----------------------------------------------------------------
/// Updates list of spaces
/// -----------------------------------------------------------------
void Engine::ObjectManager::UpdateSpaceList(unsigned idx_)
{
	for (unsigned i = idx_; i < mSpaces.size(); i++)
		mSpaces[i]->SetSceneIdx(i);
}
