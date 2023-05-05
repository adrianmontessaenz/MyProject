/*// -----------------------------------------------------------------
*  File:		EditObject.cpp
*  Brief:		Implementation of the object editor.
*  Creation:	11/12/2022
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditObject.hpp"
#include <Graphics/GraphicsManager.hpp>
#include "EditorHelpers.hpp"
#include "Editor.hpp"
#include "EditorRenderer.hpp"

/// -----------------------------------------------------------------
/// Update object visualizer
/// -----------------------------------------------------------------
void Editor::ObjectEditor::Update()
{
	glm::vec2 windowSize = gWindow->GetSize();
	ImGuiBackendFlags defaultWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos({ windowSize.x - 350, 50 });
	ImGui::SetNextWindowSize({ 350, windowSize.y - 50 });
	ImGui::Begin("Object Manager", nullptr, defaultWindowFlags);
	if (ImGui::IsWindowFocused())
		gEditor->SetFocused(true);
	if (mSelectedObj != nullptr)
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
	ImGui::PushID(&objName);
	ImGui::Text("Name:");
	ImGui::SameLine();
	if (ImGui::InputText("##", &objName, ImGuiInputTextFlags_EnterReturnsTrue))
		mSelectedObj->SetName(objName);
	ImGui::SameLine();
	ImGui::PopID();

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

	//Change object tag
	std::string objTag = mSelectedObj->GetTag();
	ImGui::PushID(&objTag);
	ImGui::Text("Tag:");
	ImGui::SameLine();
	if (ImGui::InputText("##", &objTag, ImGuiInputTextFlags_EnterReturnsTrue))
		mSelectedObj->SetTag(objTag);
	ImGui::PopID();

	//Enable or disable selected object
	bool enabled = mSelectedObj->IsEnabled();
	ImGui::Checkbox("Enabled", &enabled);
	mSelectedObj->SetEnabled(enabled);

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
			{
				auto rnd = mSelectedObj->AddEngineComp<Engine::Renderable>();
				rnd->Initialize();
				RenderEditor::GetInstance().AddRenderable(rnd);
			}
			if (selected && compName == "Camera")
				mSelectedObj->AddEngineComp<Engine::Camera>()->Initialize();
			if (selected && compName == "Collider")
			{
				auto col = mSelectedObj->AddEngineComp<Engine::Collider>();
				col->Initialize();
				RenderEditor::GetInstance().AddCollider(col);
			}
			if (selected && compName == "RigidBody")
				mSelectedObj->AddEngineComp<Engine::RigidBody>()->Initialize();
			if (selected && compName == "AudioEmitter")
				mSelectedObj->AddEngineComp<Engine::AudioEmitter>()->Initialize();
			if (selected && compName == "AudioListener")
				mSelectedObj->AddEngineComp<Engine::AudioListener>()->Initialize();
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
	if (cmpName == "Collider")
		return EditCollider(reinterpret_cast<Engine::Collider*>(cmp));
	if (cmpName == "RigidBody")
		return EditRigidBody(reinterpret_cast<Engine::RigidBody*>(cmp));
	if (cmpName == "AudioEmitter")
		return EditAudioEmitter(reinterpret_cast<Engine::AudioEmitter*>(cmp));
	if (cmpName == "AudioListener")
		return EditAudioListener(reinterpret_cast<Engine::AudioListener*>(cmp));
	//Stop everything
	return false;
}

/// -----------------------------------------------------------------
/// Edits transform component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditTransform(Engine::Transform* cmp)
{
	ImGui::PushID(&cmp);
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
		tmp = TransformDisplayCoords(rot, 6, -360.f, 360.f);
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
			tmp = TransformDisplayCoords(rot, 15, -360.f, 360.f);
			if(tmp != rot)
				cmp->SetLocalRot(tmp);
			ImGui::Text("Rot");
			ImGui::NewLine();
		}
	}
	ImGui::PopID();
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
	ImGui::PushID(&cmp);
	bool open = true;
	if (ImGui::CollapsingHeader("Renderable", &open))
	{
		//Enable or disable component
		bool active = cmp->IsActive();
		ImGui::Checkbox("Active", &active);
		cmp->SetActive(active);

		//Models
		std::string new_model = MyStringComboFromPath("Model:", cmp->GetModel(), "../data/models/", true, ".");
		cmp->SetModel(new_model);

		//Textures
		std::string new_texture = MyStringComboFromPath("Texture:", cmp->GetTexture(), "../data/textures/", true, ".");
		cmp->SetTexture(new_texture);

		//Edit color
		glm::vec4 color = cmp->GetColor();
		ImGui::Text("Color:");
		ImGui::SameLine();
		ImGui::ColorEdit4("##", &color[0]);
		cmp->SetColor(color);
	}

	//If it was deleted, delete component
	if (!open)
	{
		RenderEditor::GetInstance().RemoveRenderable(cmp);
		mSelectedObj->DeleteEngineComp<Engine::Renderable>();
	}
	ImGui::PopID();
	return open;
}

/// -----------------------------------------------------------------
/// Edits camera component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditCamera(Engine::Camera* cmp)
{
	ImGui::PushID(&cmp);
	bool open = true;
	if (ImGui::CollapsingHeader("Camera", &open))
	{
		//Enable or disable component
		bool active = cmp->IsActive();
		ImGui::Checkbox("Active", &active);
		cmp->SetActive(active);

		//Change camera size
		ImGui::Text("Change Camera Values:");
		ImGui::Separator();
		ImGui::NewLine();
		glm::vec<2,int> size = gWindow->GetSize();
		glm::vec<2,int> camSize = cmp->GetSize();
		MyDragInt("Width: ", &(camSize.x), 60.f, 0, size.x);
		ImGui::SameLine();
		MyDragInt("Height:", &(camSize.y), 60.f, 0, size.y);
		cmp->SetWidth(camSize.x);
		cmp->SetHeight(camSize.y);

		//Change planes
		glm::vec2 planes = cmp->GetPlanes();
		MyDragFloat("Near:  ", &(planes.x), 60.f, 0, 1000.f);
		ImGui::SameLine();
		MyDragFloat("Far:   ", &(planes.y), 60.f, 0, 1000.f);
		cmp->SetNear(planes.x);
		cmp->SetFar(planes.y);
		
		//Change FOV
		float fov = cmp->GetFOV();
		MyDragFloat("FOV:   ", &fov, 60.f, 0, 90.f);
		cmp->SetFOV(fov);
	}

	//If it was deleted, delete component
	if (!open)
		mSelectedObj->DeleteEngineComp<Engine::Camera>();
	ImGui::PopID();
	return open;
}

/// -----------------------------------------------------------------
/// Edits collider component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditCollider(Engine::Collider* cmp)
{
	ImGui::PushID(&cmp);
	bool open = true;
	if (ImGui::CollapsingHeader("Collider", &open))
	{
		//Enable or disable component
		bool active = cmp->IsActive();
		ImGui::Checkbox("Active", &active);
		cmp->SetActive(active);

		//Set collider ghost or not
		bool ghst = cmp->IsGhost();
		ImGui::Checkbox("Ghost", &ghst);
		cmp->SetGhost(ghst);

		//Edit collider offsets
		ImGui::Text("Collider Offsets");
		glm::vec3 tmp = TransformDisplayCoords(cmp->GetOffsetPosition(), 18, -10000.f, 10000.f);
		ImGui::Text("Pos");
		cmp->SetOffsetPosition(tmp);
		tmp = TransformDisplayCoords(cmp->GetOffsetScale(), 21, 0.f, 10000.f);
		ImGui::Text("Scale");
		cmp->SetOffsetScale(tmp);
		tmp = TransformDisplayCoords(cmp->GetOffsetRotation(), 24, 0.f, 360.f);
		ImGui::Text("Rot");
		cmp->SetOffsetRotation(tmp);
		ImGui::NewLine();

		//Set if draw collider
		bool draw = cmp->IsColliderDrawn();
		ImGui::Checkbox("Draw Collider", &draw);
		cmp->SetDrawCollider(draw);

		//Change collider type
		if (ImGui::TreeNode("Set Collider Type"))
		{
			int type = cmp->GetColliderType();
			if (ImGui::RadioButton("AABB", type == 1))
				cmp->SetColliderType(Engine::Collider::COLLIDER_AABB);
			ImGui::RadioButton("Sphere", type == 2);
			ImGui::RadioButton("OOBB", type == 4);
			ImGui::TreePop();
		}
	}

	//If it was deleted, delete component
	if (!open)
	{
		RenderEditor::GetInstance().RemoveCollider(cmp);
		mSelectedObj->DeleteEngineComp<Engine::Collider>();
	}
	ImGui::PopID();
	return open;
}

/// -----------------------------------------------------------------
/// Edits rigidbody component
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditRigidBody(Engine::RigidBody* cmp)
{
	ImGui::PushID(&cmp);
	bool open = true;
	if (ImGui::CollapsingHeader("RigidBody", &open))
	{
		//Enable or disable component
		bool active = cmp->IsActive();
		ImGui::Checkbox("Is Active", &active);
		cmp->SetActive(active);

		//Set static or dynamic
		bool stc = cmp->IsStatic();
		ImGui::Checkbox("Is Static", &stc);
		cmp->SetStatic(stc);

		//Set gravity mode
		bool gravity = cmp->HasGravity();
		ImGui::Checkbox("Has Gravity", &gravity);
		cmp->SetGravity(gravity);

		//Set mass
		float mass = cmp->GetMass();
		MyDragFloat("Mass:   ", &mass, 60.f, 0.f, 1000.f);
		cmp->SetMass(mass);
		
		//Show velocity
		ImGui::Separator();
		ImGui::Text("Debug Velocity");
		glm::vec3 tmp = TransformDisplayCoords(cmp->GetVelocity(), 27, -10000.f, 10000.f);
		ImGui::NewLine();
	}

	//If it was deleted, delete component
	if (!open)
		mSelectedObj->DeleteEngineComp<Engine::RigidBody>();
	ImGui::PopID();
	return open;
}

/// -----------------------------------------------------------------
/// Edits audio emitter
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditAudioEmitter(Engine::AudioEmitter* cmp)
{
	ImGui::PushID(&cmp);
	bool open = true;
	if (ImGui::CollapsingHeader("AudioEmitter", &open))
	{
		//Enable or disable component
		bool active = cmp->IsActive();
		ImGui::Checkbox("Is Active", &active);
		cmp->SetActive(active);

		//Change emitter type
		Engine::AudioEmitter::AudioEmitterTypes type = cmp->GetAudioType();
		if (ImGui::TreeNode("Set Emitter Type"))
		{
			if (ImGui::RadioButton("Default", type == Engine::AudioEmitter::AudioEmitterTypes::AUDIO_DEFAULT))
				cmp->SetAudioType(Engine::AudioEmitter::AudioEmitterTypes::AUDIO_DEFAULT);
			else if (ImGui::RadioButton("Music", type == Engine::AudioEmitter::AudioEmitterTypes::AUDIO_MUSIC))
				cmp->SetAudioType(Engine::AudioEmitter::AudioEmitterTypes::AUDIO_MUSIC);
			else if (ImGui::RadioButton("Sound", type == Engine::AudioEmitter::AudioEmitterTypes::AUDIO_SOUND))
				cmp->SetAudioType(Engine::AudioEmitter::AudioEmitterTypes::AUDIO_SOUND);
			ImGui::TreePop();
		}
		type = cmp->GetAudioType();
		std::string mySound = cmp->GetSoundName();
		
		//Change sound name depending on type
		if (type == Engine::AudioEmitter::AudioEmitterTypes::AUDIO_MUSIC)
			mySound = MyStringComboFromPath("Music:", mySound, "../Data/Audio/Music/", true, ".");
		else
			mySound = MyStringComboFromPath("Sound:", mySound, "../Data/Audio/Sounds/", true, ".");
		cmp->SetSoundName(mySound);

		//Pause
		bool paused = cmp->IsSoundPaused();
		ImGui::Checkbox("Pause", &paused);
		cmp->SetSoundPaused(paused);

		//Loop
		bool loop = cmp->IsLooping();
		ImGui::Checkbox("Loop", &loop);
		cmp->SetLooping(loop);
		
		//Volume
		float volume = cmp->GetVolume();
		ImGui::SliderFloat("Volume", &volume, 0.f, 1.f);
		cmp->SetVolume(volume);

		if (ImGui::Button("Play Emitter"))
			cmp->PlaySound();
		ImGui::SameLine();
		if (ImGui::Button("Stop Emitter"))
			cmp->StopSound();
	}

	//If it was deleted, delete component
	if (!open)
		mSelectedObj->DeleteEngineComp<Engine::AudioEmitter>();
	ImGui::PopID();
	return open;
}

/// -----------------------------------------------------------------
/// Edits audio listener
/// -----------------------------------------------------------------
bool Editor::ObjectEditor::EditAudioListener(Engine::AudioListener* cmp)
{
	ImGui::PushID(&cmp);
	bool open = true;
	if (ImGui::CollapsingHeader("AudioListener", &open))
	{

	}

	//If it was deleted, delete component
	if (!open)
	mSelectedObj->DeleteEngineComp<Engine::AudioListener>();
	ImGui::PopID();
	return open;
}
