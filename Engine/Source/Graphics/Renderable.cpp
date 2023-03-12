/*// -----------------------------------------------------------------
*  File:		Renderable.cpp
*  Brief:		Implementation of renderable component
*  Creation:	04/11/2022
*  Last Update:	05/03/2023
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
	mShader = gGfxMgr->GetShader("Default");
}

/// -----------------------------------------------------------------
/// Renders renderable
/// -----------------------------------------------------------------
void Engine::Renderable::Render()
{
	//Don't update if disabled or shutdown
	if (!IsActive() || !GetOwner()->IsEnabled())
		return;

	//Add uniforms to shader and render
	mShader->UniformMat4(GetOwner()->GetTransform()->GetWorldMat(), "world");
	mShader->UniformVec4(mModel->GetColor(), "col");
	mTexture->Bind();
	mShader->UniformInt(mTexture->GetType(), "texImage");
	mModel->Render();
}

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::Renderable::Shutdown()
{
	delete mModel;
	delete mTexture;
	gGfxMgr->RemoveRenderable(this);
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Write to file
/// -----------------------------------------------------------------
void Engine::Renderable::ToJson(nlohmann::ordered_json& data)
{
	data["Component"] = TypeInfo()->GetTypeName();
	data["Model"] = mModel->GetName();
	data["Texture"] = mTexture->GetName();
	data["Color"] << mModel->GetColor();
}

/// -----------------------------------------------------------------
/// Read from file
/// -----------------------------------------------------------------
void Engine::Renderable::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Model") != data.end())
		SetModel(data["Model"]);
	if (data.find("Texture") != data.end())
		SetTexture(data["Texture"]);
	if (data.find("Color") != data.end())
	{
		glm::vec4 color;
		data["Color"] >> color;
		mModel->SetColor(color);
	}
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
	if (mTexture->GetName() == name)
		return;
	delete mTexture;
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
/// Sets shader for renderable
/// -----------------------------------------------------------------
void Engine::Renderable::SetShader(Shader* shader)
{
	mShader = shader;
}

/// -----------------------------------------------------------------
/// Gets shader of renderable
/// -----------------------------------------------------------------
Engine::Shader* Engine::Renderable::GetShader() const
{
	return mShader;
}
