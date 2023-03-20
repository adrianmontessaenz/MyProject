/*// -----------------------------------------------------------------
*  File:		EditScene.hpp
*  Brief:		Header of the editor scene manager.
*  Creation:	11/12/2022
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>

namespace Editor
{
	class SceneEditor
	{
	public:
		void Update();
		Engine::Object* GetSelectedObj() const;
		void SetSelectedObj(Engine::Object* obj);

	private:
		Engine::Object* mSelectedObj = nullptr;
		bool mAddChildren = false;
		float mOrderCd = 0.f;
		float mMaxCd = 0.5f;
		
		//Space editor
		bool DragAndDropSpaces(const size_t& idx, const size_t& max_size);
		bool VisualizeSpace(Engine::Space* space);
		bool SpaceProperties(Engine::Space* space);

		bool DragAndDropObjects(Engine::Space* space, Engine::Object* obj);
		bool ReOrderObjects(std::vector<Engine::Object*> objs_, Engine::Object* obj_, const size_t idx);
		bool VisualizeObject(Engine::Object* obj, const size_t idx);
		bool VisualizeChildren(Engine::Object* obj, const size_t idx);
	};
}