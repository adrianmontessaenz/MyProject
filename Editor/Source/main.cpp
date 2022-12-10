#include "Editor/Editor.hpp"
#include <Graphics/GraphicsManager.hpp>

int main()
{
	Engine::Window* window = gWindow;
	Editor::Editor* editor = gEditor;
	Engine::GraphicsManager* graphs = gGfxMgr;
	gWindow->Initialize();
	gEditor->Initialize();
	gGfxMgr->Initialize();
	

	while (gWindow->IsEnabled())
	{
		gSDLSys->Update();
		gWindow->Update();
		gEditor->Update();

		gGfxMgr->Render();
		gEditor->Render();
		gWindow->Render();
	}

	gWindow->Shutdown();
	gEditor->Shutdown();
	return 0;
}