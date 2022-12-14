/*// -----------------------------------------------------------------
*  File:		EditScene.cpp
*  Brief:		Implementation of the editor scene manager.
*  Creation:	11/12/2022
*  Last Update:	14/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditScene.hpp"
#include <Core/Scene/ObjectManager.hpp>

/// -----------------------------------------------------------------
/// Update scene editor
/// -----------------------------------------------------------------
void Editor::SceneEditor::Update()
{
	//Create initial window
	glm::vec2 windowSize = gWindow->GetSize();
	ImGuiBackendFlags defaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos({ 0, 50 });
	ImGui::SetNextWindowSize({ 350, windowSize.y - 50 });
	ImGui::Begin("Scene Visualizer", nullptr, defaultWindowFlags);

	//Button to create new space on scene
	if (ImGui::Button("Add Space"))
	{
		Engine::Space* space = gObjMgr->AddSpace();
		space->SetName("Default");
	}

	//Loop on scene spaces
	std::vector<Engine::Space*> spaces = gObjMgr->GetSpaces();
	size_t maxSize = spaces.size();
	for (size_t idx = 0; idx < maxSize; idx++)
	{
		//Set name and check drag and drop
		ImGui::PushID(spaces[idx]->GetUniqueID());
		bool isOpen = ImGui::CollapsingHeader(spaces[idx]->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
		if(!DragAndDropSpaces(idx, maxSize))
			break;

		//If window is open, show space properties
		if (isOpen)
		{
			if (!VisualizeSpace(spaces[idx]))
				break;
		}
		ImGui::PopID();
	}
	ImGui::End();
}

/// -----------------------------------------------------------------
/// Gets selected object on scene visualizer
/// -----------------------------------------------------------------
Engine::Object* Editor::SceneEditor::GetSelectedObj() const
{
	return mSelectedObj;
}

/// -----------------------------------------------------------------
/// Sets selected object on scene visualizer
/// -----------------------------------------------------------------
void Editor::SceneEditor::SetSelectedObj(Engine::Object* obj)
{
	mSelectedObj = obj;
}

/// -----------------------------------------------------------------
/// Drag and Drop part for spaces
/// -----------------------------------------------------------------
bool Editor::SceneEditor::DragAndDropSpaces(const size_t& idx, const size_t& max_size)
{
	//Drag spaces
	if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
	{
		int next = static_cast<int>(idx) + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		if (next >= 0 && next < max_size)
		{
			gObjMgr->SwapSpaces(idx, next);
			ImGui::PopID();
			ImGui::ResetMouseDragDelta();
			return false;
		}
	}

	//No drags
	return true;
}

/// -----------------------------------------------------------------
///	Visualize space on editor
/// -----------------------------------------------------------------
bool Editor::SceneEditor::VisualizeSpace(Engine::Space* space)
{
	//Show space properties and draw table
	if(!SpaceProperties(space))
		return false;

	ImVec2 spaceMaxSize = ImVec2(0.f, ImGui::GetTextLineHeightWithSpacing() * 15.f);
	ImGuiTableFlags spaceTableFlags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY;
	if (ImGui::BeginTable(space->GetName().c_str(), 3, spaceTableFlags, spaceMaxSize))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Unique ID", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Space IDX", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		std::vector<Engine::Object*> objects = space->GetObjects();
		for (size_t idx2 = 0; idx2 < objects.size(); idx2++)
		{
			if (objects[idx2]->GetParent())
				continue;
			if (!VisualizeObject(objects[idx2]))
			{
				ImGui::EndTable();
				ImGui::PopID();
				return false;
			}
		}
		ImGui::EndTable();
	}

	return true;
}

/// -----------------------------------------------------------------
///	Visualize space properties on editor
/// -----------------------------------------------------------------
bool Editor::SceneEditor::SpaceProperties(Engine::Space* space)
{
	//Space name editor and object addition button
	std::string spaceName;
	if (ImGui::InputTextWithHint("##", "Enter new name", &spaceName, ImGuiInputTextFlags_EnterReturnsTrue))
		space->SetName(spaceName);
	ImGui::SameLine();
	if (ImGui::Button("Add Object"))
	{
		Engine::Object* tmp = space->AddObject();
		std::string tmp_name;
		tmp_name.push_back('A' + rand() % 21);
		tmp->SetName(tmp_name);
	}

	//Delete space button
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
	bool spaceDeleted = ImGui::Button("X");
	ImGui::PopStyleColor(3);
	if (spaceDeleted)
	{
		if (mSelectedObj && mSelectedObj->GetSpace() == space)
			mSelectedObj = nullptr;
		ImGui::PopID();
		gObjMgr->DeleteSpace(space);
		return false;
	}

	//Add children button
	ImGui::Checkbox("Add Children", &mAddChildren);
	return true;
}

/// -----------------------------------------------------------------
/// Drag and drop for objects in space visualizer
/// -----------------------------------------------------------------
bool Editor::SceneEditor::DragAndDropObjects(Engine::Space* space, const size_t& idx)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("Add_Children", &idx, sizeof(size_t));
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		//Add child to parent
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Add_Children"))
		{
			//Get object
			std::vector<Engine::Object*> objs = space->GetObjects();
			size_t payload_n = *(size_t*)payload->Data;
			Engine::Object* new_child = objs[payload_n];
			Engine::Object* parent_hierarchy = objs[idx]->GetParent();
			
			//If one of the object's parent is the object to add, don't do it
			while (parent_hierarchy)
			{
				//Can't make an infinite hierarchy
				if (parent_hierarchy == new_child)
					return true;
				parent_hierarchy = parent_hierarchy->GetParent();
			}

			//If all good, make child
			objs[idx]->AddChild(new_child);
			ImGui::EndDragDropTarget();
			return false;
		}
	}

	//No changes
	return true;
}

/// -----------------------------------------------------------------
/// Order editor for space visualizer
/// -----------------------------------------------------------------
bool Editor::SceneEditor::ReOrderObjects(const std::vector<Engine::Object*>& objs, const size_t& idx, const size_t& max_size)
{
	//Drag objects
	if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
	{
		int next = static_cast<int>(idx) + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		if (next >= 0 && next < objs.size())
		{
			Engine::Space* space = objs[idx]->GetSpace();
			if (objs[idx]->GetParent())
				objs[idx]->GetParent()->SwapChildren(idx, next);
			space->SwapObjects(idx, next);
			ImGui::ResetMouseDragDelta();
			return false;
		}
	}

	return true;
}

/// -----------------------------------------------------------------
/// Visualize object on editor
/// -----------------------------------------------------------------
bool Editor::SceneEditor::VisualizeObject(Engine::Object* obj)
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
	if (children.empty())
		objectFlags |= ImGuiTreeNodeFlags_Leaf;
	else
		objectFlags |= ImGuiTreeNodeFlags_OpenOnArrow;

	childOpen = ImGui::TreeNodeEx(obj->GetName().c_str(), objectFlags);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		mSelectedObj = obj;

	//Drag and drop. If child, only drag and drop inside parent objects
	auto objs = obj->GetParent() == nullptr ? obj->GetSpace()->GetObjects() : obj->GetParent()->GetChildren();
	size_t idx = obj->GetParent() == nullptr ? obj->GetSpaceIdx() : obj->GetParentIdx();
	if ((!mAddChildren && !ReOrderObjects(objs, idx, objs.size())) ||
		(mAddChildren && !DragAndDropObjects(obj->GetSpace(), obj->GetSpaceIdx())))
	{
		if (children.empty() || (childOpen && children.empty() == false))
			ImGui::TreePop();
		ImGui::PopID();
		return false;
	}

	//Set ID on second column
	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%u", obj->GetUniqueID());

	//Set Space Index on second column
	ImGui::TableSetColumnIndex(2);
	ImGui::Text("%u", obj->GetSpaceIdx());

	if (childOpen)
	{
		for (auto child : children)
		{
			if (!VisualizeObject(child))
			{
				ImGui::TreePop();
				ImGui::PopID();
				return false;
			}
		}
	}
	if (children.empty() || (childOpen && children.empty() == false))
		ImGui::TreePop();
	ImGui::PopID();
	return true;
}
