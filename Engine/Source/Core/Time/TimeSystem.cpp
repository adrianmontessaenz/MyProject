/*// -----------------------------------------------------------------
*  File:		TimeSystem.cpp
*  Brief:		Implementation of TimeSystem class
*  Creation:	17/10/2022
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "TimeSystem.hpp"

/// -----------------------------------------------------------------
/// Initializes time system
/// -----------------------------------------------------------------
void Engine::TimeSystem::Initialize()
{
	//Start timer
	mStarted = true;
	mTicks = static_cast<Uint32>(SDL_GetPerformanceCounter());
}

/// -----------------------------------------------------------------
/// Updates time system
/// -----------------------------------------------------------------
void Engine::TimeSystem::Update()
{
	//Update time if not paused
	if (mPaused)
		return;
	mPrevTicks = mTicks;
	mTicks = static_cast<Uint32>(SDL_GetPerformanceCounter());
	mDT = static_cast<float>(mTicks - mPrevTicks) / static_cast<float>(SDL_GetPerformanceFrequency());
	std::cout << "DT: " << std::to_string(mDT) << std::endl;
	mFrameRate = 1 / mDT;
}

/// -----------------------------------------------------------------
/// Shutsdown time system
/// -----------------------------------------------------------------
void Engine::TimeSystem::Shutdown()
{
	mPaused = false;
	mStarted = false;
}

/// -----------------------------------------------------------------
/// Pauses time
/// -----------------------------------------------------------------
void Engine::TimeSystem::Pause()
{
	if (mPaused == false)
	{
		mPaused = true;
		mDT = ((static_cast<Uint32>(SDL_GetPerformanceCounter()) - mTicks) * 1000.f) / SDL_GetPerformanceFrequency();
		mTicks = 0;
	}
}

/// -----------------------------------------------------------------
/// Unpauses time
/// -----------------------------------------------------------------
void Engine::TimeSystem::UnPause()
{
	if (mPaused == true)
	{
		mPaused = false;
		mTicks = static_cast<Uint32>(SDL_GetPerformanceCounter());
	}
}

/// -----------------------------------------------------------------
/// Gets delta time in seconds
/// -----------------------------------------------------------------
float Engine::TimeSystem::GetDeltaTime()
{
	return mDT;
}

/// -----------------------------------------------------------------
/// Gets actual frame rate
/// -----------------------------------------------------------------
float Engine::TimeSystem::GetFrameRate()
{
	return mFrameRate;
}
