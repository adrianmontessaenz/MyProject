/*// -----------------------------------------------------------------
*  File:		EditObject.hpp
*  Brief:		Header of the object editor.
*  Creation:	11/12/2022
*  Last Update:	11/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>
#include <Core/Entity-Component/Object.hpp>
#include <Graphics/Renderable.hpp>

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
		Engine::EngineComp* GetObjectEngineComp(const std::string compName);
		bool EditObjectEngineComp(const std::string& cmpName, Engine::EngineComp* cmp);

		//Components
		bool EditTransform(Engine::Transform* cmp);
		void TransformDisplayCoords(glm::vec3& coords, const int& coordId, const float& min, const float& max);
		bool EditRenderable(Engine::Renderable* cmp);
	};
}