/*// -----------------------------------------------------------------
*  File:		RTTI.inl
*  Brief:		Implementation for templated RTTI
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
template<typename Child>
inline void Engine::RTTI::AddType()
{
	TypeInfo* type = FindType<Child>();
	if (type)
		return;

	type = new TypeInfo;
	type->SetTypeName(TypeName<Child>());
	sTypes.push_back(type);
}

template<typename Child, typename Parent>
inline void Engine::RTTI::AddParentedType()
{
	TypeInfo* type = FindType<Child>();
	if (type)
	{
		type->AddParentType(TypeName<Parent>());
		return;
	}

	type = new TypeInfo;
	type->SetTypeName(TypeName<Child>());
	type->AddParentType(TypeName<Parent>());
	sTypes.push_back(type);
}

template<typename Type>
inline const Engine::TypeInfo* Engine::RTTI::GetTypeInfo()
{
	return FindType<Type>();
}

template<typename Type>
inline const std::string Engine::RTTI::TypeName()
{
	//Get name and offset until type name
	std::string name = typeid(Type).name();
	std::size_t offset = name.find_last_of(':');

	//If it is on a namespace, offset string
	if (offset != std::string::npos)
		name = name.substr(offset + 1);

	return name;
}

template<typename Type>
inline Engine::TypeInfo* Engine::RTTI::FindType()
{
	//Find
	for (auto types : sTypes)
	{
		if (strcmp(types->GetTypeName().c_str(), TypeName<Type>().c_str()) == 0)
			return types;
	}

	//Not found
	return nullptr;
}
