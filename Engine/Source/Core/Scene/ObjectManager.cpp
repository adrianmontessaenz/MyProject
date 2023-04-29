/*// -----------------------------------------------------------------
*  File:		ObjectManager.cpp
*  Brief:		Implementation file of ObjectManager class
*  Creation:	07/11/2022
*  Last Update:	28/04/2023
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
	size_t spaceCount = mSpaces.size();
	if (mSpaces.empty())
		return;
	for (size_t idx = 0; idx < spaceCount; idx++)
		mSpaces[idx]->Initialize();
}

/// -----------------------------------------------------------------
/// Main update on objects
/// -----------------------------------------------------------------
void Engine::ObjectManager::Update()
{
	size_t spaceCount = mSpaces.size();
	Space* space = nullptr;
	for (size_t idx = 0; idx < spaceCount; idx++)
	{
		space = mSpaces[idx];
		if (space->IsShutdown())
		{
			idx--;
			DeleteSpace(space);
			continue;
		}
		if (space->IsEnabled())
			space->Update();
	}
}

/// -----------------------------------------------------------------
/// Logic update on objects
/// -----------------------------------------------------------------
void Engine::ObjectManager::LogicUpdate()
{
	size_t spaceCount = mSpaces.size();
	Space* space = nullptr;
	for (size_t idx = 0; idx < spaceCount; idx++)
	{
		space = mSpaces[idx];
		if (space->IsShutdown())
		{
			idx--;
			DeleteSpace(space);
			continue;
		}
		if (space->IsEnabled())
			space->LogicUpdate();
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
	data["Level"] = mLvlName;
	nlohmann::ordered_json spaces;
	for (auto spa : mSpaces)
	{
		nlohmann::ordered_json spaceData;
		spa->ToJson(spaceData);
		spaces.push_back(spaceData);
	}
	data["Spaces"] = spaces;
}

/// -----------------------------------------------------------------
/// Read from json
/// -----------------------------------------------------------------
void Engine::ObjectManager::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Level") != data.end())
		mLvlName = data["Level"];
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
	size_t spaceCount = mSpaces.size();
	Space* space = nullptr;
	for (size_t idx = 0; idx < spaceCount; idx++)
	{
		space = mSpaces[idx];
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
	size_t spaceCount = mSpaces.size();
	Space* space = nullptr;
	for (size_t idx = 0; idx < spaceCount; idx++)
	{
		space = mSpaces[idx];
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
	size_t spaceCount = mSpaces.size();
	Space* space = nullptr;
	for (size_t idx = 0; idx < spaceCount; idx++)
	{
		space = mSpaces[idx];
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
/// Sets level name
/// -----------------------------------------------------------------
void Engine::ObjectManager::SetLvlName(std::string name)
{
	mLvlName = name;
}

/// -----------------------------------------------------------------
/// Gets level name
/// -----------------------------------------------------------------
std::string Engine::ObjectManager::GetLvlName() const
{
	return mLvlName;
}

/// -----------------------------------------------------------------
/// Updates list of spaces
/// -----------------------------------------------------------------
void Engine::ObjectManager::LoadScene(const std::string& file)
{
	nlohmann::ordered_json data = OpenJson(file);
	FromJson(data);
}

/// -----------------------------------------------------------------
/// Updates list of spaces
/// -----------------------------------------------------------------
void Engine::ObjectManager::SaveScene(const std::string& file)
{
	nlohmann::ordered_json data;
	ToJson(data);
	SaveJson(file, data);
}
