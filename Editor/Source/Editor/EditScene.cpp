/*// -----------------------------------------------------------------
*  File:		EditScene.cpp
*  Brief:		Implementation of the editor scene manager.
*  Creation:	11/12/2022
*  Last Update:	01/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditScene.hpp"
#include "EditorRenderer.hpp"
#include "Editor.hpp"
#include <Core/Time/TimeSystem.hpp>
#include <Physics/PhysicsManager.hpp>

/// -----------------------------------------------------------------
/// Update scene editor
/// -----------------------------------------------------------------
void Editor::SceneEditor::Update()
{
	//Create main bar for loading/saving scenes
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::IsWindowFocused())
			gEditor->SetFocused(true);
		std::string mLvlName = gObjMgr->GetLvlName();
		if (ImGui::BeginMenu(mLvlName.c_str()))
		{
			//Space name editor and object addition button
			ImGui::PushID(200);
			if (ImGui::InputTextWithHint("##", "Enter new name", &mLvlName, ImGuiInputTextFlags_EnterReturnsTrue))
				gObjMgr->SetLvlName(mLvlName);
			ImGui::PopID();
			ImGui::EndMenu();
		}

		//Save scene if not default
		if (ImGui::MenuItem("Save") && mLvlName != "Default")
		{
			std::string path = "../data/levels/" + mLvlName + ".json";
			gObjMgr->SaveScene(path);
		}

		//Load scene
		if (ImGui::BeginMenu("Load"))
		{
			std::string path = "../data/levels/";
			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				//If loaded a saved scene, shutdown and load
				std::string file = entry.path().filename().string();
				file = file.substr(0, file.size() - 5);
				if (ImGui::MenuItem(file.c_str()))
				{
					std::string new_path = path + file + ".json";
					gObjMgr->Shutdown();
					gRenderEditor->Shutdown();
					gObjMgr->LoadScene(new_path);
					gObjMgr->Initialize();
					gRenderEditor->Initialize();
					mSelectedObj = nullptr;
				}
			}

			//If loading default, shutdown
			if (ImGui::MenuItem("Default"))
			{
				gObjMgr->Shutdown();
				mSelectedObj = nullptr;
			}
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Play"))
		{
			bool playing = gEditor->IsPlaying();
			if (playing)
			{
				std::string path = "../data/levels/";
				gObjMgr->Shutdown();
				gRenderEditor->Shutdown();
				gPhysics->Shutdown();
				gObjMgr->LoadScene(path + gObjMgr->GetLvlName() + ".json");
				gObjMgr->Initialize();
				gRenderEditor->Initialize();
				mSelectedObj = nullptr;
			}
			gEditor->SetPlaying(!playing);
		}
		ImGui::EndMainMenuBar();
	}

	//Create initial window
	glm::vec2 windowSize = gWindow->GetSize();
	ImGuiBackendFlags defaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos({ 0, 50 });
	ImGui::SetNextWindowSize({ 350, windowSize.y - 50 });
	ImGui::Begin("Scene Visualizer", nullptr, defaultWindowFlags);
	
	if (ImGui::IsWindowFocused())
		gEditor->SetFocused(true);

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
	if (ImGui::BeginTable(space->GetName().c_str(), 2, spaceTableFlags, spaceMaxSize))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Unique ID", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();
		std::vector<Engine::Object*> objects = space->GetObjects();
		for (size_t idx2 = 0; idx2 < objects.size(); idx2++)
		{
			if (objects[idx2]->GetParent())
				continue;
			if (!VisualizeObject(objects[idx2], idx2))
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
bool Editor::SceneEditor::DragAndDropObjects(Engine::Space* space, Engine::Object* obj)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("Add_Children", &(*obj), sizeof(Engine::Object));
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		//Add child to parent
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Add_Children"))
		{
			//Get object
			Engine::Object* new_child = (Engine::Object*)payload->Data;
			new_child = space->GetObjectByName(new_child->GetName());
			Engine::Object* parent_hierarchy = obj;
			
			//If one of the object's parent is the object to add, don't do it
			while (parent_hierarchy)
			{
				//Can't make an infinite hierarchy
				if (parent_hierarchy == new_child)
					return true;
				parent_hierarchy = parent_hierarchy->GetParent();
			}

			//If all good, make child
			obj->AddChild(new_child);
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
bool Editor::SceneEditor::ReOrderObjects(std::vector<Engine::Object*> objs_, Engine::Object* obj_, const size_t idx)
{
	//If on cooldown, recharge
	if (mOrderCd > 0.f && mOrderCd <= mMaxCd)
	{
		mOrderCd += gTimeSys->GetDeltaTime();
		return true;
	}
	mOrderCd = 0.f;

	//Drag objects
	if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
	{
		//Get increment
		int increment = (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		int next = static_cast<int>(idx) + increment;
		if (next >= 0 && next < objs_.size())
		{
			//Keep incrementing until same parent level
			Engine::Object* parent = obj_->GetParent();
			while (parent != objs_[next]->GetParent())
			{
				next += increment;
				if (next < 0 || next >= objs_.size())
					return true;
			}

			//Swap objects and start cooldown
			Engine::Space* space = objs_[idx]->GetSpace();
			if (!obj_->GetParent())
				space->SwapObjects(idx, next);
			else
				obj_->GetParent()->SwapChildren(idx, next);
			ImGui::ResetMouseDragDelta();
			mOrderCd += gTimeSys->GetDeltaTime();
			return false;
		}
	}

	return true;
}

/// -----------------------------------------------------------------
/// Visualize object on editor
/// -----------------------------------------------------------------
bool Editor::SceneEditor::VisualizeObject(Engine::Object* obj, const size_t idx)
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
	auto objs = obj->GetSpace()->GetObjects();
	if ((!mAddChildren && !ReOrderObjects(objs, obj, idx)) ||
		(mAddChildren && !DragAndDropObjects(obj->GetSpace(), obj)))
	{
		if (children.empty() || (childOpen && children.empty() == false))
			ImGui::TreePop();
		ImGui::PopID();
		return false;
	}

	//Set ID on second column
	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%u", obj->GetUniqueID());

	if (childOpen)
	{
		for (size_t idx2 = 0; idx2 < children.size(); idx2++)
		{
			if (!VisualizeChildren(children[idx2], idx2))
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

/// -----------------------------------------------------------------
/// Visualize children on editor
/// -----------------------------------------------------------------
bool Editor::SceneEditor::VisualizeChildren(Engine::Object* obj, const size_t idx)
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
	auto objs = obj->GetParent()->GetChildren();
	if ((!mAddChildren && !ReOrderObjects(objs, obj, idx)) ||
		(mAddChildren && !DragAndDropObjects(obj->GetSpace(), obj)))
	{
		if (children.empty() || (childOpen && children.empty() == false))
			ImGui::TreePop();
		ImGui::PopID();
		return false;
	}

	//Set ID on second column
	ImGui::TableSetColumnIndex(1);
	ImGui::Text("%u", obj->GetUniqueID());

	if (childOpen)
	{
		for (size_t idx2 = 0; idx2 < children.size(); idx2++)
		{
			if (!VisualizeChildren(children[idx2], idx2))
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
