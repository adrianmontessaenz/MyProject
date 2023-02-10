/*// -----------------------------------------------------------------
*  File:		Renderable.hpp
*  Brief:		Header file of renderable component
*  Creation:	04/11/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Graphics/Model.hpp>

namespace Engine
{
	class Renderable : public EngineComp
	{
		RTTI_BASE(Renderable)
	public:
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void Shutdown() override;

		void SetModel(const std::string name);
		const std::string GetModel() const;
		void SetColor(const vec4 col);
		const vec4 GetColor() const;
		void SetTexture(const std::string name);
		const std::string GetTexture() const;
		void SetShader(Shader* shader);
		Shader* GetShader() const;

	private:
		Model* mModel = new Model();
		Texture* mTexture = nullptr;
		Shader* mShader = nullptr;
	};
}