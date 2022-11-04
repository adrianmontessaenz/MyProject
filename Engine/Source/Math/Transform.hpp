/*// -----------------------------------------------------------------
*  File:		Transform.hpp
*  Brief:		Header of Transform Component
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

using glm::vec3;
using glm::mat4;
namespace Engine
{
	class Transform : public EngineComp
	{
		RTTI_BASE(Transform)
	public:
		virtual void Initialize() override;
		virtual void Shutdown() override;

		//World
		void SetWorldPos(const vec3 pos);
		void SetWorldScale(const vec3 sca);
		void SetWorldRot(const vec3 rot);
		const vec3& GetWorldPos() const;
		const vec3& GetWorldScale() const;
		const vec3& GetWorldRot() const;
		const mat4& GetWorldMat() const;

		//Local
		void SetLocalPos(const vec3 pos);
		void SetLocalScale(const vec3 sca);
		void SetLocalRot(const vec3 rot);
		const vec3& GetLocalPos() const;
		const vec3& GetLocalScale() const;
		const vec3& GetLocalRot() const;
		const mat4& GetLocalMat() const;

	private:
		bool mWChanged = false;
		bool mLChanged = false;
		//World
		vec3 mWPos = vec3(0.f);
		vec3 mWScale = vec3(1.f);
		vec3 mWRot = vec3(0.f);
		//Local
		vec3 mLPos = vec3(0.f);
		vec3 mLScale = vec3(1.f);
		vec3 mLRot = vec3(0.f);

		//Matrices
		mat4 mWMat = glm::identity<mat4>();
		mat4 mLMat = glm::identity<mat4>();

		//Matrix Update
		void UpdateWorld();
		void ComputeWorldMat();
		void UpdateLocal();
		void ComputeLocalMat();
	};
}