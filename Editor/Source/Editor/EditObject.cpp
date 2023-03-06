/*// -----------------------------------------------------------------
*  File:		EditObject.cpp
*  Brief:		Implementation of the object editor.
*  Creation:	11/12/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditObject.hpp"
#include <Graphics/GraphicsManager.hpp>

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
		mSelectedObj->GetSpace()->DeleteObject(mSelectedObj);
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
	auto objEngineComps = mSelectedObj->GetEngineComps();

	//Loop in all engine components
	EditTransform(mSelectedObj->GetTransform());
	for (auto comp : objEngineComps)
	{
		std::string cmpName = comp->TypeInfo()->GetTypeName();
		if (!EditObjectEngineComp(cmpName, comp))
		{
			ImGui::EndChild();
			return false;
		}
		engineComps.erase(std::find(engineComps.begin(), engineComps.end(), cmpName));
	}
	
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	if (ImGui::Button("+"))
		ImGui::OpenPopup("Add Engine Comp");

	if (ImGui::BeginPopup("Add Engine Comp"))
	{
		for (auto compName : engineComps)
		{
			bool selected = ImGui::Selectable(compName.c_str());
			if (selected && compName == "Renderable")
				mSelectedObj->AddEngineComp<Engine::Renderable>()->Initialize();
			if (selected && compName == "Camera")
				mSelectedObj->AddEngineComp<Engine::Camera>()->Initialize();
		}
		ImGui::EndPopup();
	}
	ImGui::EndChild();
	return true;
}

/// -----------------------------------------------------------------
/// Edits object engine component (checks which one to edit)
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditObjectEngineComp(const std::string& cmpName, Engine::EngineComp* cmp)
{
	if (cmpName == "Renderable")
		return EditRenderable(reinterpret_cast<Engine::Renderable*>(cmp));
	if (cmpName == "Camera")
		return EditCamera(reinterpret_cast<Engine::Camera*>(cmp));
	
	//Stop everything
	return false;
}

/// -----------------------------------------------------------------
/// Edits transform component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditTransform(Engine::Transform* cmp)
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		glm::vec3 pos = cmp->GetWorldPos();
		glm::vec3 sca = cmp->GetWorldScale();
		glm::vec3 rot = cmp->GetWorldRot();

		//Show world coordinates of transform
		ImGui::BulletText("World Coordinates");
		glm::vec3 tmp = TransformDisplayCoords(pos, 0, -10000.f, 10000.f);
		if(tmp != pos)
			cmp->SetWorldPos(tmp);
		ImGui::Text("Pos");
		tmp = TransformDisplayCoords(sca, 3, 0.f, 10000.f);
		if (tmp != sca)
			cmp->SetWorldScale(tmp);
		ImGui::Text("Scale");
		tmp = TransformDisplayCoords(rot, 6, 0.f, 360.f);
		if(tmp != rot)
			cmp->SetWorldRot(tmp);
		ImGui::Text("Rot");
		ImGui::NewLine();

		//If it has parent, show also local coordinates of object
		if (cmp->GetParent())
		{
			pos = cmp->GetLocalPos();
			sca = cmp->GetLocalScale();
			rot = cmp->GetLocalRot();

			ImGui::Separator();
			ImGui::BulletText("Local Coordinates");
			tmp = TransformDisplayCoords(pos, 9, -10000.f, 10000.f);
			if (tmp != pos)
				cmp->SetLocalPos(tmp);
			ImGui::Text("Pos");
			tmp = TransformDisplayCoords(sca, 12, 0.f, 10000.f);
			if(tmp != sca)
				cmp->SetLocalScale(tmp);
			ImGui::Text("Scale");
			tmp = TransformDisplayCoords(rot, 15, 0.f, 360.f);
			if(tmp != rot)
				cmp->SetLocalRot(tmp);
			ImGui::Text("Rot");
			ImGui::NewLine();
		}
	}
	return true;
}

/// -----------------------------------------------------------------
/// Display transform coord
/// -----------------------------------------------------------------
glm::vec3 Editor::ObjectEditor::TransformDisplayCoords(const glm::vec3& coords, const int& coordId, const float& min, const float& max)
{
	glm::vec3 coordEdit = coords;
	std::string letter = "X";
	//Show X, Y and Z separately
	for (int idx = 0; idx < 3; idx++)
	{
		//Show reset button
		ImGui::PushID(coordId + idx);
		float h = (idx + 1) * 2.f / 6.f;
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(h, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(h, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(h, 0.8f, 0.8f));
		bool clicked = ImGui::Button(letter.c_str());
		ImGui::PopStyleColor(3);

		//If pressed, reset coord
		if (clicked)
			coordEdit[idx] = 0.f;
		ImGui::SameLine();

		//Show coord to edit and increase letter
		ImGui::PushItemWidth(65);
		ImGui::DragFloat("##", &coordEdit[idx], 0.1f, min, max);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::SameLine();
		letter[0]++;
	}	

	return coordEdit;
}

/// -----------------------------------------------------------------
/// Edits renderable component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditRenderable(Engine::Renderable* cmp)
{
	bool open = true;
	if (ImGui::CollapsingHeader("Renderable", &open))
	{
	}

	//If it was deleted, delete component
	if (!open)
	{
		mSelectedObj->DeleteEngineComp<Engine::Renderable>();
		return false;
	}
	return true;
}

/// -----------------------------------------------------------------
/// Edits camera component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditCamera(Engine::Camera* cmp)
{
	bool open = true;
	if (ImGui::CollapsingHeader("Camera", &open))
	{
	}

	//If it was deleted, delete component
	if (!open)
	{
		mSelectedObj->DeleteEngineComp<Engine::Renderable>();
		return false;
	}
	return true;
}
