/*// -----------------------------------------------------------------
*  File:		EditScene.hpp
*  Brief:		Header of the editor scene manager.
*  Creation:	11/12/2022
*  Last Update:	11/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include <Core/Entity-Component/Object.hpp>

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
		
		//Space editor
		bool DragAndDropSpaces(const size_t& idx, const size_t& max_size);
		bool VisualizeSpace(Engine::Space* space);
		bool SpaceProperties(Engine::Space* space);

		bool DragAndDropObjects(const std::vector<Engine::Object*>& objs, const size_t& idx, const size_t& max_size);
		bool VisualizeObject(Engine::Object* obj);
	};
}