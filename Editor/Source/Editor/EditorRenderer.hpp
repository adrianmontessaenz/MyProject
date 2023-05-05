/*// -----------------------------------------------------------------
*  File:		EditorRenderer.hpp
*  Brief:		Header for editor renderer
*  Creation:	06/03/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>
#include <Graphics/Renderable/Renderable.hpp>
#include <Graphics/Camera/Camera.hpp>
#include <Physics/Components/Collider.hpp>

namespace Editor
{
	class RenderEditor : public Engine::RunTime, public Engine::Singleton<RenderEditor>
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		void Render();
		virtual void Shutdown() override;

		void AddRenderable(Engine::Renderable* renderable);
		void RemoveRenderable(Engine::Renderable* renderable);

		void AddCollider(Engine::Collider* collider);
		void RemoveCollider(Engine::Collider* collider);

	private:
		std::unordered_map<Engine::Space*, std::vector<Engine::Renderable*>> mRenderableScene;
		std::unordered_map<Engine::Space*, std::vector<Engine::Collider*>> mColliderScene;

		//Camera
		Engine::Object* mCamObj = nullptr;
		Engine::Camera* mCam = nullptr;
		float mSpeed = 0.1f;
		float mSensitivity = 100.f;
	};
}
#define gRenderEditor (&Editor::RenderEditor::GetInstance())
