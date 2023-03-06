/*// -----------------------------------------------------------------
*  File:		InputManager.cpp
*  Brief:		Implementation of Engine Input Manager
*  Creation:	16/10/2022
*  Last Update:	06/03/2023
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
	auto keysDown = gSDLSys->GetEventsOfType(SDL_KEYDOWN);
	auto keysUp = gSDLSys->GetEventsOfType(SDL_KEYUP);

	//Update keydown events (change with timer events)
	for (const auto& k : keysDown)
	{
		if (mKeyboard[k.key.keysym.scancode] == 0.f)
			mKeyboard[k.key.keysym.scancode] = gTimeSys->GetDeltaTime();
		else
			mKeyboard[k.key.keysym.scancode] += gTimeSys->GetDeltaTime();
	}

	//Update key up keys
	for (const auto& k : keysUp)
	{
		if (k.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
			int a = 0;
		mKeyboard[k.key.keysym.scancode] = 0.f;
	}

	//Get mouse events and update
	auto mouseDown = gSDLSys->GetEventsOfType(SDL_MOUSEBUTTONDOWN);
	auto mouseUp = gSDLSys->GetEventsOfType(SDL_MOUSEBUTTONUP);
	SDL_GetMouseState(&mMousePos.x, &mMousePos.y);

	//Update keydown events (change with timer events)
	for (const auto& m : mouseDown)
	{
		if (mMouse[m.button.button] == 0.f)
			mMouse[m.button.button] = gTimeSys->GetDeltaTime();
		else
			mMouse[m.button.button] += gTimeSys->GetDeltaTime();
	}

	//Update key up keys
	for (const auto& m : mouseUp)
		mMouse[m.button.button] = 0.f;
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
