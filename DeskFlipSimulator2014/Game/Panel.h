#pragma once

#include <Engine/Entity.h>
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

enum PANEL_TYPE{
	WALL = 0,
	INTERSECTION = 1,
	CORNER = 2
};

class Panel: public Entity
{
public:
	Panel(PANEL_TYPE p);
	~Panel();

	void Update();
	void Render();
	void SetPhysicsOrigin();

	btCollisionShape *collisionShape;
	btRigidBody *bod;

private:
	Model *model;
	btDefaultMotionState *motionState;
	btScalar mass;
	btVector3 inertia;
	btTransform startTransform;
};