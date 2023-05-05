/*// -----------------------------------------------------------------
*  File:		EditObject.hpp
*  Brief:		Header of the object editor.
*  Creation:	11/12/2022
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>
#include <Graphics/Renderable/Renderable.hpp>
#include <Graphics/Camera/Camera.hpp>
#include <Physics/Components/Collider.hpp>
#include <Physics/Components/RigidBody.hpp>
#include <Sound/AudioEmitter.hpp>
#include <Sound/AudioListener.hpp>

namespace Editor
{
	class ObjectEditor
	{
	public:
		void Update();
		Engine::Object* GetSelectedObj() const;
		void SetSelectedObj(Engine::Object* obj);

	private:
		Engine::Object* mSelectedObj = nullptr;
		void EditPickedObject();
		bool ObjectProperties();

		bool ObjectEngineComponents();
		bool EditObjectEngineComp(const std::string& cmpName, Engine::EngineComp* cmp);

		//Components
		bool EditTransform(Engine::Transform* cmp);
		glm::vec3 TransformDisplayCoords(const glm::vec3& coords, const int& coordId, const float& min, const float& max);
		bool EditRenderable(Engine::Renderable* cmp);
		bool EditCamera(Engine::Camera* cmp);
		bool EditCollider(Engine::Collider* cmp);
		bool EditRigidBody(Engine::RigidBody* cmp);
		bool EditAudioEmitter(Engine::AudioEmitter* cmp);
		bool EditAudioListener(Engine::AudioListener* cmp);
	};
}