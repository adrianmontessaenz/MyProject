/*// -----------------------------------------------------------------
*  File:		Collisions.cpp
*  Brief:		Implementation of collision tests
*  Creation:	19/03/2023
*  Last Update:	19/03/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Collisions.hpp"

bool Engine::AABBvsAABBEx(Collider* p1, Collider* p2, Contact* c)
{
	//Get transforms of each collider
	Transform* o1 = p1->GetOwner()->GetTransform();
	Transform* o2 = p2->GetOwner()->GetTransform();

	//Get position and scale with offseted of collider
	glm::vec3 pos1 = o1->GetWorldPos() + p1->GetOffsetPosition();
	glm::vec3 pos2 = o2->GetWorldPos() + p2->GetOffsetPosition();
	glm::vec3 sca1 = o1->GetWorldScale() * p1->GetOffsetScale();
	glm::vec3 sca2 = o2->GetWorldScale() * p2->GetOffsetScale();

	//Get result of collision and update contact if exists
	bool result = AABBvsAABB(pos1, sca1, pos2, sca2);
	if (result && c)
	{
		//Compute minkowski difference box and get min and max values
		glm::vec3 mk_pos = pos2 - pos1;
		glm::vec3 mk_sca = sca2 + sca1;

		glm::vec3 mk_min = mk_pos - (mk_sca / 2.f);
		glm::vec3 mk_max = mk_pos + (mk_sca / 2.f);

		//Check first left and right
		c->mPen = std::min(fabsf(mk_min.x), fabsf(mk_max.x));
		c->mNormal = glm::vec3(1.f, 0.f, 0.f);

		//Check top and bot
		float tmp = std::min(c->mPen, std::min(fabsf(mk_min.y), fabsf(mk_max.y)));
		if (tmp != c->mPen)
		{
			c->mPen = tmp;
			c->mNormal = glm::vec3(0.f, 1.f, 0.f);
		}

		//Check near and far
		tmp = std::min(c->mPen, std::min(fabsf(mk_min.z), fabsf(mk_max.z)));
		if (tmp != c->mPen)
		{
			c->mPen = tmp;
			c->mNormal = glm::vec3(0.f, 0.f, 1.f);
		}

		//Check that normal is pointing correctly
		if (glm::dot(c->mNormal, mk_pos) < 0)
			c->mNormal = -c->mNormal;
	}
	return result;
}

bool Engine::AABBvsAABB(glm::vec3 pos1, glm::vec3 sca1, glm::vec3 pos2, glm::vec3 sca2)
{
	//Compute minkowski difference box and get min and max values
	glm::vec3 mk_pos = pos2 - pos1;
	glm::vec3 mk_sca = sca2 + sca1;

	glm::vec3 mk_min = mk_pos - (mk_sca / 2.f);
	glm::vec3 mk_max = mk_pos + (mk_sca / 2.f);

	//Check if minkowski aabb contains the origin and return result
	bool x = mk_min.x <= 0.f && mk_max.x >= 0.f;
	bool y = mk_min.y <= 0.f && mk_max.y >= 0.f;
	bool z = mk_min.z <= 0.f && mk_max.z >= 0.f;
	if (x && y && z)
		return true;
	return false;
}

bool Engine::Contact::operator==(const Contact& other)
{
	return (mP1 == other.mP1 && mP2 == other.mP2) || (mP1 == other.mP2 && mP2 == other.mP1);
}
