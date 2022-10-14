/*// -----------------------------------------------------------------
*  File:		Window.cpp
*  Brief:		Implementation of Engine Window
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <Core/Events/SDLEvents.hpp>
#include "Window.hpp"
#include <SDL.h>

Engine::Window::Window() : mWindow(nullptr), mWidth(1280), mHeight(720), mMode(WindowMode::WINDOW_NORMAL)
{
}

void Engine::Window::Initialize()
{
	//Initialize window
	Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	SDL_Init(SDL_INIT_EVERYTHING);
	mWindow = SDL_CreateWindow("MyProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, window_flags);
	//If error, throw
	if (mWindow == nullptr)
		throw "Could not create window %s\n", SDL_GetError();
	
	mRenderer = SDL_CreateRenderer(mWindow, 0, 0);
	//If error, throw
	if (mRenderer == nullptr)
		throw "Could not create window %s\n", SDL_GetError();

	SDL_SetWindowMinimumSize(mWindow, 128, 78);
}

void Engine::Window::Update()
{
	if (IsShutdown())
		return;
	gSDLSys->Update();
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
	int x = 0, y = 0;
	SDL_GetWindowSize(mWindow, &x, &y);
	mWidth = x;
	mHeight = y;

	switch (mMode)
	{
	case WindowMode::WINDOW_NORMAL:
		SDL_SetWindowBordered(mWindow, SDL_TRUE);
		SDL_SetWindowFullscreen(mWindow, 0);
		break;
	case WindowMode::WINDOW_BORDERLESS:
		SDL_SetWindowBordered(mWindow, SDL_FALSE);
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case WindowMode::WINDOW_FULLSCREEN:
		SDL_SetWindowBordered(mWindow, SDL_FALSE);
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
		break;
	default:
		SDL_SetWindowBordered(mWindow, SDL_TRUE);
		SDL_SetWindowFullscreen(mWindow, 0);
		break;
	}
}

void Engine::Window::LateUpdate()
{
	SDL_SetRenderDrawColor(mRenderer, 40, 43, 200, 255);
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = mWidth;
	rect.h = mHeight;
	SDL_RenderFillRect(mRenderer, &rect);
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
}

const Engine::WindowMode Engine::Window::GetWindowMode() const
{
	return mMode;
}
