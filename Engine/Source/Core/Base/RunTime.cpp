/*// -----------------------------------------------------------------
*  File:		RunTime.cpp
*  Brief:		Declaration of runtime functions
*  Creation:	13/10/2022
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "RunTime.hpp"

/// -----------------------------------------------------------------
/// Empty Update
/// -----------------------------------------------------------------
void Engine::RunTime::Update(){}

/// -----------------------------------------------------------------
/// Sets shutdown state
/// -----------------------------------------------------------------
void Engine::RunTime::SetShutdown(const bool shutdown)
{
	mIsShutdown = shutdown;
}

/// -----------------------------------------------------------------
/// Gets shutdown state
/// -----------------------------------------------------------------
bool Engine::RunTime::IsShutdown() const
{
	return mIsShutdown;
}
