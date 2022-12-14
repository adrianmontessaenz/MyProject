/*// -----------------------------------------------------------------
*  File:		Identity.cpp
*  Brief:		Implementation of Identity class
*  Creation:	21/10/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

#include <pch.h>
#include "Identity.hpp"

//Static
unsigned Engine::Identity::sNextId = 0;
std::stack<unsigned> Engine::Identity::sUnusedID;

/// -----------------------------------------------------------------
/// Identity constructor
/// -----------------------------------------------------------------
Engine::Identity::Identity() : mName(nullptr), mUID(0)
{
	if (!sUnusedID.empty())
	{
		mUID = sUnusedID.top();
		sUnusedID.pop();
	}
	else
		mUID = sNextId++;
}

/// -----------------------------------------------------------------
/// Identity destructor
/// -----------------------------------------------------------------
Engine::Identity::~Identity()
{
	//Delete name and push id to unused
	delete mName;
	sUnusedID.push(mUID);

	//If no ids in use, clear vector
	if (sUnusedID.size() == sNextId)
	{
		while(sUnusedID.empty() == false)
			sUnusedID.pop();
		sNextId = 0;
	}
}

/// -----------------------------------------------------------------
/// Sets name of identity
/// -----------------------------------------------------------------
void Engine::Identity::SetName(const std::string name)
{
	//Delete previous name
	if (mName != nullptr)
	{
		delete mName;
		mName = nullptr;
	}

	//If empty name, return
	if (name.empty())
		return;

	//Create new name
	mName = new char[name.length() + 1]{ 0 };
	strcpy(mName, name.c_str());
}

/// -----------------------------------------------------------------
/// Gets name as string
/// -----------------------------------------------------------------
std::string Engine::Identity::GetName() const
{
	return std::string(mName);
}

/// -----------------------------------------------------------------
/// Gets its unique id
/// -----------------------------------------------------------------
const unsigned Engine::Identity::GetUniqueID() const
{
	return mUID;
}
