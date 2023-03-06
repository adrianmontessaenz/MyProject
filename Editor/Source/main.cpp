/*// -----------------------------------------------------------------
*  File:		main.cpp
*  Brief:		Main of editor
*  Creation:	11/12/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

#include "Editor/Editor.hpp"
#include <Graphics/GraphicsManager.hpp>
#include <Core/Time/TimeSystem.hpp>

int main(void)
{
	//Get all the singletons
	Engine::Window* window = gWindow;
	Editor::Editor* editor = gEditor;
	Engine::GraphicsManager* graphs = gGfxMgr;
	Engine::TimeSystem* time = gTimeSys;
	Engine::SDLEventSystem* SDLeventSys = gSDLSys;

	//Initialize
	window->Initialize();
	editor->Initialize();
	graphs->Initialize();
	time->Initialize();
	
	//Update
	while (window->IsActive())
	{
		SDLeventSys->Update();
		window->Update();
		time->Update();
		editor->Update();

		//Render
		graphs->Render();
		editor->Render();
		window->Render();
	}

	//Shutdown
	time->Shutdown();
	window->Shutdown();
	editor->Shutdown();
	return 0;
}