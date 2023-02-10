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