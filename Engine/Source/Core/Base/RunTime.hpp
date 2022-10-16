/*// -----------------------------------------------------------------
*  File:		RunTime.hpp
*  Brief:		Header of runtime functions
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class RunTime
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update();
		virtual void LogicUpdate();
		virtual void Render();
		virtual void Shutdown() = 0;

		void SetShutdown(const bool shutdown);
		bool IsShutdown() const;

		void SetEnabled(const bool enable);
		bool IsEnabled() const;
	protected:
		RunTime() = default;

	private:
		bool mIsEnabled = true;
		bool mIsShutdown = false;
	};
}