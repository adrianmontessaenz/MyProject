/*// -----------------------------------------------------------------
*  File:		ObjectManager.cpp
*  Brief:		Implementation file of ObjectManager class
*  Creation:	07/11/2022
*  Last Update:	10/02/2023
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
/// Write to json
/// -----------------------------------------------------------------
void Engine::ObjectManager::ToJson(nlohmann::ordered_json& data)
{
	nlohmann::ordered_json spaces;
	for (auto spa : mSpaces)
		spa->ToJson(spaces);
	data["Spaces"] = spaces;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::ObjectManager::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Spaces") != data.end())	//Remove in future
	{
		for (auto spa : data["Spaces"])
		{
			Space* tmp = new Space;
			tmp->FromJson(spa);
			mSpaces.push_back(tmp);
		}
	}
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
	if (space_ == nullptr)
		return;
	auto it = std::find(mSpaces.begin(), mSpaces.end(), space_);
	if(it == mSpaces.end())
		mSpaces.push_back(space_);
}

/// -----------------------------------------------------------------
/// Removes space and deletes it
/// -----------------------------------------------------------------
void Engine::ObjectManager::DeleteSpace(Space* space_)
{
	if (space_ == nullptr)
		return;
	if (space_->IsShutdown() == false)
		space_->Shutdown();
	auto it = std::find(mSpaces.begin(), mSpaces.end(), space_);
	if(it != mSpaces.end())
		mSpaces.erase(it);
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
/// Swaps two spaces' order
/// -----------------------------------------------------------------
void Engine::ObjectManager::SwapSpaces(const size_t& l_idx, const size_t& r_idx)
{
	Space* tmp = mSpaces[l_idx];
	mSpaces[l_idx] = mSpaces[r_idx];
	mSpaces[r_idx] = tmp;
}

/// -----------------------------------------------------------------
/// Updates list of spaces
/// -----------------------------------------------------------------
void Engine::ObjectManager::LoadScene(const std::string& lvl)
{
	nlohmann::ordered_json data = OpenJson(lvl);
	FromJson(data);
}

/// -----------------------------------------------------------------
/// Updates list of spaces
/// -----------------------------------------------------------------
void Engine::ObjectManager::SaveScene(const std::string& lvl)
{
	nlohmann::ordered_json data;
	data["Level"] = lvl;
	ToJson(data);
	SaveJson(lvl, data);
}
