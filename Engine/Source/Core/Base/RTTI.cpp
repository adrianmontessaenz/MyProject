/*// -----------------------------------------------------------------
*  File:		RTTI.hpp
*  Brief:		Implementation for RTTI
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "RTTI.hpp"
std::vector<Engine::TypeInfo*> Engine::RTTI::sTypes;

void Engine::TypeInfo::SetTypeName(const std::string name)
{
	//If not new, throw (since a type name shouldn't change)
	if (mTypeName != nullptr)
	{
		gDebugSys->Log("Trying to re-define its typename", mTypeName);
		throw;
	}

	//If new, create it
	char* new_name = new char[name.length() + 1]{ 0 };
	strcpy(new_name, name.c_str());
	mTypeName = new_name;
}

void Engine::TypeInfo::AddParentType(const std::string parent)
{
	//If it is already in the list, don't add it
	if (HasParent(parent))
		return;

	//If new, add it
	char* name = new char[parent.length() + 1]{ 0 };
	strcpy(name, parent.c_str());
	mParentNames.push_back(name);
}

const std::string Engine::TypeInfo::GetTypeName() const
{
	return std::string(mTypeName);
}

const std::vector<const char*>& Engine::TypeInfo::GetParentNames() const
{
	return mParentNames;
}

bool Engine::TypeInfo::HasParent(const std::string name) const
{
	//If found, it is child of
	for (auto parents : mParentNames)
	{
		if (strcmp(name.c_str(), parents) == 0)
			return true;
	}

	//Not child of
	return false;
}
