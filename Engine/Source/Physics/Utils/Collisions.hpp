/*// -----------------------------------------------------------------
*  File:		Collisions.hpp
*  Brief:		Header of collision tests
*  Creation:	19/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Physics/Components/Collider.hpp>

namespace Engine
{
	struct Contact
	{
		Collider* mP1 = nullptr;
		Collider* mP2 = nullptr;

		glm::vec3 mNormal = glm::vec3(0.f);
		float mPen = 0.f;

		bool operator==(const Contact& other);
	};

	//Collision functions to call on update
	bool AABBvsAABBEx(Collider* p1, Collider* p2, Contact* c);


	//Actual collision functions
	bool AABBvsAABB(glm::vec3 pos1, glm::vec3 sca1, glm::vec3 pos2, glm::vec3 sca2);
}