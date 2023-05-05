/*// -----------------------------------------------------------------
*  File:		AudioEmitter.hpp
*  Brief:		Header for audio emitter
*  Creation:	05/05/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class AudioEmitter : public EngineComp
	{
		RTTI_BASE(AudioEmitter)
	public:
		enum AudioEmitterTypes
		{
			AUDIO_MUSIC = 0,
			AUDIO_SOUND = 1,
			AUDIO_DEFAULT = 2
		};

		//Runtime
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Shutdown() override;

		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;

		//Sound name
		void SetSoundName(const std::string name);
		const std::string GetSoundName() const;
		FMOD::Sound* GetSound() const;

		//Sound Playing
		void PlaySound();
		bool IsSoundPlaying() const;
		void StopSound();
		void DeleteSound();

		//Sound Properties
		void SetSoundPaused(const bool paused);
		bool IsSoundPaused() const;
		void SetLooping(const bool loop);
		bool IsLooping() const;
		void SetAudioType(const AudioEmitterTypes type);
		void SetVolume(const float volume);
		float GetVolume() const;
		AudioEmitterTypes GetAudioType() const;

	private:
		FMOD::Channel* mChannel = nullptr;
		FMOD::Sound* mSound = nullptr;
		std::string mSoundName;
		bool mPaused = false;
		bool mLooping = false;
		float mVolume = 1.f;
		AudioEmitterTypes mType = AudioEmitterTypes::AUDIO_DEFAULT;

		void UpdateVolume();
	};
}
