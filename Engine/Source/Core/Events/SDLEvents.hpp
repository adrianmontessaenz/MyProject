#pragma once
#include <Core/Base/RunTime.hpp>
#include <Core/Base/Singleton.hpp>
#include <map>
#include <vector>
#include <SDL.h>

namespace Engine
{
	class SDLEventSystem : public RunTime, public Singleton<SDLEventSystem>
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Shutdown() override;

		const std::vector<SDL_Event>& GetEventsOfType(Uint32 type);

	private:
		std::map<Uint32, std::vector<SDL_Event>> mEvents;
	};
}
#define gSDLSys (&Engine::SDLEventSystem::GetInstance())