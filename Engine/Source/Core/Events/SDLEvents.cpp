#include "SDLEvents.hpp"

void Engine::SDLEventSystem::Initialize()
{
}

void Engine::SDLEventSystem::Update()
{
	//Get all events
	SDL_Event sdl_event;
	while (SDL_PollEvent(&sdl_event))
		mEvents[sdl_event.type].push_back(sdl_event);
}

void Engine::SDLEventSystem::LateUpdate()
{
	mEvents.clear();
}

void Engine::SDLEventSystem::Shutdown()
{
}

const std::vector<SDL_Event>& Engine::SDLEventSystem::GetEventsOfType(Uint32 type)
{
	return mEvents[type];
}
