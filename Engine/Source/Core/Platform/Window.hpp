/*// -----------------------------------------------------------------
*  File:		main.cpp
*  Brief:		Header of Window class
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Core/Base/RunTime.hpp>
class SDL_Window;

namespace Engine
{
	class Window : public RunTime
	{
	public:
		Window();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Shutdown() override;

	private:
		SDL_Window* mWindow;
		unsigned mWidth;
		unsigned mHeight;
	};
}