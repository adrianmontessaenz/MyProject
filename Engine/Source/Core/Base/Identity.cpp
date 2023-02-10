/*// -----------------------------------------------------------------
*  File:		Identity.cpp
*  Brief:		Implementation of Identity class
*  Creation:	21/10/2022
*  Last Update:	10/02/2023
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
Engine::Identity::Identity() : mUID(0)
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
	mName = name;
}

/// -----------------------------------------------------------------
/// Gets name as string
/// -----------------------------------------------------------------
std::string Engine::Identity::GetName() const
{
	return mName;
}

/// -----------------------------------------------------------------
/// Gets its unique id
/// -----------------------------------------------------------------
const unsigned Engine::Identity::GetUniqueID() const
{
	return mUID;
}
