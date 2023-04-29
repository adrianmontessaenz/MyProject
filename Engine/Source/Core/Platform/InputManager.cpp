/*// -----------------------------------------------------------------
*  File:		InputManager.cpp
*  Brief:		Implementation of Engine Input Manager
*  Creation:	16/10/2022
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "InputManager.hpp"
#include <Core/Time/TimeSystem.hpp>

/// -----------------------------------------------------------------
/// Empty initialization
/// -----------------------------------------------------------------
void Engine::InputManager::Initialize(){}

/// -----------------------------------------------------------------
/// Input update. Checks keys to be updated
/// -----------------------------------------------------------------
void Engine::InputManager::Update()
{
	//Update previous state
	mPrevKeyboard = mKeyboard;
	mPrevMouse = mMouse;

	//Get new events
	const std::vector<SDL_Event>& keysDown = gSDLSys->GetEventsOfType(SDL_KEYDOWN);
	const std::vector<SDL_Event>& keysUp = gSDLSys->GetEventsOfType(SDL_KEYUP);
	size_t keyDownCount = keysDown.size();
	size_t keyUpCount = keysUp.size();

	//Update keydown events (change with timer events)
	SDL_Scancode keyCode = SDL_SCANCODE_0;
	for (size_t idx = 0; idx < keyDownCount; idx++)
	{
		keyCode = keysDown[idx].key.keysym.scancode;
		if (mKeyboard[keyCode] == 0.f)
			mKeyboard[keyCode] = gTimeSys->GetDeltaTime();
		else
			mKeyboard[keyCode] += gTimeSys->GetDeltaTime();
	}

	//Update key up keys
	for (size_t idx = 0; idx < keyUpCount; idx++)
		mKeyboard[keysUp[idx].key.keysym.scancode] = 0.f;

	//Get mouse events and update
	const std::vector<SDL_Event>& mouseDown = gSDLSys->GetEventsOfType(SDL_MOUSEBUTTONDOWN);
	const std::vector<SDL_Event>& mouseUp = gSDLSys->GetEventsOfType(SDL_MOUSEBUTTONUP);
	size_t mouseDownCount = mouseDown.size();
	size_t mouseUpCount = mouseUp.size();
	SDL_GetMouseState(&mMousePos.x, &mMousePos.y);

	//Update keydown events (change with timer events)
	Uint8 mouseButton = 0;
	for (size_t idx = 0; idx < mouseDownCount; idx++)
	{
		mouseButton = mouseDown[idx].button.button;
		if (mMouse[mouseButton] == 0.f)
			mMouse[mouseButton] = gTimeSys->GetDeltaTime();
		else
			mMouse[mouseButton] += gTimeSys->GetDeltaTime();
	}

	//Update key up keys
	for (size_t idx = 0; idx < mouseUpCount; idx++)
	{
		mouseButton = mouseUp[idx].button.button;
		mMouse[mouseButton] = 0.f;
	}
}

/// -----------------------------------------------------------------
/// Empty shutdown
/// -----------------------------------------------------------------
void Engine::InputManager::Shutdown()
{
	mMouse.clear();
	mPrevMouse.clear();
	mKeyboard.clear();
	mPrevKeyboard.clear();
}

/// -----------------------------------------------------------------
/// Checks if key was triggered
/// -----------------------------------------------------------------
bool Engine::InputManager::IsKeyTriggered(SDL_Scancode key)
{
	return mKeyboard[key] > 0.f && mPrevKeyboard[key] <= 0.f;
}

/// -----------------------------------------------------------------
/// Checks if key was pressed
/// -----------------------------------------------------------------
bool Engine::InputManager::IsKeyPressed(SDL_Scancode key)
{
	return mKeyboard[key] > gTimeSys->GetDeltaTime() && mPrevKeyboard[key] > 0.f;
}

/// -----------------------------------------------------------------
/// Checks if key was released
/// -----------------------------------------------------------------
bool Engine::InputManager::IsKeyReleased(SDL_Scancode key)
{
	return mKeyboard[key] <= 0.f && mPrevKeyboard[key] > 0.f;
}

/// -----------------------------------------------------------------
/// Checks if mouse was triggered
/// -----------------------------------------------------------------
bool Engine::InputManager::IsMouseTriggered(Uint8 mouse)
{
	return mPrevMouse[mouse] <= 0.f && mMouse[mouse] > 0.f;
}

/// -----------------------------------------------------------------
/// Checks if mouse was pressed
/// -----------------------------------------------------------------
bool Engine::InputManager::IsMousePressed(Uint8 mouse)
{
	return mMouse[mouse] > gTimeSys->GetDeltaTime() && mMouse[mouse] > 0.f;
}

/// -----------------------------------------------------------------
/// Checks if mouse was released
/// -----------------------------------------------------------------
bool Engine::InputManager::IsMouseReleased(Uint8 mouse)
{
	return mMouse[mouse] <= 0.f && mPrevMouse[mouse] > 0.f;
}

/// -----------------------------------------------------------------
/// Gets mouse position
/// -----------------------------------------------------------------
const glm::vec<2, int> Engine::InputManager::GetMousePos() const
{
	return mMousePos;
}
