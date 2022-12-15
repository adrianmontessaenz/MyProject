/*// -----------------------------------------------------------------
*  File:		GraphicsManager.hpp
*  Brief:		Header file of graphics manager
*  Creation:	04/11/2022
*  Last Update:	15/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Core/Scene/Space.hpp>

namespace Engine
{
	class Renderable;
	class GraphicsManager : public RunTime, public Singleton<GraphicsManager>
	{
	public:
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void Shutdown() override;

		Shader* GetShader(const std::string name);
		void AddRenderable(Renderable* rend);
		void RemoveRenderable(Renderable* rend);

	private:
		std::vector<Shader*> mShaders;
		std::map<Space*, std::vector<Renderable*>> mRenderables;

		void UpdateRendIdx(Engine::Space* space, const int idx_);
	};
}
#define gGfxMgr (&Engine::GraphicsManager::GetInstance())
