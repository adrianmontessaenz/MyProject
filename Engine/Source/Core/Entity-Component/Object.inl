/*// -----------------------------------------------------------------
*  File:		Object.inl
*  Brief:		Implementation of templated Object class functions
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

template<typename Type>
inline const int Engine::Object::AddEngineComp()
{
	//If it is not an engine component or already added, return
	EngineComp* comp;
	const int failed = GetEngineComp<Type>(comp);
	if (failed != 0)
		return failed;		//-1: Not EngineComp, 1: Already Added

	//If not, create and add
	Type* comp = new Type;
	comp->SetOwner(this);
	mEngineComps.push_back(comp);
	return 0;						//Added correctly
}

template<typename Type>
inline const int Engine::Object::GetEngineComp(Engine::EngineComp* comp)
{
	//If it is not an engine component or already added, return
	Engine::RTTI* info = Type::TYPE();
	if (info->HasParent(RTTI::TypeName<EngineComp>()))
		return -1;
	std::string typeName = info->GetTypeName();

	//Search for component. If found, set it
	for (auto component : mEngineComps)
	{
		if (strcmp(component->TypeInfo()->GetTypeName().c_str(), typeName.c_str()) == 0)
		{
			comp = component;
			return 1;
		}
	}

	//Not found
	return 0;
}

template<typename Type>
inline const int Engine::Object::AddLogicComp()
{
	//If it is not an logic component or already added, return
	LogicComp* comp;
	const int failed = GetLogicComp<Type>(comp);
	if (failed != 0)
		return failed;		//-1: Not LogicComp, 1: Already Added

	//If not, create and add
	Type* comp = new Type;
	comp->SetOwner(this);
	mLogicComps.push_back(comp);
	return 0;						//Added correctly
}

template<typename Type>
inline const int Engine::Object::GetLogicComp(Engine::LogicComp* comp)
{
	//If it is not an engine component or already added, return
	Engine::RTTI* info = Type::TypeInfo();
	if (info->HasParent(RTTI::TypeName<LogicComp>()))
		return -1;

	std::string typeName = info->GetTypeName();
	//Search for component. If found, set it
	for (auto component : mLogicComps)
	{
		if (strcmp(component->TypeInfo()->GetTypeName().c_str(), typeName.c_str()) == 0)
		{
			comp = component;
			return 1;
		}
	}

	//Not found
	return 0;
}