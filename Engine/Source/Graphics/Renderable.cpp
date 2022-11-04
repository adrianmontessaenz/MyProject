/*// -----------------------------------------------------------------
*  File:		Renderable.cpp
*  Brief:		Implementation of renderable component
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Renderable.hpp"
#include <Core/Entity-Component/Object.hpp>
#include <Graphics/GraphicsManager.hpp>

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::Renderable::Initialize()
{
	RTTI::AddParentedType<Renderable, EngineComp>();
	gGfxMgr->AddRenderable(this);
}

/// -----------------------------------------------------------------
/// Renders renderable
/// -----------------------------------------------------------------
void Engine::Renderable::Render()
{
	//Get world matrix of object
	Transform* trans = GetOwner()->GetEngineComp<Transform>();
	glm::mat4 world = trans->GetWorldMat();

	//Add uniforms to shader and render
	Shader* shader = gGfxMgr->GetShader("default");
	shader->UniformMat4(world, "world");
	shader->UniformVec3(mModel->GetColor(), "col");
	mModel->Render();
}

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::Renderable::Shutdown()
{
	delete mModel;
	gGfxMgr->RemoveRenderable(this);
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Sets model of renderable
/// -----------------------------------------------------------------
void Engine::Renderable::SetModel(const std::string name)
{
	mModel->SetModel(name);
}

/// -----------------------------------------------------------------
/// Gets model name
/// -----------------------------------------------------------------
const std::string Engine::Renderable::GetModel() const
{
	return mModel->GetName();
}

/// -----------------------------------------------------------------
/// Sets model color
/// -----------------------------------------------------------------
void Engine::Renderable::SetColor(const vec3 col)
{
	mModel->SetColor(col);
}

/// -----------------------------------------------------------------
/// Gets model color
/// -----------------------------------------------------------------
const vec3 Engine::Renderable::GetColor() const
{
	return mModel->GetColor();
}

/// -----------------------------------------------------------------
/// Set index of renderable in manager
/// -----------------------------------------------------------------
void Engine::Renderable::SetIndexOnManager(const int idx)
{
	mIdx = idx;
}

/// -----------------------------------------------------------------
/// Get index of renderable in manager
/// -----------------------------------------------------------------
const int Engine::Renderable::GetIndexOnManager() const
{
	return mIdx;
}
