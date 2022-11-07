#include "Object.hpp"
/*// -----------------------------------------------------------------
*  File:		Object.inl
*  Brief:		Implementation of templated Object class functions
*  Creation:	21/10/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

/// -----------------------------------------------------------------
/// Adds engine component to object
/// -----------------------------------------------------------------
template<typename Type>
inline Type* Engine::Object::AddEngineComp(int* debug)
{
	//If it is not an engine component or already added, return
	int failed = 2;
	EngineComp* comp = GetEngineComp<Type>(&failed);
	if (failed != 0)
	{
		//-1: Not EngineComp, 1: Already Added
		if (debug)
			*debug = failed;
		return reinterpret_cast<Type*>(comp);
	}

	//If not, create and add
	comp = new Type;
	comp->SetOwner(this);
	mEngineComps.push_back(comp);

	//Added correctly
	if (debug)
		*debug = failed;	
	return reinterpret_cast<Type*>(comp);
}

/// -----------------------------------------------------------------
/// Gets engine component from object.
/// Returns -1 if it is not an engine component
/// Returns 0 if it was not added to object
/// Returns 1 if success
/// -----------------------------------------------------------------
template<typename Type>
inline Type* Engine::Object::GetEngineComp(int* debug)
{
	//If it is not an engine component or already added, return
	const Engine::TypeInfo* info = Type::TYPE();
	if (info == nullptr)
	{
		//Create type for the first time to be able to get type
		Type tmp;
		tmp.Initialize();
		info = Type::TYPE();
		tmp.Shutdown();
	}
	if (!info->HasParent(RTTI::TypeName<EngineComp>()))
	{
		if (debug)
			*debug = -1;
		return nullptr;
	}
	std::string typeName = info->GetTypeName();

	//Search for component. If found, set it
	for (auto component : mEngineComps)
	{
		if (strcmp(component->TypeInfo()->GetTypeName().c_str(), typeName.c_str()) == 0)
		{
			if (debug)
				*debug = 1;
			return reinterpret_cast<Type*>(component);
		}
	}

	//Not found
	if (debug)
		*debug = 0;
	return nullptr;
}

/// -----------------------------------------------------------------
/// Deletes engine component from object
/// -----------------------------------------------------------------
template<typename Type>
inline void Engine::Object::DeleteEngineComp(int* debug)
{
	//If it is not an engine component or not added, return
	int failed = 2;
	EngineComp* comp = GetEngineComp<Type>(&failed);
	if (failed <= 0)
	{
		if (debug)
			*debug = failed;
		return;
	}

	//Check that is shutdown and then delete it
	if (comp->IsShutdown())
		comp->Shutdown();
	mEngineComps.erase(std::find(mEngineComps.begin(), mEngineComps.end(), comp));
	delete comp;
	if (debug)
		*debug = failed;
}

/// -----------------------------------------------------------------
/// Adds logic component to object
/// -----------------------------------------------------------------
template<typename Type>
inline Type* Engine::Object::AddLogicComp(int* debug)
{
	//If it is not an logic component or already added, return
	LogicComp* comp;
	const int failed = GetLogicComp<Type>(comp);
	if (failed != 0)
	{
		//-1: Not LogicComp, 1: Already Added
		if (debug)
			*debug = failed;	
		return reinterpret_cast<Type*>(comp);
	}

	//If not, create and add
	comp = new Type;
	comp->SetOwner(this);
	mLogicComps.push_back(comp);

	//Added correctly
	if (debug)
		*debug = 0;		
	return reinterpret_cast<Type*>(comp);
}

/// -----------------------------------------------------------------
/// Gets logic component from object
/// Returns -1 if it is not an logic component
/// Returns 0 if it was not added to object
/// Returns 1 if success
/// -----------------------------------------------------------------
template<typename Type>
inline Type* Engine::Object::GetLogicComp(int* debug)
{
	//If it is not an engine component or already added, return
	Engine::TypeInfo* info = Type::TypeInfo();
	if (info == nullptr)
	{
		//Create type for the first time to be able to get type
		Type tmp;
		tmp.Initialize();
		info = Type::TYPE();
		tmp.Shutdown();
	}
	if (!info->HasParent(RTTI::TypeName<LogicComp>()))
	{
		if (debug)
			*debug = -1;
		return nullptr;
	}

	std::string typeName = info->GetTypeName();
	//Search for component. If found, set it
	for (auto component : mLogicComps)
	{
		if (strcmp(component->TypeInfo()->GetTypeName().c_str(), typeName.c_str()) == 0)
		{
			if (debug)
				*debug = 1;
			return reinterpret_cast<Type*>(component);
		}
	}

	//Not found
	if (debug)
		*debug = 0;
	return nullptr;
}

/// -----------------------------------------------------------------
/// Deletes logic component from object
/// -----------------------------------------------------------------
template<typename Type>
inline void Engine::Object::DeleteLogicComp(int* debug)
{
	//If it is not an logic component or not added, return
	int failed = 2;
	LogicComp* comp = GetLogicComp<Type>(&failed);
	if (failed <= 0)
	{
		if (debug)
			*debug = failed;
		return;
	}

	//Check that is shutdown and then delete it
	if (comp->IsShutdown())
		comp->Shutdown();
	mLogicComps.erase(std::find(mLogicComps.begin(), mLogicComps.end(), comp));
	delete comp;
	if (debug)
		*debug = failed;
}
