/*// -----------------------------------------------------------------
*  File:		AudioListener.cpp
*  Brief:		Implementation for audio listener (not used right now)
*  Creation:	05/05/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "AudioListener.hpp"
#include <Sound/SoundManager.hpp>

/// -----------------------------------------------------------------
/// Initialize audio listener
/// -----------------------------------------------------------------
void Engine::AudioListener::Initialize()
{
	RTTI::AddParentedType<AudioListener, EngineComp>();
	if (gSndMgr->GetAudioListener())
	{
		GetOwner()->DeleteEngineComp<AudioListener>();
		return;
	}
	gSndMgr->AddAudioListener(this);
	mIsListener = true;
}

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::AudioListener::Update()
{
}

/// -----------------------------------------------------------------
/// Shutdown audio listener
/// -----------------------------------------------------------------
void Engine::AudioListener::Shutdown()
{
	if (gSndMgr->GetAudioListener() == this)
		gSndMgr->RemoveAudioListener();
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::AudioListener::ToJson(nlohmann::ordered_json& data)
{
}

/// -----------------------------------------------------------------
/// Empty for the moment
/// -----------------------------------------------------------------
void Engine::AudioListener::FromJson(const nlohmann::ordered_json& data)
{
}
