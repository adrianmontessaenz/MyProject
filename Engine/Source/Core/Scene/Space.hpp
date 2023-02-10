/*// -----------------------------------------------------------------
*  File:		Space.hpp
*  Brief:		Header file of Space class
*  Creation:	07/11/2022
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Space : public RunTime, public Identity, public Serialized
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LogicUpdate() override;
		virtual void Shutdown() override;

		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;

		Object* AddObject();
		void AddObject(Object* obj_);
		void RemoveObject(Object* obj_);
		void DeleteObject(Object* obj_);
		void SwapObjects(const size_t& l_idx, const size_t& r_idx);

		Object* GetObjectByName(const std::string name_);
		std::vector<Object*> GetObjectsByName(const std::string name_);
		Object* GetObjectByID(const unsigned id_);
		std::vector<Object*> GetObjects() const;

	private:
		std::vector<Object*> mObjects;
		void AddObjectRecursive(Object* obj_);
		void DeleteObjectRecursive(Object* obj_);
	};
}