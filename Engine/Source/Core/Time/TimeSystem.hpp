/*// -----------------------------------------------------------------
*  File:		TimeSystem.hpp
*  Brief:		Header of TimeSystem class
*  Creation:	17/10/2022
*  Last Update:	17/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class TimeSystem : public RunTime, public Singleton<TimeSystem>
	{
	public:
		TimeSystem();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Shutdown() override;

		//Extra functions
		void Pause();
		void UnPause();

		//Get delta time
		float GetDeltaTime();
		float GetFrameRate();

	private:
		//Timers
		Uint32 mPrevTicks;
		Uint32 mTicks;
		Uint32 mPausedTicks;
		bool mStarted;
		bool mPaused;

		float mFrameRate;
		float mFrameCounter;
	};
}
#define gTimeSys (&Engine::TimeSystem::GetInstance())