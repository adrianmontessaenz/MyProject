/*// -----------------------------------------------------------------
*  File:		Serialized.cpp
*  Brief:		Implementation file for serialized class
*  Creation:	10/02/2023
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Serialized.hpp"

/* // ---------------------------------------------------------------------
*	Brief:		Open Json on given file name
*/ // ---------------------------------------------------------------------
const nlohmann::ordered_json Engine::Serialized::OpenJson(std::string fileName)
{
	//Open file
	std::ifstream fp(fileName);
	nlohmann::ordered_json data;
	if (fp.is_open() && fp.good())
	{
		fp >> std::setw(4) >> data;
		fp.close();
	}
	return data;
}

/* // ---------------------------------------------------------------------
*	Brief:		Saves json on given file name
*/ // ---------------------------------------------------------------------
void Engine::Serialized::SaveJson(std::string filename, const nlohmann::ordered_json& data)
{
	//Save to file
	std::ofstream fp(filename);
	if (fp.is_open() && fp.good())
	{
		fp << std::setw(4) << data;
		fp.close();
	}
}
