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
	{
		ImGui::PushID(mSelectedObj->GetUniqueID());
		EditPickedObject();
		ImGui::PopID();
	}
	ImGui::End();
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
	if (!ObjectProperties())
		return;
	ImGui::Separator();

	if (!ObjectEngineComponents())
		return;

	//Offset child windows
	glm::vec2 windowSize = gWindow->GetSize();
	float offset = (windowSize.y - 200) / 2;

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

	ImGui::EndChild();
	return true;
}

/// -----------------------------------------------------------------
/// Show object engine components
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::ObjectEngineComponents()
{
	//Offset child windows
	glm::vec2 windowSize = gWindow->GetSize();
	float offset = (windowSize.y - 200) / 2;

	//Show engine components of the object
	ImGui::Text("Engine Components");
	ImGui::BeginChild("EngineComps", { ImGui::GetContentRegionAvail().x, offset - 50.f }, false, ImGuiWindowFlags_HorizontalScrollbar);
	auto engineComps = Engine::RTTI::GetTypesWithParent("EngineComp");

	//Loop in all engine components
	for (size_t idx = 0; idx < engineComps.size();)
	{
		Engine::EngineComp* cmp = GetObjectEngineComp(engineComps[idx]);
		if (cmp)
		{
			if (!EditObjectEngineComp(engineComps[idx], cmp))
			{
				ImGui::EndChild();
				return false;
			}
			engineComps.erase(engineComps.begin() + idx);
		}
		else
			idx++;
	}
	
	ImGui::EndChild();
	return true;
}

/// -----------------------------------------------------------------
/// Checks if object has engine component
/// -----------------------------------------------------------------
Engine::EngineComp* Editor::ObjectEditor::GetObjectEngineComp(const std::string compName)
{
	if (compName == "Transform")
		return mSelectedObj->GetEngineComp<Engine::Transform>();
	else if (compName == "Renderable")
		return mSelectedObj->GetEngineComp<Engine::Renderable>();
	return nullptr;
}

/// -----------------------------------------------------------------
/// Edits object engine component (checks which one to edit)
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditObjectEngineComp(const std::string& cmpName, Engine::EngineComp* cmp)
{
	if (cmpName == "Transform")
		return EditTransform(reinterpret_cast<Engine::Transform*>(cmp));
	else if (cmpName == "Renderable")
		return EditRenderable(reinterpret_cast<Engine::Renderable*>(cmp));
	
	//Stop everything
	return false;
}

/// -----------------------------------------------------------------
/// Edits transform component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditTransform(Engine::Transform* cmp)
{
	bool open = true;
	if (ImGui::CollapsingHeader("Transform", &open))
	{
		glm::vec3 pos = cmp->GetWorldPos();
		glm::vec3 sca = cmp->GetWorldScale();
		glm::vec3 rot = cmp->GetWorldRot();

		//Show world coordinates of transform
		ImGui::BulletText("World Coordinates");
		TransformDisplayCoords(pos, 0, -10000.f, 10000.f);
		ImGui::Text("Pos");
		cmp->SetWorldPos(pos);
		TransformDisplayCoords(sca, 3, 0.f, 10000.f);
		ImGui::Text("Scale");
		cmp->SetWorldScale(sca);
		TransformDisplayCoords(rot, 6, 0.f, 360.f);
		ImGui::Text("Rot");
		cmp->SetWorldRot(rot);
		ImGui::NewLine();

		//If it has parent, show also local coordinates of object
		if (cmp->GetOwner()->GetParent())
		{
			pos = cmp->GetLocalPos();
			sca = cmp->GetLocalScale();
			rot = cmp->GetLocalRot();

			ImGui::Separator();
			ImGui::BulletText("Local Coordinates");
			TransformDisplayCoords(pos, 9, -10000.f, 10000.f);
			ImGui::Text("Pos");
			cmp->SetLocalPos(pos);
			TransformDisplayCoords(sca, 12, 0.f, 10000.f);
			ImGui::Text("Scale");
			cmp->SetLocalScale(sca);
			TransformDisplayCoords(rot, 15, 0.f, 360.f);
			ImGui::Text("Rot");
			cmp->SetLocalRot(rot);
			ImGui::NewLine();
		}
	}

	//If it was deleted, delete component
	if (!open)
	{
		mSelectedObj->DeleteEngineComp<Engine::Transform>();
		return false;
	}
	return true;
}

void Editor::ObjectEditor::TransformDisplayCoords(glm::vec3& coords, const int& coordId, const float& min, const float& max)
{
	for (std::string letter = "X"; letter[0] <= 'Z'; letter[0]++)
	{
		ImGui::PushID(coordId + letter[0] - 'X');
		float h = (letter[0] - 'X' + 1) * 2.f / 6.f;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(h, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(h, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(h, 0.8f, 0.8f));
		bool clicked = ImGui::Button(letter.c_str());
		ImGui::PopStyleColor(3);
		if (clicked)
			coords[letter[0] - 'X'] = 0.f;
		ImGui::SameLine();
		ImGui::PushItemWidth(65);
		float coord = coords[letter[0] - 'X'];
		ImGui::DragFloat("##", &coord, 0.1f, min, max);
		coords[letter[0] - 'X'] = coord;
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::SameLine();
	}	
}

/// -----------------------------------------------------------------
/// Edits renderable component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditRenderable(Engine::Renderable* cmp)
{
	return false;
}
