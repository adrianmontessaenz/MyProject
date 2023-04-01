/*// -----------------------------------------------------------------
*  File:		RigidBody.hpp
*  Brief:		Header file of RigidBody
*  Creation:	19/03/2023
*  Last Update:	01/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class RigidBody : public EngineComp
	{
		RTTI_BASE(RigidBody)
	public:
		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;
		
		//Forces
		void ApplyForces();
		void AddForce(const std::string& name, const glm::vec3& force);
		void RemoveForce(const std::string& name);
		bool HasForce(const std::string& name);
		glm::vec3 GetTotalForces();

		//Settors
		void SetStatic(const bool stc);
		bool IsStatic() const;
		void SetGravity(const bool gravity);
		bool HasGravity() const;
		void SetMass(const float mass);
		float GetMass() const;
		void SetVelocity(const glm::vec3& vel);
		glm::vec3 GetVelocity() const;
		void SetDrag(const float drag);
		float GetDrag() const;

	private:
		bool mStatic = true;
		bool mGravity = true;
		float mMass = 1.f;
		float mDrag = 0.99f;
		glm::vec3 mVelocity = glm::vec3(0.f);
		std::vector <std::pair<std::string, glm::vec3>> mForces;
	};
}