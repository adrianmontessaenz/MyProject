/*// -----------------------------------------------------------------
*  File:		Renderable.cpp
*  Brief:		Implementation of renderable component
*  Creation:	04/11/2022
*  Last Update:	07/11/2022
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
	//Don't update if disabled or shutdown
	if (!IsEnabled() || IsShutdown())
		return;

	//Get world matrix of object
	Transform* trans = GetOwner()->GetEngineComp<Transform>();
	glm::mat4 world = trans->GetWorldMat();

	//Add uniforms to shader and render
	Shader* shader = gGfxMgr->GetShader("default");
	shader->UniformMat4(world, "world");
	shader->UniformVec4(mModel->GetColor(), "col");
	if (mTexture)
	{
		shader->UniformInt(1, "hasTexture");
		mTexture->Bind();
		shader->UniformInt(mTexture->GetType(), "texImage");
	}
	mModel->Render();
}

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::Renderable::Shutdown()
{
	delete mModel;
	if(mTexture)
		delete mTexture;
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
void Engine::Renderable::SetColor(const vec4 col)
{
	mModel->SetColor(col);
}

/// -----------------------------------------------------------------
/// Gets model color
/// -----------------------------------------------------------------
const vec4 Engine::Renderable::GetColor() const
{
	return mModel->GetColor();
}

/// -----------------------------------------------------------------
/// Sets texture to renderable
/// -----------------------------------------------------------------
void Engine::Renderable::SetTexture(const std::string name)
{
	if (mTexture != nullptr)
	{
		delete mTexture;
		mTexture = nullptr;
	}
	if (name != "none")
		mTexture = new Texture(name, 0);
}

/// -----------------------------------------------------------------
/// Gets texture of renderable
/// -----------------------------------------------------------------
const std::string Engine::Renderable::GetTexture() const
{
	return mTexture->GetName();
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
