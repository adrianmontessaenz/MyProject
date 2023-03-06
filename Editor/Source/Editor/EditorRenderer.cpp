/*// -----------------------------------------------------------------
*  File:		EditorRenderer.cpp
*  Brief:		Implementation of editor renderer
*  Creation:	06/03/2023
*  Last Update:	06/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include "EditorRenderer.hpp"
#include <Core/Scene/ObjectManager.hpp>
#include <Core/Entity-Component/Object.hpp>
#include <Graphics/Shader.hpp>
#include <Core/Platform/InputManager.hpp>
#include "Editor.hpp"

/// -----------------------------------------------------------------
/// Initialize editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::Initialize() noexcept
{
	//Create camera
	mCamObj = new Engine::Object();
	mCam = mCamObj->AddEngineComp<Engine::Camera>();
	mCamObj->Initialize();

	//Get all renderables at the beginning (if scene was loaded)
	for (auto space : gObjMgr->GetSpaces())
	{
		for (auto obj : space->GetObjects())
		{
			if (auto cmp = obj->GetEngineComp<Engine::Renderable>())
				mScene[space].push_back(cmp);
		}
	}
}

/// -----------------------------------------------------------------
/// Update camera of editor
/// -----------------------------------------------------------------
void Editor::RenderEditor::Update() noexcept
{
	//Update object and check if focused in any window
	mCamObj->Update();
	if (gEditor->IsFocused())
	{
		std::cout << "false" << std::endl;
		return;
	}

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
		std::cout << "true" << std::endl;
		glm::vec2 mousePos = gInputMgr->GetMousePos();
		glm::vec2 mouseRot;

		//Handle camera movement
		glm::vec3 camPos = mCamObj->GetTransform()->GetWorldPos();
		glm::vec3 camRot = mCamObj->GetTransform()->GetWorldRot();
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_W))
			mCamObj->GetTransform()->SetWorldPos(camPos + mSpeed * camRot);
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_D))
			mCamObj->GetTransform()->SetWorldPos(camPos + mSpeed * glm::normalize(mCam->GetRight()));
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_A))
			mCamObj->GetTransform()->SetWorldPos(camPos - mSpeed * glm::normalize(mCam->GetRight()));
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_S))
			mCamObj->GetTransform()->SetWorldPos(camPos - mSpeed * camRot);
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_SPACE))
			mCamObj->GetTransform()->SetWorldPos(camPos + mSpeed * mCam->GetUp());
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_LCTRL))
			mCamObj->GetTransform()->SetWorldPos(camPos - mSpeed * mCam->GetUp());
		if (gInputMgr->IsKeyPressed(SDL_SCANCODE_LSHIFT))
			mSpeed += 0.05f;
		else
			mSpeed = 0.1f;

		//Compute how much to move the mouse and compute rotation in the X
		mouseRot.x = mSensitivity * (mousePos.y - (float)(size.y / 2.f)) / (float)size.y;
		mouseRot.y = mSensitivity * (mousePos.x - (float)(size.x / 2.f)) / (float)size.x;
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
void Editor::RenderEditor::Render() noexcept
{
	vec2 size = gWindow->GetSize();
	glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
	glClearColor(0.07f, 0.13f, 0.17f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Get view and projection matrices (Temporal)
	glm::mat4 view = mCam->GetView();
	glm::mat4 proj = mCam->GetProj();

	Engine::Shader* currShader = nullptr;
	for (auto space : mScene)
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
	}
}

/// -----------------------------------------------------------------
/// Delete editor camera on shutdown
/// -----------------------------------------------------------------
void Editor::RenderEditor::Shutdown() noexcept
{
	mCamObj->Shutdown();
	delete mCamObj;
}

/// -----------------------------------------------------------------
/// Adds renderable to editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::AddRenderable(Engine::Renderable* renderable)
{
	Engine::Space* space = renderable->GetOwner()->GetSpace();
	if (std::find(mScene[space].begin(), mScene[space].end(), renderable) == mScene[space].end())
		mScene[space].push_back(renderable);
}

/// -----------------------------------------------------------------
/// Removes renderable from editor renderer
/// -----------------------------------------------------------------
void Editor::RenderEditor::RemoveRenderable(Engine::Renderable* renderable)
{
	Engine::Space* space = renderable->GetOwner()->GetSpace();
	auto it = std::find(mScene[space].begin(), mScene[space].end(), renderable);
	if (it != mScene[space].end())
		mScene[space].erase(it);
}
