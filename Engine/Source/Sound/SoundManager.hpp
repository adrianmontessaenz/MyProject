/*// -----------------------------------------------------------------
*  File:		SoundManager.hpp
*  Brief:		Header for sound manager
*  Creation:	05/05/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include "AudioListener.hpp"

namespace Engine
{
	class SoundManager : public Singleton<SoundManager>, public RunTime
	{
		RTTI_BASE(SoundManager)
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		FMOD::Channel* PlaySound(const std::string soundName, FMOD::Sound* sound, bool isMusic);

		float GetMainVolume() const;
		void SetMainVolume(const float value);
		bool IsMainVolumeMuted() const;
		void SetMainVolumeMuted(const bool muted);
		float GetSoundVolume() const;
		void SetSoundVolume(const float value);
		bool IsSoundVolumeMuted() const;
		void SetSoundVolumeMuted(const bool muted);
		float GetMusicVolume() const;
		void SetMusicVolume(const float value);
		bool IsMusicVolumeMuted() const;
		void SetMusicVolumeMuted(const bool muted);

		void AddAudioListener(AudioListener* listener);
		void RemoveAudioListener();
		AudioListener* GetAudioListener() const;

	private:
		FMOD::System* mSystem = nullptr;
		std::vector<std::pair<bool, FMOD::Channel*>> mSoundChannels;
		std::vector<std::pair<bool, FMOD::Channel*>> mMusicChannels;
		AudioListener* mListener = nullptr;

		//Volume settings
		float mMainVolume = 1.f;
		bool mMainVolumeMuted = false;
		float mSoundVolume = 1.f;
		bool mSoundVolumeMuted = false;
		float mMusicVolume = 1.f;
		bool mMusicVolumeMuted = false;
	};
}
#define gSndMgr (&Engine::SoundManager::GetInstance())
