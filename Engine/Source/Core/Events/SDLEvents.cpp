/*// -----------------------------------------------------------------
*  File:		SDLEvents.cpp
*  Brief:		Implementation of sdlevents gatherer
*  Creation:	14/10/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "SDLEvents.hpp"

/// -----------------------------------------------------------------
/// Empty Initialization
/// -----------------------------------------------------------------
void Engine::SDLEventSystem::Initialize(){}

/// -----------------------------------------------------------------
/// Gets all the events that happened in frame and stores them
/// -----------------------------------------------------------------
void Engine::SDLEventSystem::Update()
{
	//Get all events
	mEvents.clear();
	SDL_Event sdl_event;
	while (SDL_PollEvent(&sdl_event))
		mEvents[sdl_event.type].push_back(sdl_event);
}

/// -----------------------------------------------------------------
/// Empty shutdown
/// -----------------------------------------------------------------
void Engine::SDLEventSystem::Shutdown()
{
	mEvents.clear();
}

/// -----------------------------------------------------------------
/// Gets requested events
/// -----------------------------------------------------------------
const std::vector<SDL_Event>& Engine::SDLEventSystem::GetEventsOfType(Uint32 type)
{
	return mEvents[type];
}

/// -----------------------------------------------------------------
/// Gets all events
/// -----------------------------------------------------------------
const std::map<Uint32, std::vector<SDL_Event>>& Engine::SDLEventSystem::GetEvents()
{
	return mEvents;
}
