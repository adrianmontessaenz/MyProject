/*// -----------------------------------------------------------------
*  File:		GraphicsManager.cpp
*  Brief:		Implementation file of graphics manager
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "GraphicsManager.hpp"
#include <Graphics/Renderable.hpp>

/// -----------------------------------------------------------------
/// Initialize graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::Initialize()
{
	mShaders.push_back(new Shader("default.vert", "default.frag"));
}

/// -----------------------------------------------------------------
/// Render all renderables
/// -----------------------------------------------------------------
void Engine::GraphicsManager::Render()
{
	vec2 size = gWindow->GetSize();
	glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
	glClearColor(0.07f, 0.13f, 0.17f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Get view and projection matrices (Temporal)
	glm::mat4 view = glm::translate(glm::identity<mat4>(), glm::vec3(0.f, -0.5f, -2.f));
	glm::mat4 proj = glm::perspective(glm::radians(45.f), size.x / size.y, 0.1f, 100.f);

	//Activate shader and set matrices
	mShaders[0]->Activate();
	mShaders[0]->UniformMat4(proj, "proj");
	mShaders[0]->UniformMat4(view, "view");
	for (auto rend : mRenderables)
		rend->Render();
}

/// -----------------------------------------------------------------
/// Shutdown graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::Shutdown()
{
	mRenderables.clear();
	for (auto shader : mShaders)
		delete shader;
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
	int rendIdx = rend->GetIndexOnManager();
	if (rendIdx == -1)
	{
		mRenderables.push_back(rend);
		rend->SetIndexOnManager(static_cast<int>(mRenderables.size() - 1));
	}
}

/// -----------------------------------------------------------------
/// Removes renderable from graphics manager
/// -----------------------------------------------------------------
void Engine::GraphicsManager::RemoveRenderable(Renderable* rend)
{
	int rendIdx = rend->GetIndexOnManager();
	if (rendIdx != -1)
		mRenderables.erase(mRenderables.begin() + rend->GetIndexOnManager());
}
