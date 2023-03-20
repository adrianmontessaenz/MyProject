/*// -----------------------------------------------------------------
*  File:		Camera.hpp
*  Brief:		Header for camera
*  Creation:	06/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Camera : public EngineComp
	{
		RTTI_BASE(Camera)
	public:
		virtual void Initialize() noexcept override;
		virtual void Update() noexcept override;
		virtual void Shutdown() noexcept override;

		virtual void ToJson(nlohmann::ordered_json& data) override;
		virtual void FromJson(const nlohmann::ordered_json& data) override;

		//Settors and gettors
		//Transform
		void SetWidth(const int width);
		void SetHeight(const int height);
		void SetSize(const glm::vec2& size);
		glm::vec2 GetSize() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetFront() const;
		glm::vec3 GetRight() const;

		//Planes
		void SetFOV(const float fov);
		float GetFOV() const;
		void SetNear(const float near);
		void SetFar(const float far);
		glm::vec2 GetPlanes() const;

		//Matrices
		glm::mat4 GetView() const;
		glm::mat4 GetProj() const;
	private:
		//Camera "transform" variables
		Transform* mOwnerTransform = nullptr;
		glm::vec3 mUp = glm::vec3(0.f, 1.f, 0.f);
		glm::vec3 mFront = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 mRight = glm::normalize(glm::cross(mFront, { 0.f,1.f,0.f }));
		glm::vec<2, int> mSize{ 0,0 };

		//Camera variables
		float mFOV = 45.f;
		glm::vec<2, float> mPlanes{ 0.1f, 1000.f };

		//Render matrices
		glm::mat4 mView = glm::identity<glm::mat4>();
		glm::mat4 mProj = glm::identity<glm::mat4>();
		void UpdateMatrices();
	};
}