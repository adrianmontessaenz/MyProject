/*// -----------------------------------------------------------------
*  File:		SoundManager.cpp
*  Brief:		Implementation of sound manager
*  Creation:	05/05/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "SoundManager.hpp"
#include "AudioEmitter.hpp"

/// -----------------------------------------------------------------
/// Sound manager initialization
/// -----------------------------------------------------------------
void Engine::SoundManager::Initialize()
{
	RTTI::AddParentedType<AudioEmitter, EngineComp>();
	RTTI::AddParentedType<AudioListener, EngineComp>();

	//Initialize FMOD systems
	FMOD_RESULT result = FMOD::System_Create(&mSystem);
	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));

	result = mSystem->init(36, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));

	//Set 4 music channels and 32 sound channels
	mSoundChannels.resize(32);
	mMusicChannels.resize(4);
}

/// -----------------------------------------------------------------
/// Sound manager update
/// -----------------------------------------------------------------
void Engine::SoundManager::Update()
{
	if (mListener == nullptr)
		return;

	//Update if channels are free or not
	size_t channelCount = mSoundChannels.size();
	for (size_t idx = 0; idx < channelCount; idx++)
		mSoundChannels[idx].second->isPlaying(&mSoundChannels[idx].first);

	channelCount = mMusicChannels.size();
	for (size_t idx = 0; idx < channelCount; idx++)
		mMusicChannels[idx].second->isPlaying(&mMusicChannels[idx].first);
}

/// -----------------------------------------------------------------
/// Sound manager shutdown
/// -----------------------------------------------------------------
void Engine::SoundManager::Shutdown()
{
	//Shutdown FMOD systems
	FMOD_RESULT result = mSystem->close();
	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));

	result = mSystem->release();
	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));
}

/// -----------------------------------------------------------------
/// Creates sound or music on first free channel
/// -----------------------------------------------------------------
FMOD::Channel* Engine::SoundManager::PlaySound(const std::string soundName, FMOD::Sound* sound, const bool isMusic)
{
	//No listener no sound on application
	if (mListener == nullptr)
		return nullptr;

	//Get first free sound channel. If not, return nullptr
	size_t channelCount = isMusic ? mMusicChannels.size() : mSoundChannels.size();
	size_t idx = 0;
	FMOD::Channel* freeChannel = nullptr;
	for (idx = 0; idx < channelCount; idx++)
	{
		if (!isMusic && mSoundChannels[idx].first == false)
		{
			freeChannel = mSoundChannels[idx].second;
			break;
		}
		else if(mMusicChannels[idx].first == false)
		{
			freeChannel = mMusicChannels[idx].second;
			break;
		}
	}
	if(idx == channelCount)
		return freeChannel;


	//Create sound with name and return channel
	std::string name = isMusic ? "../Data/Audio/Music/" : "../Data/Audio/Sounds/";
	name += soundName;
	mSystem->createSound(name.c_str(), FMOD_DEFAULT, 0, &sound);
	mSystem->playSound(sound, 0, false, &freeChannel);
	return freeChannel;
}

/// -----------------------------------------------------------------
/// Get main volume of application
/// -----------------------------------------------------------------
float Engine::SoundManager::GetMainVolume() const
{
	return mMainVolume;
}

/// -----------------------------------------------------------------
/// Set main volume of application
/// -----------------------------------------------------------------
void Engine::SoundManager::SetMainVolume(const float value)
{
	mMainVolume = value;
}

/// -----------------------------------------------------------------
/// Check if main Volume Muted
/// -----------------------------------------------------------------
bool Engine::SoundManager::IsMainVolumeMuted() const
{
	return mMainVolumeMuted;
}

/// -----------------------------------------------------------------
/// Set Main Volume Muted
/// -----------------------------------------------------------------
void Engine::SoundManager::SetMainVolumeMuted(const bool muted)
{
	mMainVolumeMuted = muted;
}

/// -----------------------------------------------------------------
/// Get sound volume of application
/// -----------------------------------------------------------------
float Engine::SoundManager::GetSoundVolume() const
{
	return mSoundVolume;
}

/// -----------------------------------------------------------------
/// Set sound volume of application
/// -----------------------------------------------------------------
void Engine::SoundManager::SetSoundVolume(const float value)
{
	mSoundVolume = value;
}

/// -----------------------------------------------------------------
/// Check if Sound Volume Muted
/// -----------------------------------------------------------------
bool Engine::SoundManager::IsSoundVolumeMuted() const
{
	return mSoundVolumeMuted;
}

/// -----------------------------------------------------------------
/// Set Sound Volume Muted
/// -----------------------------------------------------------------
void Engine::SoundManager::SetSoundVolumeMuted(const bool muted)
{
	mSoundVolumeMuted = muted;
}

/// -----------------------------------------------------------------
/// Get music volume of application
/// -----------------------------------------------------------------
float Engine::SoundManager::GetMusicVolume() const
{
	return mMusicVolume;
}

/// -----------------------------------------------------------------
/// Set music volume of application
/// -----------------------------------------------------------------
void Engine::SoundManager::SetMusicVolume(const float value)
{
	mMusicVolume = value;
}

/// -----------------------------------------------------------------
/// Check if Music Volume Muted
/// -----------------------------------------------------------------
bool Engine::SoundManager::IsMusicVolumeMuted() const
{
	return mMusicVolumeMuted;
}

/// -----------------------------------------------------------------
/// Set Music Volume Muted
/// -----------------------------------------------------------------
void Engine::SoundManager::SetMusicVolumeMuted(const bool muted)
{
	mMusicVolumeMuted = muted;
}

/// -----------------------------------------------------------------
/// Set audio listener
/// -----------------------------------------------------------------
void Engine::SoundManager::AddAudioListener(AudioListener* listener)
{
	if (mListener)
		return;
	mListener = listener;
}

/// -----------------------------------------------------------------
/// Remove audio listener
/// -----------------------------------------------------------------
void Engine::SoundManager::RemoveAudioListener()
{
	mListener = nullptr;
}

/// -----------------------------------------------------------------
/// Get audio listener
/// -----------------------------------------------------------------
Engine::AudioListener* Engine::SoundManager::GetAudioListener() const
{
	return mListener;
}
