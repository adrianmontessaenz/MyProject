#pragma once
#include <Core/Base/RunTime.hpp>
#include <SDL.h>
#include <map>
#include <vector>

namespace Engine
{
	class SDLEventSystem : public RunTime
	{
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Shutdown() override;

	private:
		std::map<Uint32, std::vector<SDL_Event>> mEvents;
	};
}