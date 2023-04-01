/*// -----------------------------------------------------------------
*  File:		Collider.hpp
*  Brief:		Header file of collider
*  Creation:	19/03/2023
*  Last Update:	01/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Graphics/Renderable/Model.hpp>

namespace Engine
{
	class Collider : public EngineComp
	{
		RTTI_BASE(Collider)
	public:
		enum ColliderType
		{
			COLLIDER_AABB = 1,
			COLLIDER_SPHERE = 2,
			COLLIDER_OBB = 4
		};
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void Shutdown() override;
		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;

		void SetOffsetPosition(const glm::vec3& pos);
		glm::vec3 GetOffsetPosition() const;
		void SetOffsetScale(const glm::vec3& sca);
		glm::vec3 GetOffsetScale() const;
		void SetOffsetRotation(const glm::vec3& rot);
		glm::vec3 GetOffsetRotation() const;
		glm::mat4 GetOffsetMatrix() const;

		void SetGhost(const bool ghost);
		bool IsGhost() const;
		void SetDrawCollider(const bool draw);
		bool IsColliderDrawn() const;
		void SetColliderType(ColliderType type);
		int GetColliderType() const;

	private:
		//Offsets
		glm::vec3 mPos = glm::vec3(0.f);
		glm::vec3 mSca = glm::vec3(1.f);
		glm::vec3 mRot = glm::vec3(0.f);
		glm::mat4 mMat = glm::identity<glm::mat4>();
		void ComputeMyMatrix();

		//Other collider data
		bool mGhost = false;
		ColliderType mType = COLLIDER_AABB;

		//Draw data
		bool mDraw = false;
		Model* mModel = nullptr;
		void UpdateModel();
	};
}
