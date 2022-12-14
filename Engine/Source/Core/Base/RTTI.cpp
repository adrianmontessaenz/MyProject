/*// -----------------------------------------------------------------
*  File:		RTTI.hpp
*  Brief:		Implementation for RTTI
*  Creation:	21/10/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "RTTI.hpp"
std::vector<Engine::TypeInfo*> Engine::RTTI::sTypes;

/// -----------------------------------------------------------------
/// Type info destructor
/// -----------------------------------------------------------------
Engine::TypeInfo::~TypeInfo()
{
	delete mTypeName;
	for (auto parent : mParentNames)
		delete parent;
	mParentNames.clear();
	mParentNames.shrink_to_fit();
}

/// -----------------------------------------------------------------
/// Sets name of typeinfo
/// -----------------------------------------------------------------
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

/// -----------------------------------------------------------------
/// Adds parent name to type info
/// -----------------------------------------------------------------
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

/// -----------------------------------------------------------------
/// Gets typename as string
/// -----------------------------------------------------------------
const std::string Engine::TypeInfo::GetTypeName() const
{
	return std::string(mTypeName);
}

/// -----------------------------------------------------------------
/// Gets list of parents
/// -----------------------------------------------------------------
const std::vector<std::string> Engine::TypeInfo::GetParentNames() const
{
	std::vector<std::string> copy_vec;
	for (auto parent : mParentNames)
		copy_vec.push_back(std::string(parent));
	return copy_vec;
}

/// -----------------------------------------------------------------
/// Checks if object has parent of given name
/// -----------------------------------------------------------------
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

/// -----------------------------------------------------------------
/// Get types that have parent type
/// -----------------------------------------------------------------
std::vector<std::string> Engine::RTTI::GetTypesWithParent(const std::string name)
{
	std::vector<std::string> result;
	for (auto type : sTypes)
	{
		if (type->HasParent(name))
			result.push_back(type->GetTypeName());
	}
	return result;
}

/// -----------------------------------------------------------------
/// RTTI type destructor
/// -----------------------------------------------------------------
void Engine::RTTI::FreeTypeInfos()
{
	for (auto type : sTypes)
		delete type;
	sTypes.clear();
	sTypes.shrink_to_fit();
}
