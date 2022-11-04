/*// -----------------------------------------------------------------
*  File:		TimeSystem.hpp
*  Brief:		Header of TimeSystem class
*  Creation:	17/10/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class TimeSystem : public RunTime, public Singleton<TimeSystem>
	{
	public:
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
		Uint32 mPrevTicks = 0;
		Uint32 mTicks = 0;
		Uint32 mPausedTicks = 0;
		bool mStarted = false;
		bool mPaused = false;

		float mFrameRate = 0.f;
		float mFrameCounter = 0.f;
	};
}
#define gTimeSys (&Engine::TimeSystem::GetInstance())