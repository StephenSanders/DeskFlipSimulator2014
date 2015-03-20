#pragma once

#include <Engine/Entity.h>
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class CornerDesk : public Entity
{
public:
	CornerDesk();
	~CornerDesk();

	void Update();
	void Render();
	void SetPhysicsOrigin();

	btCompoundShape *collisionShape;
	btCollisionShape* shape1Shape, *shape2Shape;
	btRigidBody *bod;

private:
	Model *model;
	btDefaultMotionState *motionState;
	btScalar mass;
	btVector3 inertia;
	btTransform startTransform;
};