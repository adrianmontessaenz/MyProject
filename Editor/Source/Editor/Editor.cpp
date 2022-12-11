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

	glm::vec2 windowSize = gWindow->GetSize();
	ImGuiBackendFlags defaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos({ 0, 50 });
	ImGui::SetNextWindowSize({ 350, windowSize.y - 50 });
	ImGui::Begin("Scene Visualizer", nullptr, defaultWindowFlags);

	//Create new space
	if (ImGui::Button("Add Space"))
	{
		Engine::Space* space = gObjMgr->AddSpace();
		space->SetName("Default");
	}

	//Loop on scene
	std::vector<Engine::Space*> spaces = gObjMgr->GetSpaces();
	for (size_t idx = 0; idx < spaces.size(); idx++)
	{
		//Set name
		ImGui::PushID(spaces[idx]->GetUniqueID());
		bool isOpen = ImGui::CollapsingHeader(spaces[idx]->GetName().c_str());
		
		//Drag spaces
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("Space Order Change", &idx, sizeof(size_t));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Space Order Change"))
			{
				size_t changeIdx = *(size_t*)payload->Data;
				gObjMgr->SwapSpaces(idx, changeIdx);
				ImGui::PopID();
				break;
			}
			ImGui::EndDragDropTarget();
		}

		if (isOpen)
		{
			std::string spaceName;
			if (ImGui::InputTextWithHint("##", "Enter new name", &spaceName, ImGuiInputTextFlags_EnterReturnsTrue))
				spaces[idx]->SetName(spaceName);

			//Delete space button
			ImGui::SameLine();
			if (ImGui::Button("Add Object"))
			{
				Engine::Object* tmp = spaces[idx]->AddObject();
				tmp->SetName("Default");
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
			if (ImGui::Button("X"))
			{
				if (mSelectedObj->GetSpace() == spaces[idx])
					mSelectedObj = nullptr;
				ImGui::PopID();
				gObjMgr->DeleteSpace(spaces[idx]);
				ImGui::PopStyleColor(3);
				break;
			}
			else
				ImGui::PopStyleColor(3);

			//Show objects of space
			ImVec2 spaceMaxSize = ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing() * 15.f);
			ImGuiTableFlags spaceTableFlags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY;
			if (ImGui::BeginTable(spaces[idx]->GetName().c_str(), 3, spaceTableFlags, spaceMaxSize))
			{
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Unique ID", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Space IDX", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableHeadersRow();
				std::vector<Engine::Object*> objects = spaces[idx]->GetObjects();
				for (size_t idx2 = 0; idx2 < objects.size(); idx2++)
				{
					if (objects[idx2]->GetParent())
						continue;
					VisualizeObject(objects[idx2], idx2);
				}
				ImGui::EndTable();
			}
		}
		ImGui::PopID();
	}
	ImGui::End();

	ImGui::SetNextWindowPos({ windowSize.x - 350, 50 });
	ImGui::SetNextWindowSize({ 350, windowSize.y - 50 });
	if (ImGui::Begin("Object Manager", nullptr, defaultWindowFlags) && mSelectedObj != nullptr)
		EditPickedObject();
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

/// -----------------------------------------------------------------
/// Visualize object on editor
/// -----------------------------------------------------------------
void Editor::Editor::VisualizeObject(Engine::Object* obj, const unsigned idx)
{
	//Push id of object and set name first
	ImGui::PushID(obj->GetUniqueID());
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::AlignTextToFramePadding();
	std::vector<Engine::Object*> children = obj->GetChildren();
	bool childOpen = false;

	//If parent, or end of hierarchy set as leaf. Otherwise set as treenode
	ImGuiTreeNodeFlags objectFlags = ImGuiTreeNodeFlags_None;
	if (mSelectedObj == obj)
		objectFlags |= ImGuiTreeNodeFlags_Selected;
	if(children.empty())
		objectFlags |= ImGuiTreeNodeFlags_Leaf;
	else if(children.empty() == false)
		objectFlags |= ImGuiTreeNodeFlags_OpenOnArrow;

	childOpen = ImGui::TreeNodeEx(obj->GetName().c_str(), objectFlags);
	if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
		mSelectedObj = obj;

	//Set ID on second column
	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%u", obj->GetUniqueID());

	//Set Space Index on second column
	ImGui::TableSetColumnIndex(2);
	ImGui::Text("%u", idx);

	//Show children
	if (children.empty() == false && childOpen)
	{
		for (Engine::Object* child : children)
			VisualizeObject(child, idx + 1);
	}
	ImGui::TreePop();
	ImGui::PopID();
}

/// -----------------------------------------------------------------
/// Visualize picked object
/// -----------------------------------------------------------------
void Editor::Editor::EditPickedObject()
{
	//Offset child windows
	glm::vec2 windowSize = gWindow->GetSize();
	float offset = (windowSize.y - 200) / 2;

	//First window: properties
	ImGui::Text("Properties");
	ImGui::BeginChild("Properties", { ImGui::GetContentRegionAvail().x, 150 }, false, ImGuiWindowFlags_HorizontalScrollbar);
	std::string objName = mSelectedObj->GetName();
	ImGui::Text("Name:");
	ImGui::SameLine();
	if (ImGui::InputText("##", &objName, ImGuiInputTextFlags_EnterReturnsTrue))
		mSelectedObj->SetName(objName);
	ImGui::SameLine();
	if (ImGui::Button("X"))
	{
		mSelectedObj->Shutdown();
		mSelectedObj = nullptr;
		ImGui::EndChild();
		return;
	}

	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Text("Engine Components");
	ImGui::BeginChild("EngineComps", { ImGui::GetContentRegionAvail().x, offset - 50.f }, false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Text("Logic Components");
	ImGui::BeginChild("LogicComps", { ImGui::GetContentRegionAvail().x, offset - 54.f }, false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::EndChild();
}
