/*// -----------------------------------------------------------------
*  File:		Editor.cpp
*  Brief:		Main part of the editor.
*  Creation:	17/11/2022
*  Last Update:	10/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "Editor.hpp"
#include <Core/Scene/Space.hpp>
#include <Core/Scene/ObjectManager.hpp>

/// -----------------------------------------------------------------
/// Editor initialization
/// -----------------------------------------------------------------
void Editor::Editor::Initialize()
{
	//Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//Set ImGui style
	ImGui::StyleColorsDark();

	//Initialize SDL and OpenGL for Imgui
	ImGui_ImplSDL2_InitForOpenGL(gWindow->GetWindow(), gWindow->GetContext());
	ImGui_ImplOpenGL3_Init("#version 430");
}

/// -----------------------------------------------------------------
/// Update editor
/// -----------------------------------------------------------------
void Editor::Editor::Update()
{
	//Deal with SDL events on ImGui
	auto events = gSDLSys->GetEvents();
	for (auto ev : events)
		for (const auto& subev : ev.second)
			ImGui_ImplSDL2_ProcessEvent(&subev);

	//Start new render frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//Update scene editor and set object to visualize
	mSceneEditor.Update();
	mObjectEditor.SetSelectedObj(mSceneEditor.GetSelectedObj());

	//Update object editor and set selected object (in case it was deleted)
	mObjectEditor.Update();
	mSceneEditor.SetSelectedObj(mObjectEditor.GetSelectedObj());
	ImGui::End();
}

/// -----------------------------------------------------------------
/// Render editor
/// -----------------------------------------------------------------
void Editor::Editor::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// -----------------------------------------------------------------
/// Shutdown editor
/// -----------------------------------------------------------------
void Editor::Editor::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
