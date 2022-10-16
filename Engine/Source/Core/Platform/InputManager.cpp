/*// -----------------------------------------------------------------
*  File:		InputManager.cpp
*  Brief:		Implementation of Engine Input Manager
*  Creation:	16/10/2022
*  Last Update:	16/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "InputManager.hpp"

void Engine::InputManager::Initialize()
{
}

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
			mKeyboard[k.key.keysym.scancode] = 0.016f;
		else
			mKeyboard[k.key.keysym.scancode] += 0.016f;
	}

	//Update key up keys
	for (const auto& k : keysUp)
		mKeyboard[k.key.keysym.scancode] = 0.f;

	//Get mouse events and update
	auto mouseDown = gSDLSys->GetEventsOfType(SDL_MOUSEBUTTONDOWN);
	auto mouseUp = gSDLSys->GetEventsOfType(SDL_MOUSEBUTTONDOWN);
	SDL_GetMouseState(&mMousePos.x, &mMousePos.y);

	//Update keydown events (change with timer events)
	for (const auto& m : mouseDown)
	{
		if (mMouse[m.button.button] == 0.f)
			mMouse[m.button.button] = 0.016f;
		else
			mMouse[m.button.button] += 0.016f;
	}

	//Update key up keys
	for (const auto& m : mouseUp)
		mMouse[m.button.button] = 0.f;
}

void Engine::InputManager::Shutdown()
{
}

bool Engine::InputManager::IsKeyTriggered(SDL_Scancode key)
{
	return mKeyboard[key] > 0.f && mPrevKeyboard[key] <= 0.f;
}

bool Engine::InputManager::IsKeyPressed(SDL_Scancode key)
{
	return mKeyboard[key] > 1.f;
}

bool Engine::InputManager::IsKeyReleased(SDL_Scancode key)
{
	return mKeyboard[key] <= 0.f && mPrevKeyboard[key] > 0.f;
}

bool Engine::InputManager::IsMouseTriggered(Uint8 mouse)
{
	return mPrevMouse[mouse] <= 0.f && mMouse[mouse] > 0.f;
}

bool Engine::InputManager::IsMousePressed(Uint8 mouse)
{
	return mMouse[mouse] > 1.f;
}

bool Engine::InputManager::IsMouseReleased(Uint8 mouse)
{
	return mMouse[mouse] <= 0.f && mPrevMouse[mouse] > 0.f;
}

const glm::vec<2, int> Engine::InputManager::GetMousePos() const
{
	return mMousePos;
}
