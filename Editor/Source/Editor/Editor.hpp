/*// -----------------------------------------------------------------
*  File:		Editor.hpp
*  Brief:		Header of the editor.
*  Creation:	17/11/2022
*  Last Update:	10/12/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>
#include <Core/Entity-Component/Object.hpp>

namespace Editor
{
	class Editor : public Engine::RunTime, public Engine::Singleton<Editor>
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Shutdown() override;

	private:
		Engine::Object* mSelectedObj = nullptr;
		void VisualizeObject(Engine::Object* obj, const unsigned idx);
		void EditPickedObject();
	};
}
#define gEditor (&Editor::Editor::GetInstance())