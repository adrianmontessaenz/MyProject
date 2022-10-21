/*// -----------------------------------------------------------------
*  File:		Components.hpp
*  Brief:		Header for components (Both engine and logic)
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Object;
	class Component : public RunTime
	{
	public:
		virtual void Initialize() override = 0;
		virtual void Shutdown() override = 0;

		void SetOwner(Object* owner);
		const Object* GetOwner() const;
	private:
		Object* mOwner;
	};

	class EngineComp : public Component
	{
	public:
		virtual void Initialize() override = 0;
		virtual void Shutdown() override = 0;
	};

	class LogicComp : public Component
	{
	public:
		virtual void Initialize() override = 0;
		virtual void Shutdown() override = 0;
	};
}