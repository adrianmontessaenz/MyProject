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

/// -----------------------------------------------------------------
/// Constructor to initialize all variables
/// -----------------------------------------------------------------
Engine::Window::Window() : mWindow(nullptr), mRenderer(nullptr), mDisplay(SDL_DisplayMode()), mSize(1280, 720), mPos(0,0), 
						   mMode(WindowMode::WINDOW_NORMAL), mDisplayIdx(0)
{
}

/// -----------------------------------------------------------------
/// Window creation and SDL initialization
/// -----------------------------------------------------------------
void Engine::Window::Initialize()
{
	//Initialize window. If error, throw
	Uint32 window_flags = SDL_WINDOW_RESIZABLE;
	SDL_Init(SDL_INIT_EVERYTHING);
	mWindow = SDL_CreateWindow("MyProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mSize.x, mSize.y, window_flags);
	if (mWindow == nullptr)
		throw "Could not create window %s\n", SDL_GetError();
	
	//Create renderer. If errow, throw
	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
	if (mRenderer == nullptr)
		throw "Could not create window %s\n", SDL_GetError();

	SDL_RenderSetVSync(mRenderer, 1);
	SDL_SetWindowMinimumSize(mWindow, 128, 78);

	//Get current display
	mDisplayIdx = SDL_GetWindowDisplayIndex(mWindow);
	SDL_GetDesktopDisplayMode(mDisplayIdx, &mDisplay);
}

/// -----------------------------------------------------------------
/// Window update
/// -----------------------------------------------------------------
void Engine::Window::Update()
{
	//Get window events and update
	auto vector = gSDLSys->GetEventsOfType(SDL_EventType::SDL_WINDOWEVENT);
	for (auto& we : vector)
	{
		switch (we.window.event)
		{
			case SDL_WINDOWEVENT_CLOSE:
				SetEnabled(false);
				return;
			case SDL_WINDOWEVENT_RESIZED:
				mSize.x = we.window.data1;
				mSize.y = we.window.data2;
				break;
			case SDL_WINDOWEVENT_MOVED:
				mPos.x = we.window.data1;
				mPos.y = we.window.data2;
				break;
			case SDL_WINDOWEVENT_DISPLAY_CHANGED:
				gDebugSys->Log("Window Changed Screen", "Window");
				mDisplayIdx = we.window.data1;
				SDL_GetCurrentDisplayMode(mDisplayIdx, &mDisplay);
			default:
				break;
		}
	}
}

/// -----------------------------------------------------------------
/// Window renderization
/// -----------------------------------------------------------------
void Engine::Window::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 40, 43, 200, 255);
	SDL_RenderClear(mRenderer);
	SDL_Rect rect;
	if (mMode == Engine::WindowMode::WINDOW_NORMAL)
	{
		rect.w = mSize.x;
		rect.h = mSize.y;
		rect.x = mPos.x;
		rect.y = mPos.y;
	}
	else
		SDL_GetDisplayBounds(mDisplayIdx, &rect);

	SDL_RenderFillRect(mRenderer, &rect);
	SDL_RenderPresent(mRenderer);
}

/// -----------------------------------------------------------------
/// Window shutdown
/// -----------------------------------------------------------------
void Engine::Window::Shutdown()
{
	//Destroy sdl window
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Sets window mode to fullscreen, borderless or normal
/// -----------------------------------------------------------------
void Engine::Window::SetWindowMode(Engine::WindowMode state)
{
	mMode = state;
	UpdateWindowMode();
}

/// -----------------------------------------------------------------
/// Gets window mode
/// -----------------------------------------------------------------
const Engine::WindowMode Engine::Window::GetWindowMode() const
{
	return mMode;
}

/// -----------------------------------------------------------------
/// Gets window size.
/// -----------------------------------------------------------------
const glm::vec<2, int> Engine::Window::GetSize() const
{
	return mSize;
}

/// -----------------------------------------------------------------
/// Sets window size
/// -----------------------------------------------------------------
void Engine::Window::SetSize(const glm::vec<2, int> new_size)
{
	mSize = new_size;
}

/// -----------------------------------------------------------------
/// Updates window based on its current mode
/// -----------------------------------------------------------------
void Engine::Window::UpdateWindowMode()
{
	switch (mMode)
	{
	case WindowMode::WINDOW_NORMAL:
		SDL_SetWindowFullscreen(mWindow, 0);
		break;
	case WindowMode::WINDOW_BORDERLESS:
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case WindowMode::WINDOW_FULLSCREEN:
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
		break;
	}
}
