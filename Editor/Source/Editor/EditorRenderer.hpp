/*// -----------------------------------------------------------------
*  File:		EditorRenderer.hpp
*  Brief:		Header for editor renderer
*  Creation:	06/03/2023
*  Last Update:	06/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>
#include <Core/Scene/Space.hpp>
#include <Graphics/Renderable.hpp>
#include <Graphics/Camera/Camera.hpp>

namespace Editor
{
	class RenderEditor : public Engine::RunTime, public Engine::Singleton<RenderEditor>
	{
	public:
		virtual void Initialize() noexcept override;
		virtual void Update() noexcept override;
		virtual void Render() noexcept override;
		virtual void Shutdown() noexcept override;

		void AddRenderable(Engine::Renderable* renderable);
		void RemoveRenderable(Engine::Renderable* renderable);

	private:
		std::unordered_map<Engine::Space*, std::vector<Engine::Renderable*>> mScene;

		//Camera
		Engine::Object* mCamObj = nullptr;
		Engine::Camera* mCam = nullptr;
		float mSpeed = 0.1f;
		float mSensitivity = 100.f;
	};
}
#define gRenderEditor (&Editor::RenderEditor::GetInstance())
