/*// -----------------------------------------------------------------
*  File:		ObjectManager.hpp
*  Brief:		Header file of ObjectManager class
*  Creation:	07/11/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Core/Scene/Space.hpp>

namespace Engine
{
	class ObjectManager : public RunTime, public Singleton<ObjectManager>
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LogicUpdate() override;
		virtual void Shutdown() override;

		Space* AddSpace();
		void AddSpace(Space* obj_);
		void DeleteSpace(Space* space);

		Space* GetSpaceByName(const std::string name_);
		std::vector<Space*> GetSpacesByName(const std::string name_);
		Space* GetSpaceByID(const unsigned id_);
		std::vector<Space*> GetSpaces() const;

	private:
		std::vector<Space*> mSpaces;
		void UpdateSpaceList(unsigned idx_);
	};
}
#define gObjMgr (&Engine::ObjectManager::GetInstance())