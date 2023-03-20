/*// -----------------------------------------------------------------
*  File:		Editor.hpp
*  Brief:		Header of the editor.
*  Creation:	17/11/2022
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <pch.h>
#include "Editor/EditScene.hpp"
#include "Editor/EditObject.hpp"

namespace Editor
{
	class EditorClass : public Engine::RunTime, public Engine::Singleton<EditorClass>
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void Shutdown() override;

		void SetPlaying(const bool play);
		bool IsPlaying() const;
		void SetFocused(const bool focus);
		bool IsFocused() const;

	private:
		//Editor helpers
		SceneEditor mSceneEditor;
		ObjectEditor mObjectEditor;		
		bool mPlaying = false;
		bool mFocused = false;
	};
}
#define gEditor (&Editor::EditorClass::GetInstance())