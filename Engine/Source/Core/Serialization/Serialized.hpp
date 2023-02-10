/*// -----------------------------------------------------------------
*  File:		Serialized.hpp
*  Brief:		Header file for serialized class
*  Creation:	10/02/2023
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Serialized
	{
		RTTI_BASE(Serialized)
	public:
		virtual void ToJson(nlohmann::ordered_json& data) = 0;
		virtual void FromJson(const nlohmann::ordered_json& data) = 0;

		const nlohmann::ordered_json OpenJson(std::string filename);
		void SaveJson(std::string filename, const nlohmann::ordered_json& data);
	protected:
		Serialized() = default;
	};
}
#include "Serialized.inl"