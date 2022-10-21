/*// -----------------------------------------------------------------
*  File:		Identity.hpp
*  Brief:		Header of Identity class
*  Creation:	21/10/2022
*  Last Update:	21/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
namespace Engine
{
	class Identity
	{
	public:
		//Name
		void SetName(const std::string name);
		std::string GetName() const;

		//UniqueID
		void SetUniqueID(const unsigned id);
		const unsigned GetUniqueID() const;

	private:
		char* mName;
		unsigned mUID;
	};
}