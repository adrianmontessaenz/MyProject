/*// -----------------------------------------------------------------
*  File:		DebugSystem.cpp
*  Brief:		Implementation of Engine Debug System
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "DebugSystem.hpp"

void Engine::DebugSystem::Log(const std::string msg, const std::string typeName)
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::cout << "Called at: " << std::ctime(&time);
	if (!typeName.empty())
		std::cout << typeName << " - ";
	std::cout << msg << std::endl << std::endl;
}
