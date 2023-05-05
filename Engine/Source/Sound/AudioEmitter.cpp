/*// -----------------------------------------------------------------
*  File:		AudioEmitter.cpp
*  Brief:		Implementation for audio emitter
*  Creation:	05/05/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include <Sound/SoundManager.hpp>
#include "AudioEmitter.hpp"

/// -----------------------------------------------------------------
/// Empty initialize
/// -----------------------------------------------------------------
void Engine::AudioEmitter::Initialize()
{
	RTTI::AddParentedType<AudioEmitter, EngineComp>();
}

/// -----------------------------------------------------------------
/// Emitter update
/// -----------------------------------------------------------------
void Engine::AudioEmitter::Update()
{
	//If no channel, no update
	if (mChannel == nullptr)
		return;

	//If channel stopped sound, remove from audio source
	if (!IsSoundPlaying() || gSndMgr->GetAudioListener() == nullptr)
		StopSound();
	UpdateVolume();
}

/// -----------------------------------------------------------------
/// Emitter shutdown
/// -----------------------------------------------------------------
void Engine::AudioEmitter::Shutdown()
{
	StopSound();
	SetShutdown(true);
}

/// -----------------------------------------------------------------
/// Save emitter properties to json
/// -----------------------------------------------------------------
void Engine::AudioEmitter::ToJson(nlohmann::ordered_json& data)
{
	data["Sound"] = mSoundName;
	data["Volume"] = mVolume;
	data["Looping"] = mLooping;
	data["Paused"] = mPaused;
}

/// -----------------------------------------------------------------
/// Load emitter properties from json
/// -----------------------------------------------------------------
void Engine::AudioEmitter::FromJson(const nlohmann::ordered_json& data)
{
	if (data.find("Sound") != data.end())
		mSoundName = data["Sound"];
	if (data.find("Volume") != data.end())
		mVolume = data["Volume"];
	if (data.find("Looping") != data.end())
		mLooping = data["Looping"];
	if (data.find("Paused") != data.end())
		mPaused = data["Paused"];
}

/// -----------------------------------------------------------------
/// Set emitter sound name
/// -----------------------------------------------------------------
void Engine::AudioEmitter::SetSoundName(const std::string name)
{
	mSoundName = name;
}

/// -----------------------------------------------------------------
/// Get emitter sound name
/// -----------------------------------------------------------------
const std::string Engine::AudioEmitter::GetSoundName() const
{
	return mSoundName;
}

/// -----------------------------------------------------------------
/// Get emitter sound
/// -----------------------------------------------------------------
FMOD::Sound* Engine::AudioEmitter::GetSound() const
{
	return mSound;
}

/// -----------------------------------------------------------------
/// Play emitter sound
/// -----------------------------------------------------------------
void Engine::AudioEmitter::PlaySound()
{
	StopSound();
	mChannel = gSndMgr->PlaySound(mSoundName, mSound, mType == AUDIO_MUSIC);
	if (mChannel)
	{
		mChannel->setPaused(mPaused);
		mChannel->setMode(mLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}
}

/// -----------------------------------------------------------------
/// Check if emitter sound is playing
/// -----------------------------------------------------------------
bool Engine::AudioEmitter::IsSoundPlaying() const
{
	bool isPlaying;
	mChannel->isPlaying(&isPlaying);
	return isPlaying;
}

/// -----------------------------------------------------------------
/// Stop emitter sound
/// -----------------------------------------------------------------
void Engine::AudioEmitter::StopSound()
{
	if (mChannel)
	{
		mChannel->stop();
		mChannel = nullptr;
	}
	DeleteSound();
}

/// -----------------------------------------------------------------
/// Delete emitter sound
/// -----------------------------------------------------------------
void Engine::AudioEmitter::DeleteSound()
{
	if (mSound)
	{
		FMOD_RESULT result = mSound->release();
		if (result != FMOD_OK)
			throw std::runtime_error(FMOD_ErrorString(result));
	}
}

/// -----------------------------------------------------------------
/// Set emitter paused
/// -----------------------------------------------------------------
void Engine::AudioEmitter::SetSoundPaused(const bool paused)
{
	mPaused = paused;
	if (mChannel)
		mChannel->setPaused(mPaused);
}

/// -----------------------------------------------------------------
/// Check if emitter paused
/// -----------------------------------------------------------------
bool Engine::AudioEmitter::IsSoundPaused() const
{
	return mPaused;
}

/// -----------------------------------------------------------------
/// Set emitter looping
/// -----------------------------------------------------------------
void Engine::AudioEmitter::SetLooping(const bool loop)
{
	mLooping = loop;
	if(mChannel)
		mChannel->setMode(mLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

/// -----------------------------------------------------------------
/// Check if emitter looping
/// -----------------------------------------------------------------
bool Engine::AudioEmitter::IsLooping() const
{
	return mLooping;
}

/// -----------------------------------------------------------------
/// Set emitter volume
/// -----------------------------------------------------------------
void Engine::AudioEmitter::SetVolume(const float volume)
{
	mVolume = volume;
	if (mChannel)
		UpdateVolume();
}

/// -----------------------------------------------------------------
/// Get emitter volume
/// -----------------------------------------------------------------
float Engine::AudioEmitter::GetVolume() const
{
	return mVolume;
}

/// -----------------------------------------------------------------
/// Set emitter audio type
/// -----------------------------------------------------------------
void Engine::AudioEmitter::SetAudioType(const AudioEmitterTypes type)
{
	mType = type;
	if(mChannel)
		UpdateVolume();
}

/// -----------------------------------------------------------------
/// Get emitter audio type
/// -----------------------------------------------------------------
Engine::AudioEmitter::AudioEmitterTypes Engine::AudioEmitter::GetAudioType() const
{
	return mType;
}

/// -----------------------------------------------------------------
/// Update emitter volume
/// -----------------------------------------------------------------
void Engine::AudioEmitter::UpdateVolume()
{
	//Set volume depending on type
	float volume = mVolume;
	if (gSndMgr->IsMainVolumeMuted())
		volume = 0.f;
	else
	{
		switch (mType)
		{
		case AUDIO_DEFAULT:
			volume *= gSndMgr->GetMainVolume();
			break;
		case AUDIO_MUSIC:
			volume *= gSndMgr->IsMusicVolumeMuted() ? 0.f : gSndMgr->GetMainVolume() * gSndMgr->GetMusicVolume();
			break;
		case AUDIO_SOUND:
			volume *= gSndMgr->IsSoundVolumeMuted() ? 0.f : gSndMgr->GetMainVolume() * gSndMgr->GetSoundVolume();
			break;
		}
	}
	mChannel->setVolume(volume);
}
