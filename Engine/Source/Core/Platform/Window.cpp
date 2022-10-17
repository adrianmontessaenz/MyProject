/*// -----------------------------------------------------------------
*  File:		Window.cpp
*  Brief:		Implementation of Engine Window
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Window.hpp"

Engine::Window::Window() : mWindow(nullptr), mRenderer(nullptr), mSize(1280, 720), mFullSize(0), mMode(WindowMode::WINDOW_NORMAL), mDisplayIdx(0)
{
}

void Engine::Window::Initialize()
{
	//Initialize window
	Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	SDL_Init(SDL_INIT_EVERYTHING);
	mWindow = SDL_CreateWindow("MyProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mSize.x, mSize.y, window_flags);
	//If error, throw
	if (mWindow == nullptr)
		throw "Could not create window %s\n", SDL_GetError();
	
	mRenderer = SDL_CreateRenderer(mWindow, 0, 0);
	//If error, throw
	if (mRenderer == nullptr)
		throw "Could not create window %s\n", SDL_GetError();

	SDL_RenderSetVSync(mRenderer, 1);
	SDL_SetWindowMinimumSize(mWindow, 128, 78);
}

void Engine::Window::Update()
{
	auto vector = gSDLSys->GetEventsOfType(SDL_EventType::SDL_WINDOWEVENT);
	for (auto& we : vector)
	{
		switch (we.window.event)
		{
			case SDL_WINDOWEVENT_CLOSE:
				SetEnabled(false);
				return;

			default:
				break;
		}
	}
	
	if (mMode == Engine::WindowMode::WINDOW_NORMAL)
	{
		SDL_GetWindowSize(mWindow, &mSize.x, &mSize.y);
		mDisplayIdx = SDL_GetWindowDisplayIndex(mWindow);
	}
	SDL_DisplayMode dm;
	SDL_GetDesktopDisplayMode(mDisplayIdx, &dm);
	mFullSize = { dm.w, dm.h };
}

void Engine::Window::Render()
{
	int windowIdx = SDL_GetWindowDisplayIndex(mWindow);
	SDL_SetRenderDrawColor(mRenderer, 40, 43, 200, 255);
	SDL_RenderClear(mRenderer);
	SDL_RenderPresent(mRenderer);
}

void Engine::Window::Shutdown()
{
	//Destroy sdl window
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
	SetShutdown(true);
}

void Engine::Window::SetWindowMode(Engine::WindowMode state)
{
	mMode = state;
	UpdateWindowMode();
}

const Engine::WindowMode Engine::Window::GetWindowMode() const
{
	return mMode;
}

const glm::vec<2, int> Engine::Window::GetSize() const
{
	return mSize;
}

void Engine::Window::SetSize(const glm::vec<2, int> new_size)
{
	mSize = new_size;
}

void Engine::Window::UpdateWindowMode()
{
	int x = 0, y = 0;
	SDL_GetWindowSize(mWindow, &x, &y);

	switch (mMode)
	{
	case WindowMode::WINDOW_NORMAL:
		SDL_SetWindowFullscreen(mWindow, 0);
		if(x != mSize.x || y != mSize.y)
			SDL_SetWindowSize(mWindow, mSize.x, mSize.y);
		SDL_SetWindowBordered(mWindow, SDL_TRUE);
		break;
	case WindowMode::WINDOW_BORDERLESS:
		SDL_SetWindowBordered(mWindow, SDL_FALSE);
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case WindowMode::WINDOW_FULLSCREEN:
		SDL_SetWindowBordered(mWindow, SDL_FALSE);
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
		if (x != mFullSize.x || y != mFullSize.y)
			SDL_SetWindowSize(mWindow, mFullSize.x, mFullSize.y);
		break;
	default:
		SDL_SetWindowBordered(mWindow, SDL_TRUE);
		SDL_SetWindowFullscreen(mWindow, 0);
		break;
	}
}
