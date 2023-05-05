/*// -----------------------------------------------------------------
*  File:		ObjectManager.hpp
*  Brief:		Header file of ObjectManager class
*  Creation:	07/11/2022
*  Last Update:	05/05/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class ObjectManager : public RunTime, public Singleton<ObjectManager>, public Serialized
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		void LogicUpdate();
		virtual void Shutdown() override;

		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;

		//Space management
		Space* AddSpace();
		void AddSpace(Space* obj_);
		void DeleteSpace(Space* space);
		Space* GetSpaceByName(const std::string name_);
		std::vector<Space*> GetSpacesByName(const std::string name_);
		Space* GetSpaceByID(const unsigned id_);
		std::vector<Space*> GetSpaces() const;
		void SwapSpaces(const size_t& l_idx, const size_t& r_idx);

		//Get/Set
		void SetLvlName(std::string name);
		std::string GetLvlName() const;

		//Scene management
		void LoadScene(const std::string& filename);
		void SaveScene(const std::string& filename);

	private:
		std::vector<Space*> mSpaces;
		std::string mLvlName = "Default";
	};
}
#define gObjMgr (&Engine::ObjectManager::GetInstance())