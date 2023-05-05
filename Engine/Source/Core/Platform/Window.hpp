/*// -----------------------------------------------------------------
*  File:		Window.hpp
*  Brief:		Header of Window class
*  Creation:	13/10/2022
*  Last Update:	05/05/2023
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
		virtual void Initialize() override;
		virtual void Update() override;
		void Render();
		virtual void Shutdown() override;

		void SetWindowMode(WindowMode state);
		const WindowMode GetWindowMode() const;

		const glm::vec<2, int> GetSize() const;
		void SetSize(const glm::vec<2, int> new_size);

		SDL_Window* GetWindow();
		SDL_GLContext& GetContext();
		
		bool IsActive() const noexcept;

	private:
		bool mActive = true;
		SDL_Window* mWindow = nullptr;
		SDL_GLContext mContext = SDL_GLContext();
		SDL_DisplayMode mDisplay = SDL_DisplayMode();
		glm::vec<2, int> mSize = { 1280,720 };
		glm::vec<2, int> mPos = { 0,0 };
		WindowMode mMode = WindowMode::WINDOW_NORMAL;
		unsigned mDisplayIdx = 0;

		void UpdateWindowMode();
	};
}
#define gWindow (&Engine::Window::GetInstance())