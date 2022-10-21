/*// -----------------------------------------------------------------
*  File:		main.cpp
*  Brief:		Main of engine (temporal)
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
* 
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include <Core/Platform/Window.hpp>
#include <Core/Platform/InputManager.hpp>
#include <Core/Time/TimeSystem.hpp>
#include <iostream>

int main(void)
{
	gWindow->Initialize();
	gTimeSys->Initialize();
	while (gWindow->IsEnabled())
	{
		//Update
		gSDLSys->Update();
		gWindow->Update();
		gInputMgr->Update();

		//Logic Update
		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_F11))
		{
			if (gWindow->GetWindowMode() == Engine::WindowMode::WINDOW_FULLSCREEN)
				gWindow->SetWindowMode(Engine::WindowMode::WINDOW_NORMAL);
			else
				gWindow->SetWindowMode(Engine::WindowMode::WINDOW_FULLSCREEN);
		}
		else if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_ESCAPE))
			gWindow->SetEnabled(false);

		if (gInputMgr->IsKeyTriggered(SDL_SCANCODE_A))
			gDebugSys->Log("A key is triggered");
		else if (gInputMgr->IsKeyPressed(SDL_SCANCODE_A))
			gDebugSys->Log("A key is pressed");
		else if (gInputMgr->IsKeyReleased(SDL_SCANCODE_A))
			gDebugSys->Log("A key is released");

		//Render
		/*std::cout << "Delta Time (Seconds): " << gTimeSys->GetDeltaTime() << std::endl;*/
		//std::cout << "FrameRate: " << gTimeSys->GetFrameRate() << "/ms" << std::endl;
		gWindow->Render();
		gTimeSys->Update();
	}

	gWindow->Shutdown();
	return 0;
}