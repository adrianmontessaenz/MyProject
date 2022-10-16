/*// -----------------------------------------------------------------
*  File:		InputManager.hpp
*  Brief:		Header of Engine Input Manager
*  Creation:	16/10/2022
*  Last Update:	16/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class InputManager : public RunTime, public Singleton<InputManager>
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Shutdown() override;

		//Keyboard
		bool IsKeyTriggered(SDL_Scancode key);
		bool IsKeyPressed(SDL_Scancode key);
		bool IsKeyReleased(SDL_Scancode key);

		//Mouse
		bool IsMouseTriggered(Uint8 mouse);
		bool IsMousePressed(Uint8 mouse);
		bool IsMouseReleased(Uint8 mouse);

		const glm::vec<2, int> GetMousePos() const;

	private:
		//Keyboard
		std::map<SDL_Scancode, float> mKeyboard;
		std::map<SDL_Scancode, float> mPrevKeyboard;

		//Mouse
		std::map<Uint8, float> mMouse;
		std::map<Uint8, float> mPrevMouse;
		glm::vec<2, int> mMousePos;

	};
}
#define gInputMgr (&Engine::InputManager::GetInstance())
