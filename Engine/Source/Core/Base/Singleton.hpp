/*// -----------------------------------------------------------------
*  File:		Singleton.hpp
*  Brief:		Header of singleton class
*  Creation:	13/10/2022
*  Last Update:	13/10/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	template<typename Type>
	class Singleton
	{
	public:
		//No copies
		Singleton(Singleton& other) = delete;
		Singleton& operator=(Singleton& other) = delete;

		static Type& GetInstance();

	protected:
		Singleton() = default;
	};

	template<typename Type>
	inline Type& Engine::Singleton<Type>::GetInstance()
	{
		static Type instance;
		return instance;
	}
}