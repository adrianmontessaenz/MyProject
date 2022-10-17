/*// -----------------------------------------------------------------
*  File:		TimeSystem.cpp
*  Brief:		Implementation of TimeSystem class
*  Creation:	17/10/2022
*  Last Update:	17/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "TimeSystem.hpp"

Engine::TimeSystem::TimeSystem() : mPrevTicks(0), mTicks(0), mPausedTicks(0), mStarted(false), mPaused(false), mFrameCounter(0), mFrameRate(0)
{
}

void Engine::TimeSystem::Initialize()
{
	//Start timer
	mStarted = true;
	mTicks = SDL_GetTicks();
}

void Engine::TimeSystem::Update()
{
	//Update time if not paused
	if (mPaused)
		return;
	mPrevTicks = mTicks;
	mTicks = SDL_GetTicks();

	//Update frame rate
	mFrameCounter += static_cast<float>(mTicks - mPrevTicks) / 1000.f;
	if (mFrameCounter >= 1.f)
	{
		mFrameRate = (mFrameCounter * 1000.f) / static_cast<float>(mTicks - mPrevTicks);
		mFrameCounter = 0;
	}
}

void Engine::TimeSystem::Shutdown()
{
	mPaused = false;
	mStarted = false;
}

void Engine::TimeSystem::Pause()
{
	if (mPaused == false)
	{
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mTicks;
		mTicks = 0;
	}
}

void Engine::TimeSystem::UnPause()
{
	if (mPaused == true)
	{
		mPaused = false;
		mTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

float Engine::TimeSystem::GetDeltaTime()
{
	float toSeconds = 1 / 1000.f;
	if (mPaused)
		return (mPausedTicks * toSeconds);
	return ((mTicks - mPrevTicks) * toSeconds);
}

float Engine::TimeSystem::GetFrameRate()
{
	return mFrameRate;
}
