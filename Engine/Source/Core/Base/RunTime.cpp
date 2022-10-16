/*// -----------------------------------------------------------------
*  File:		RunTime.cpp
*  Brief:		Declaration of runtime functions
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "RunTime.hpp"

void Engine::RunTime::Update()
{
}

void Engine::RunTime::LogicUpdate()
{
}

void Engine::RunTime::Render()
{
}

void Engine::RunTime::SetShutdown(const bool shutdown)
{
	mIsShutdown = shutdown;
}

bool Engine::RunTime::IsShutdown() const
{
	return mIsShutdown;
}

void Engine::RunTime::SetEnabled(const bool enable)
{
	mIsEnabled = enable;
}

bool Engine::RunTime::IsEnabled() const
{
	return mIsEnabled;
}
