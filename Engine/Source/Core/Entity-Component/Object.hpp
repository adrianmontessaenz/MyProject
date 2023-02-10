/*// -----------------------------------------------------------------
*  File:		Object.hpp
*  Brief:		Header of Object class
*  Creation:	21/10/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Core/Scene/Space.hpp>

namespace Engine
{
	class Object : public RunTime, public Identity
	{
		RTTI_BASE(Object)
	public:
		~Object();

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
		std::vector<EngineComp*> GetEngineComps() const;
		template<typename Type>
		void DeleteEngineComp(int* debug = nullptr);

		//Logic Components
		template<typename Type>
		Type* AddLogicComp(int* debug = nullptr);
		template<typename Type>
		Type* GetLogicComp(int* debug = nullptr);
		template<typename Type>
		void DeleteLogicComp(int* debug = nullptr);

		//Hierarchy
		void AddChild(Object* child);
		void RemoveChild(Object* child);
		Object* GetChildByName(const std::string name) const;
		std::vector<Object*> GetChildrenByName(const std::string name) const;
		const std::vector<Object*> GetChildren() const;
		void SwapChildren(const size_t& l_idx, const size_t& r_idx);

		void SetParent(Object* parent);
		Object* GetParent() const;
		void SetSpace(Space* space_);
		Space* GetSpace() const;

	private:
		std::vector<EngineComp*> mEngineComps;
		std::vector<LogicComp*> mLogicComps;

		Object* mParent = nullptr;
		std::vector<Object*> mChildren;
		Space* mSpace = nullptr;
	};
}
#include "Object.inl"