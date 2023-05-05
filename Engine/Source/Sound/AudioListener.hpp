/*// -----------------------------------------------------------------
*  File:		AudioListener.hpp
*  Brief:		Header for audio listener (not used right now)
*  Creation:	05/05/2023
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class AudioListener : public EngineComp
	{
		RTTI_BASE(AudioListener)
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Shutdown() override;

		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;

	private:
		bool mIsListener = false;
	};
}
