/*// -----------------------------------------------------------------
*  File:		Object.hpp
*  Brief:		Header of Object class
*  Creation:	21/10/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
namespace Engine
{
	class Object : public RunTime, public Identity
	{
		RTTI_BASE(Object)
	public:
		//Runtime
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LogicUpdate() override;
		virtual void Shutdown() override;

		//Engine Components
		template<typename Type>
		Type* AddEngineComp(int* debug = nullptr);
		template<typename Type>
		Type* GetEngineComp(int* debug = nullptr);

		//Logic Components
		template<typename Type>
		Type* AddLogicComp(int* debug = nullptr);
		template<typename Type>
		Type* GetLogicComp(int* debug = nullptr);

		//Hierarchy
		void AddChildren(Object* child);
		Object* GetChildByName(const std::string name) const;
		std::vector<Object*> GetChildrenByName(const std::string name) const;
		const std::vector<Object*> GetChildren() const;

		void SetParent(Object* parent);
		Object* GetParent() const;
	private:
		std::vector<EngineComp*> mEngineComps;
		std::vector<LogicComp*> mLogicComps;

		Object* mParent = nullptr;
		std::vector<Object*> mChildren;
	};
}
#include "Object.inl"