/*// -----------------------------------------------------------------
*  File:		PhysicsManager.hpp
*  Brief:		Header file of physics manager
*  Creation:	19/03/2023
*  Last Update:	01/04/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once
#include <Physics/Components/Collider.hpp>
#include <Physics/Utils/Collisions.hpp>
#include <Physics/Components/RigidBody.hpp>
#include <Graphics/Renderable/Model.hpp>

namespace Engine
{
	class PhysicsManager : public Singleton<PhysicsManager>
	{
		RTTI_BASE(PhysicsManager)
	public:
		void Initialize();
		void Update();
		void Render();
		void Shutdown();

		void AddCollider(Collider* col);
		void RemoveCollider(Collider* col);
		void UpdateCollider(Collider* col);

		void AddRigidBody(RigidBody* rb);
		void RemoveRigidBody(RigidBody* rb);
	private:
		//Collision data
		typedef bool(*CollisionFn)(Collider*, Collider*, Contact*);
		std::map<uint8_t, CollisionFn> mCollisionFns;
		std::map<Space*, std::vector<Collider*>> mStaticObjs;
		std::map<Space*, std::vector<Collider*>> mDynamicObjs;
		std::vector<Contact> mCurrentContact;
		std::vector<Contact> mPreviousContact;
		void ResolveContactPenetration(Collider* p1, Collider* p2, Contact* contact);
		void ResolveContactVelocity(Collider* p1, Collider* p2, Contact* contact);

		//RigidBody data
		std::vector<RigidBody*> mRigidBodies;
	};
}
#define gPhysics (&Engine::PhysicsManager::GetInstance())
