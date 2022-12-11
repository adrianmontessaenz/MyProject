/*// -----------------------------------------------------------------
*  File:		Space.hpp
*  Brief:		Header file of Space class
*  Creation:	07/11/2022
*  Last Update:	07/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Space : public RunTime, public Identity
	{
	public:
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LogicUpdate() override;
		virtual void Shutdown() override;

		Object* AddObject();
		void AddObject(Object* obj_, bool rec = false);
		void RemoveObject(Object* obj_, bool rec = false);
		void DeleteObject(Object* obj_, bool rec = false);
		void SwapObjects(const size_t& l_idx, const size_t& r_idx);
		void MoveObject(Object* obj, const size_t& idx);

		Object* GetObjectByName(const std::string name_);
		std::vector<Object*> GetObjectsByName(const std::string name_);
		Object* GetObjectByID(const unsigned id_);
		std::vector<Object*> GetObjects() const;

		void SetSceneIdx(const int idx_);
		const int GetSceneIdx() const;

	private:
		std::vector<Object*> mObjects;
		int mSceneIdx = -1;
		void UpdateObjectIdx(unsigned idx_);
	};
}