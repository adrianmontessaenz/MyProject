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
	};
}