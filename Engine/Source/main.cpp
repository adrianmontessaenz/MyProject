/*// -----------------------------------------------------------------
*  File:		main.cpp
*  Brief:		Main of engine (temporal)
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
* 
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <Core/Platform/Window.hpp>

int main(void)
{
	Engine::Window* wind = gWindow;
	wind->Initialize();

	while (wind->IsEnabled())
	{
		wind->Update();
		wind->LateUpdate();
	}

	wind->Shutdown();
	return 0;
}