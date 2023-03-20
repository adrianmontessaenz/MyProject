/*// -----------------------------------------------------------------
*  File:		GraphicsManager.hpp
*  Brief:		Header file of graphics manager
*  Creation:	04/11/2022
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

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
	};
}
#define gGfxMgr (&Engine::GraphicsManager::GetInstance())
