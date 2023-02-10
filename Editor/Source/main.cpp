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

int main()
{
	Engine::Window* window = gWindow;
	Editor::Editor* editor = gEditor;
	Engine::GraphicsManager* graphs = gGfxMgr;
	gWindow->Initialize();
	gEditor->Initialize();
	gGfxMgr->Initialize();
	gTimeSys->Initialize();
	
	while (gWindow->IsEnabled())
	{
		gSDLSys->Update();
		gWindow->Update();
		gTimeSys->Update();
		gEditor->Update();

		gGfxMgr->Render();
		gEditor->Render();
		gWindow->Render();
	}

	gTimeSys->Shutdown();
	gWindow->Shutdown();
	gEditor->Shutdown();
	return 0;
}