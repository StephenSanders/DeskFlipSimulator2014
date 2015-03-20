#pragma once

#include <Engine/Defines.h>
#include <Engine/Types.h>
#include <Bullet/btBulletDynamicsCommon.h>

namespace Physics{

	extern btBroadphaseInterface               *broadphase;
	extern btDefaultCollisionConfiguration     *collisionConfiguration;
	extern btCollisionDispatcher               *dispatcher;
	extern btSequentialImpulseConstraintSolver *solver;
	extern btDiscreteDynamicsWorld             *world;
	extern btAxisSweep3						   *sweepBP;

	void InitPhysics();

	void Process();

	void DeinitPhysics();

	btVector3 LocalTranslationToPhysics(Vector4 translation);

	btQuaternion LocalRotationToPhysics(Vector4 rotation);
}