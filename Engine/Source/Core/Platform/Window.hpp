/*// -----------------------------------------------------------------
*  File:		Window.hpp
*  Brief:		Header of Window class
*  Creation:	13/10/2022
*  Last Update:	14/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Core/Base/RunTime.hpp>
#include <Core/Base/Singleton.hpp>

class SDL_Window;
class SDL_Renderer;

namespace Engine
{
	enum WindowMode
	{
		WINDOW_NORMAL = 0,
		WINDOW_BORDERLESS = 1,
		WINDOW_FULLSCREEN = 2
	};

	class Window : public RunTime, public Singleton<Window>
	{
	public:
		Window();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Shutdown() override;

		void SetWindowMode(WindowMode state);
		const WindowMode GetWindowMode() const;

	private:
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		unsigned mWidth;
		unsigned mHeight;
		WindowMode mMode;
	};
}
#define gWindow (&Engine::Window::GetInstance())