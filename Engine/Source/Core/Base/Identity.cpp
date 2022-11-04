/*// -----------------------------------------------------------------
*  File:		Identity.cpp
*  Brief:		Implementation of Identity class
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

#include <pch.h>
#include "Identity.hpp"

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
/// Sets unique id of identity
/// -----------------------------------------------------------------
void Engine::Identity::SetUniqueID(const unsigned id)
{
	mUID = id;
}

/// -----------------------------------------------------------------
/// Gets its unique id
/// -----------------------------------------------------------------
const unsigned Engine::Identity::GetUniqueID() const
{
	return mUID;
}
