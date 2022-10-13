/*// -----------------------------------------------------------------
*  File:		Window.cpp
*  Brief:		Implementation of Engine Window
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "Window.hpp"
#include <SDL.h>

Engine::Window::Window() : mWindow(nullptr), mWidth(1280), mHeight(720)
{
}

void Engine::Window::Initialize()
{
	//Initialize window
	Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	SDL_Init(SDL_INIT_VIDEO);
	mWindow = SDL_CreateWindow("MyProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, window_flags);

	//If error, throw
	if (mWindow == nullptr)
		throw "Could not create window %s\n", SDL_GetError();
}

void Engine::Window::Update()
{
	if (IsShutdown())
		return;

	SDL_Event window_event;
	while (SDL_PollEvent(&window_event))
	{
		switch (window_event.window.event)
		{

		}
	}
}

void Engine::Window::LateUpdate()
{
}

void Engine::Window::Shutdown()
{
	//Destroy sdl window
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	SetShutdown(true);
}
