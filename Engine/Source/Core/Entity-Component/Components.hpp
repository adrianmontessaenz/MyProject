/*// -----------------------------------------------------------------
*  File:		Components.hpp
*  Brief:		Header for components (Both engine and logic)
*  Creation:	21/10/2022
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Object;
	class Component : public RunTime, public Serialized
	{
		RTTI_BASE(Component)
	public:
		virtual void Initialize() override = 0;
		virtual void Shutdown() override = 0;
		virtual ~Component() {};

		virtual void ToJson(nlohmann::ordered_json& data) = 0;
		virtual void FromJson(const nlohmann::ordered_json& data) = 0;

		void SetOwner(Object* owner);
		Object* GetOwner() const;

		virtual void SetActive(const bool& active) noexcept;
		virtual bool IsActive() const noexcept;
	
	private:
		Object* mOwner = nullptr;
		bool mActive = true;
	};

	class EngineComp : public Component
	{
		RTTI_BASE(EngineComp)
	public:
		virtual void Initialize() override = 0;
		virtual void Shutdown() override = 0;
		virtual void ToJson(nlohmann::ordered_json& data) = 0;
		virtual void FromJson(const nlohmann::ordered_json& data) = 0;
	};

	class LogicComp : public Component
	{
		RTTI_BASE(LogicComp)
	public:
		virtual void Initialize() override = 0;
		virtual void Shutdown() override = 0;
		virtual void ToJson(nlohmann::ordered_json& data) = 0;
		virtual void FromJson(const nlohmann::ordered_json& data) = 0;
	};
}