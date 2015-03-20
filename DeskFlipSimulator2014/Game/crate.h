#pragma once

#include <Engine/Entity.h>
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class crate : public Entity
{
	public:
		crate();
		~crate();

		void Update();
		void Render();
		void SetPhysicsOrigin();
		void applyForce(float x, float y, float z);

		btCollisionShape *collisionShape;
		btRigidBody *bod;

	private:
		Model *model;
		btDefaultMotionState *motionState;
		btScalar mass;
		btVector3 inertia;
		btTransform startTransform;
};