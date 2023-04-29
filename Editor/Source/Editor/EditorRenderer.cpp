/*// -----------------------------------------------------------------
*  File:		EditorRenderer.cpp
*  Brief:		Implementation of editor renderer
*  Creation:	06/03/2023
*  Last Update:	28/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditorRenderer.hpp"
#include <Core/Platform/InputManager.hpp>
#include <Graphics/GraphicsManager.hpp>
#include <Core/Time/TimeSystem.hpp>
#include "Editor.hpp"

/// -----------------------------------------------------------------
/// Initialize editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::Initialize()
{
	//Create camera
	mCamObj = new Engine::Object();
	mCam = mCamObj->AddEngineComp<Engine::Camera>();
	mCamObj->Initialize();
	mCamObj->GetTransform()->SetWorldPos({ 0.f,0.f,2.f });
	mCamObj->GetTransform()->SetWorldRot({ 0.f,0.f,-1.f });

	//Get all renderables at the beginning (if scene was loaded)
	for (auto space : gObjMgr->GetSpaces())
	{
		for (auto obj : space->GetObjects())
		{
			if (auto cmp1 = obj->GetEngineComp<Engine::Renderable>())
				mRenderableScene[space].push_back(cmp1);
			if (auto cmp2 = obj->GetEngineComp<Engine::Collider>())
				mColliderScene[space].push_back(cmp2);
		}
	}
}

/// -----------------------------------------------------------------
/// Update camera of editor
/// -----------------------------------------------------------------
void Editor::RenderEditor::Update()
{
	//Update object and check if focused in any window
	mCamObj->Update();
	if (gEditor->IsFocused())
		return;

	//Set mouse position in center if clicked only once
	glm::vec<2,int> size = gWindow->GetSize();
	if (gInputMgr->IsMouseTriggered(SDL_BUTTON_RIGHT))
	{
		SDL_ShowCursor(false);
		SDL_WarpMouseInWindow(gWindow->GetWindow(), size.x / 2, size.y / 2);
	}

	//If pressed, handle mouse rotation
	if (gInputMgr->IsMousePressed(SDL_BUTTON_RIGHT))
	{
		//Disable and get size and mouse pos
		glm::vec2 mousePos = gInputMgr->GetMousePos();
		glm::vec2 mouseRot;

		//Handle camera movement
		glm::vec3 camPos = mCamObj->GetTransform()->GetWorldPos();
		glm::vec3 camRot = mCamObj->GetTransform()->GetWorldRot();
		glm::vec3 camFront = mCam->GetFront();
		glm::vec3 camUp = mCam->GetUp();
		glm::vec3 camRight = mCam->GetRight();
		float dt = gTimeSys->GetDeltaTime();

		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_W))
			camPos += mSpeed * camFront * dt;
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_D))
			camPos += mSpeed * camRight * dt;
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_A))
			camPos -= mSpeed * camRight * dt;
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_S))
			camPos -= mSpeed * camFront * dt;
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_SPACE))
			camPos += mSpeed * camUp * dt;
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_LCTRL))
			camPos -= mSpeed * camUp * dt;
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_LSHIFT))
			mSpeed = 5.f;
		else
			mSpeed = 3.f;

		mCamObj->GetTransform()->SetWorldPos(camPos);
		//Compute how much to move the mouse and compute rotation in the X
		mouseRot.x = mSensitivity * (float)(mousePos.y - (size.y / 2)) / size.y;
		mouseRot.y = mSensitivity * (float)(mousePos.x - (size.x / 2)) / size.x;

		glm::vec3 newRot = glm::rotate(camRot, glm::radians(-mouseRot.x), glm::normalize(mCam->GetRight()));
		
		//If rotating, compute also the Y rotation with the new rotation
		//Otherwise, just update on the Y
		if (!(glm::angle(newRot, -mCam->GetUp()) <= glm::radians(5.f) || glm::angle(newRot, mCam->GetUp()) <= glm::radians(5.f)))
		{
			mCamObj->GetTransform()->SetWorldRot(newRot);
			newRot = glm::rotate(newRot, glm::radians(-mouseRot.y), mCam->GetUp());
		}
		else
			newRot = glm::rotate(camRot, glm::radians(-mouseRot.y), mCam->GetUp());
		
		//Set the new rotation and reset the mouse position
		mCamObj->GetTransform()->SetWorldRot(newRot);
		SDL_WarpMouseInWindow(gWindow->GetWindow(), size.x / 2, size.y / 2);
	}

	////Show mouse
	if (gInputMgr->IsMouseReleased(SDL_BUTTON_RIGHT))
		SDL_ShowCursor(true);
}

/// -----------------------------------------------------------------
/// Render scene in editor camera
/// -----------------------------------------------------------------
void Editor::RenderEditor::Render()
{
	vec2 size = gWindow->GetSize();
	glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
	glClearColor(0.07f, 0.13f, 0.17f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Get view and projection matrices (Temporal)
	glm::mat4 view = mCam->GetView();
	glm::mat4 proj = mCam->GetProj();

	Engine::Shader* currShader = nullptr;
	for (auto space : mRenderableScene)
	{
		for (auto rend : space.second)
		{
			//Activate shader and set matrices
			if (rend->GetShader() != currShader)
			{
				currShader = rend->GetShader();
				currShader->Activate();
				currShader->UniformMat4(proj, "proj");
				currShader->UniformMat4(view, "view");
			}
			rend->Render();
		}

		//Draw colliders in editor mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		currShader = gGfxMgr->GetShader("Default");
		currShader->Activate();
		currShader->UniformMat4(proj, "proj");
		currShader->UniformMat4(view, "view");
		for (auto collider : mColliderScene[space.first])
			if (collider->IsColliderDrawn())
				collider->Render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/// -----------------------------------------------------------------
/// Delete editor camera on shutdown
/// -----------------------------------------------------------------
void Editor::RenderEditor::Shutdown()
{
	mCamObj->Shutdown();
	delete mCamObj;
	mRenderableScene.clear();
	mColliderScene.clear();
}

/// -----------------------------------------------------------------
/// Adds renderable to editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::AddRenderable(Engine::Renderable* renderable)
{
	Engine::Space* space = renderable->GetOwner()->GetSpace();
	if (std::find(mRenderableScene[space].begin(), mRenderableScene[space].end(), renderable) == mRenderableScene[space].end())
		mRenderableScene[space].push_back(renderable);
}

/// -----------------------------------------------------------------
/// Removes renderable from editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::RemoveRenderable(Engine::Renderable* renderable)
{
	Engine::Space* space = renderable->GetOwner()->GetSpace();
	auto it = std::find(mRenderableScene[space].begin(), mRenderableScene[space].end(), renderable);
	if (it != mRenderableScene[space].end())
		mRenderableScene[space].erase(it);
}

/// -----------------------------------------------------------------
/// Adds collider to editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::AddCollider(Engine::Collider* collider)
{
	Engine::Space* space = collider->GetOwner()->GetSpace();
	if (std::find(mColliderScene[space].begin(), mColliderScene[space].end(), collider) == mColliderScene[space].end())
		mColliderScene[space].push_back(collider);
}

/// -----------------------------------------------------------------
/// Removes collider from editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::RemoveCollider(Engine::Collider* collider)
{
	Engine::Space* space = collider->GetOwner()->GetSpace();
	auto it = std::find(mColliderScene[space].begin(), mColliderScene[space].end(), collider);
	if (it != mColliderScene[space].end())
		mColliderScene[space].erase(it);
}
