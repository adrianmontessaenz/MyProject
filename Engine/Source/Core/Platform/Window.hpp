/*// -----------------------------------------------------------------
*  File:		Window.hpp
*  Brief:		Header of Window class
*  Creation:	13/10/2022
*  Last Update:	14/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

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
		virtual void Render() override;
		virtual void Shutdown() override;

		void SetWindowMode(WindowMode state);
		const WindowMode GetWindowMode() const;

		const glm::vec<2, int> GetSize() const;
		void SetSize(const glm::vec<2, int> new_size);

	private:
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		glm::vec<2, int> mSize;
		glm::vec<2, int> mFullSize;
		WindowMode mMode;
		unsigned mDisplayIdx;

		void UpdateWindowMode();
	};
}
#define gWindow (&Engine::Window::GetInstance())