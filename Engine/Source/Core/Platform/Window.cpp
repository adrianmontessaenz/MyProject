/*// -----------------------------------------------------------------
*  File:		Window.cpp
*  Brief:		Implementation of Engine Window
*  Creation:	13/10/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Window.hpp"

/// -----------------------------------------------------------------
/// Window creation and SDL initialization
/// -----------------------------------------------------------------
void Engine::Window::Initialize()
{
	//Initialize window. If error, throw
	Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mWindow = SDL_CreateWindow("MyProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mSize.x, mSize.y, window_flags);
	if (mWindow == nullptr)
		throw "Could not create window %s\n", SDL_GetError();
	
	//Create renderer. If errow, throw
	mContext = SDL_GL_CreateContext(mWindow);
	SDL_GL_MakeCurrent(mWindow, mContext);
	SDL_SetWindowMinimumSize(mWindow, 128, 78);

	//Get current display
	mDisplayIdx = SDL_GetWindowDisplayIndex(mWindow);
	SDL_GetDesktopDisplayMode(mDisplayIdx, &mDisplay);

	//Set up initial openGL
	gladLoadGL();
	glViewport(0, 0, mSize.x, mSize.y);
	glClearColor(0.07f, 0.13f, 0.17f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
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
				break;
		}
	}
}

/// -----------------------------------------------------------------
/// Window renderization
/// -----------------------------------------------------------------
void Engine::Window::Render()
{
	SDL_GL_SwapWindow(mWindow);
}

/// -----------------------------------------------------------------
/// Window shutdown
/// -----------------------------------------------------------------
void Engine::Window::Shutdown()
{
	//Destroy sdl window
	SDL_DestroyWindow(mWindow);
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
/// Gets SDL window
/// -----------------------------------------------------------------
SDL_Window* Engine::Window::GetWindow()
{
	return mWindow;
}

/// -----------------------------------------------------------------
/// Gets SDL context of OpenGL
/// -----------------------------------------------------------------
SDL_GLContext& Engine::Window::GetContext()
{
	return mContext;
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
