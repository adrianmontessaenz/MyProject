/*// -----------------------------------------------------------------
*  File:		RTTI.hpp
*  Brief:		Header for RTTI implementation
*  Creation:	21/10/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
namespace Engine
{
	class TypeInfo
	{
	public:
		~TypeInfo();
		//Settors
		void SetTypeName(const std::string name);
		void AddParentType(const std::string parent);

		//Gettors
		const std::string GetTypeName() const;
		const std::vector<std::string> GetParentNames() const;
		bool HasParent(const std::string name) const;

	private:
		const char* mTypeName = nullptr;
		std::vector<const char*> mParentNames;
	};

	class RTTI
	{
	public:
		template<typename Child> static void AddType();
		template<typename Child, typename Parent> static void AddParentedType();
		template<typename Type> static const TypeInfo* GetTypeInfo();
		template<typename Type>	static const std::string TypeName(); 
		static void FreeTypeInfos();

	private:
		static std::vector<TypeInfo*> sTypes;
		template<typename Type> static TypeInfo* FindType();
	};
}
#include "RTTI.inl"

//RTTI Base macro
#define RTTI_BASE(baseType)\
public:\
virtual const Engine::TypeInfo* TypeInfo()\
{return RTTI::GetTypeInfo<baseType>(); }\
static const Engine::TypeInfo* TYPE()\
{return RTTI::GetTypeInfo<baseType>(); }
