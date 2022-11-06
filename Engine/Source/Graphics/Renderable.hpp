/*// -----------------------------------------------------------------
*  File:		Renderable.hpp
*  Brief:		Header file of renderable component
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
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

		//For gfx manager
		void SetIndexOnManager(const int idx);
		const int GetIndexOnManager() const;
	private:
		int mIdx = -1;
		Model* mModel = new Model();
		Texture* mTexture = nullptr;
	};
}