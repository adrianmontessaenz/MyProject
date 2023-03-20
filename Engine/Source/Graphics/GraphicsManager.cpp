/*// -----------------------------------------------------------------
*  File:		GraphicsManager.cpp
*  Brief:		Implementation file of graphics manager
*  Creation:	04/11/2022
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "GraphicsManager.hpp"
#include <Graphics/Renderable/Renderable.hpp>
#include <Graphics/Camera/Camera.hpp>

/// -----------------------------------------------------------------
/// Initialize graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::Initialize()
{
	RTTI::AddParentedType<Renderable, EngineComp>();

	//Add all renderables of initial scene
	auto spaces = gObjMgr->GetSpaces();
	for (auto space : spaces)
	{
		auto objs = space->GetObjects();
		for (auto obj : objs)
		{
			auto rend = obj->GetEngineComp<Renderable>();
			if (rend)
				AddRenderable(rend);
		}
	}

	//Add shaders
	mShaders.push_back(new Shader("default.vs", "default.fs"));
	mShaders.back()->SetName("Default");
}

/// -----------------------------------------------------------------
/// Render all renderables
/// -----------------------------------------------------------------
void Engine::GraphicsManager::Render()
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

	Shader* currShader = nullptr;
	for (auto space : mRenderables)
	{
		//Get view and projection matrices (Temporal)
		Camera* cam = space.first->GetObjectByName("Main Camera")->GetEngineComp<Camera>();
		glm::mat4 view = cam->GetView();
		glm::mat4 proj = cam->GetProj();

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
/// Shutdown graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::Shutdown()
{
	mRenderables.clear();
	for (auto shader : mShaders)
		delete shader;
	mShaders.clear();
	mShaders.shrink_to_fit();
}

/// -----------------------------------------------------------------
/// Gets shader of given name
/// -----------------------------------------------------------------
Engine::Shader* Engine::GraphicsManager::GetShader(const std::string name)
{
	for (auto shader : mShaders)
	{
		if (strcmp(name.c_str(), shader->GetName().c_str()) == 0)
			return shader;
	}
	return nullptr;
}

/// -----------------------------------------------------------------
/// Adds renderable to graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::AddRenderable(Renderable* rend)
{
	//No owner == for typeinfo
	if (rend->GetOwner() == nullptr)
		return;
	mRenderables[rend->GetOwner()->GetSpace()].push_back(rend);
}

/// -----------------------------------------------------------------
/// Removes renderable from graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::RemoveRenderable(Renderable* rend)
{
	Space* space = rend->GetOwner()->GetSpace();
	auto it = std::find(mRenderables[space].begin(), mRenderables[space].end(), rend);
	if (it != mRenderables[space].end())
	{
		mRenderables[space].erase(it);
		if (mRenderables[space].empty())
			mRenderables.erase(space);
	}
}
