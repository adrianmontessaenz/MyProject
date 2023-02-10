/*// -----------------------------------------------------------------
*  File:		Identity.hpp
*  Brief:		Header of Identity class
*  Creation:	21/10/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
namespace Engine
{
	class Identity
	{
	public:
		Identity();
		~Identity();
		//Name
		void SetName(const std::string name);
		std::string GetName() const;

		//UniqueID
		const unsigned GetUniqueID() const;

	private:
		std::string mName;
		unsigned mUID;
		static unsigned sNextId;
		static std::stack<unsigned> sUnusedID;
	};
}