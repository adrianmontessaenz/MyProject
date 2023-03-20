/*// -----------------------------------------------------------------
*  File:		main.cpp
*  Brief:		Main of editor
*  Creation:	11/12/2022
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

#include "Editor/Editor.hpp"
#include "Editor/EditorRenderer.hpp"
#include <Graphics/GraphicsManager.hpp>
#include <Core/Time/TimeSystem.hpp>
#include <Core/Platform/InputManager.hpp>
#include <Physics/PhysicsManager.hpp>

int main(void)
{
	//Get all the singletons
	Engine::Window* window = gWindow;
	Editor::EditorClass* editor = gEditor;
	Editor::RenderEditor* renderEditor = gRenderEditor;
	Engine::GraphicsManager* graphs = gGfxMgr;
	Engine::TimeSystem* time = gTimeSys;
	Engine::SDLEventSystem* SDLeventSys = gSDLSys;
	Engine::ObjectManager* objMgr = gObjMgr;
	Engine::InputManager* input = gInputMgr;
	Engine::PhysicsManager* physics = gPhysics;

	//Initialize
	window->Initialize();
	editor->Initialize();
	objMgr->Initialize();
	renderEditor->Initialize();
	graphs->Initialize();
	physics->Initialize();
	time->Initialize();
	input->Initialize();
	
	//Update
	while (window->IsActive())
	{
		//Normal updates
		SDLeventSys->Update();
		window->Update();
		time->Update();
		editor->Update();
		input->Update();

		//Update editor render if not playing
		if (editor->IsPlaying() == false)
		{
			renderEditor->Update();
			renderEditor->Render();
		}
		//If playing, update normally
		else
		{
			objMgr->Update();
			physics->Update();
			renderEditor->Update();
			renderEditor->Render();
			//graphs->Render();
		}
		editor->Render();
		window->Render();
	}

	//Shutdown
	time->Shutdown();
	objMgr->Shutdown();
	graphs->Shutdown();
	physics->Shutdown();
	renderEditor->Shutdown();
	editor->Shutdown();
	window->Shutdown();
	return 0;
}