#pragma once

#include <Engine/Entity.h>
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>


class PhysicsEntity : public Entity
{
public:
	PhysicsEntity();
	~PhysicsEntity();

	void Update();
	void Render();
	void SetPhysicsOrigin();
	virtual int Die(); //returns -1 if needs to be removed from scene

	btCollisionShape *collisionShape;
	btCompoundShape *compoundShape;
	btRigidBody *bod;
	btScalar mass;
	int hp;
	int isEnemy;
protected:
	btDefaultMotionState *motionState;
	btVector3 inertia;
	btTransform startTransform;
};