/*// -----------------------------------------------------------------
*  File:		EditObject.cpp
*  Brief:		Implementation of the object editor.
*  Creation:	11/12/2022
*  Last Update:	14/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditObject.hpp"

/// -----------------------------------------------------------------
/// Update object visualizer
/// -----------------------------------------------------------------
void Editor::ObjectEditor::Update()
{
	glm::vec2 windowSize = gWindow->GetSize();
	ImGuiBackendFlags defaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos({ windowSize.x - 350, 50 });
	ImGui::SetNextWindowSize({ 350, windowSize.y - 50 });
	if (ImGui::Begin("Object Manager", nullptr, defaultWindowFlags) && mSelectedObj != nullptr)
		EditPickedObject();
}

/// -----------------------------------------------------------------
/// Gets selected object on object visualizer
/// -----------------------------------------------------------------
Engine::Object* Editor::ObjectEditor::GetSelectedObj() const
{
	return mSelectedObj;
}

/// -----------------------------------------------------------------
/// Sets selected object on object visualizer
/// -----------------------------------------------------------------
void Editor::ObjectEditor::SetSelectedObj(Engine::Object* obj)
{
	mSelectedObj = obj;
}

/// -----------------------------------------------------------------
/// Visualize picked object
/// -----------------------------------------------------------------
void Editor::ObjectEditor::EditPickedObject()
{
	//Offset child windows
	glm::vec2 windowSize = gWindow->GetSize();
	float offset = (windowSize.y - 200) / 2;

	if (!ObjectProperties())
		return;

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

/// -----------------------------------------------------------------
/// Show object properties
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::ObjectProperties()
{
	//First window: properties
	ImGui::Text("Properties");
	ImGui::BeginChild("Properties", { ImGui::GetContentRegionAvail().x, 150 }, false, ImGuiWindowFlags_HorizontalScrollbar);
	std::string objName = mSelectedObj->GetName();
	ImGui::Text("Name:");
	ImGui::SameLine();
	if (ImGui::InputText("##", &objName, ImGuiInputTextFlags_EnterReturnsTrue))
		mSelectedObj->SetName(objName);
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
	bool objectDeleted = ImGui::Button("X");
	ImGui::PopStyleColor(3);
	if (objectDeleted)
	{
		mSelectedObj->GetSpace()->DeleteObject(mSelectedObj, mSelectedObj->GetSpaceIdx());
		mSelectedObj = nullptr;
		ImGui::EndChild();
		return false;
	}

	return true;
}
