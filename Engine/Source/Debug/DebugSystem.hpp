/*// -----------------------------------------------------------------
*  File:		DebugSystem.hpp
*  Brief:		Header of Engine Debug System
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class DebugSystem : public Singleton<DebugSystem>
	{
	public:
		void Log(const std::string msg, const std::string typeName = std::string());
	};
}
#define gDebugSys (&Engine::DebugSystem::GetInstance())