#include "physics.h"
#include <Engine/Time.h>

btBroadphaseInterface               *Physics::broadphase;
btDefaultCollisionConfiguration     *Physics::collisionConfiguration;
btCollisionDispatcher               *Physics::dispatcher;
btSequentialImpulseConstraintSolver *Physics::solver;
btDiscreteDynamicsWorld             *Physics::world;
btAxisSweep3						*Physics::sweepBP;

void Physics::InitPhysics(){

	//broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	sweepBP = new btAxisSweep3(worldMin, worldMax);
	broadphase = sweepBP;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
	world->setGravity(btVector3(0, -9.8, 0));
	

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),0.0f);
	btCollisionShape* ceilingShape = new btStaticPlaneShape(btVector3(0, -1, 0), 0.0f);

	btCollisionShape* Wall1Shape = new btStaticPlaneShape(btVector3(1, 0, 0), 0.0f);
	btCollisionShape* Wall2Shape = new btStaticPlaneShape(btVector3(0, 0, 1), 0.0f);
	btCollisionShape* Wall3Shape = new btStaticPlaneShape(btVector3(-1, 0, 0), 0.0f);
	btCollisionShape* Wall4Shape = new btStaticPlaneShape(btVector3(0, 0, -1), 0.0f);


	btScalar mass = 0; //rigidbody is static if mass is zero, otherwise dynamic
	btVector3 localInertia(0, 0, 0);

	groundShape->calculateLocalInertia(mass, localInertia);

	btTransform groundTransform, wall1Transform, wall2Transform, ceilingTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(10.0f, 0.0f, 10.0f));
	ceilingTransform.setIdentity();
	ceilingTransform.setOrigin(btVector3(10.0f, 2.438f, 10.0f));
	wall1Transform.setIdentity();
	wall1Transform.setOrigin(btVector3(0.0, 0.0, 0.0));
	wall2Transform.setIdentity();
	wall2Transform.setOrigin(btVector3(20.0, 0.0, 20.0));

	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform); //motionstate provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState2 = new btDefaultMotionState(wall1Transform);
	btDefaultMotionState* myMotionState3 = new btDefaultMotionState(wall2Transform);
	btDefaultMotionState* myMotionState4 = new btDefaultMotionState(ceilingTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo2(mass, myMotionState2, Wall1Shape, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo3(mass, myMotionState2, Wall2Shape, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo4(mass, myMotionState3, Wall3Shape, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo5(mass, myMotionState3, Wall4Shape, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo6(mass, myMotionState4, ceilingShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	btRigidBody* body2 = new btRigidBody(rbInfo2);
	btRigidBody* body3 = new btRigidBody(rbInfo3);
	btRigidBody* body4 = new btRigidBody(rbInfo4);
	btRigidBody* body5 = new btRigidBody(rbInfo5);
	btRigidBody* body6 = new btRigidBody(rbInfo6);
	body->setFriction(6.0);
	world->addRigidBody(body); //add the body to the dynamics world
	world->addRigidBody(body2);
	world->addRigidBody(body3);
	world->addRigidBody(body4);
	world->addRigidBody(body5);
	world->addRigidBody(body6);

}

void Physics::Process(){
	
	float test = Time::Delta();
	world->stepSimulation(Time::Delta(), 7, btScalar(1.) / btScalar(60.));

}

btVector3 Physics::LocalTranslationToPhysics(Vector4 translation){

	DirectX::XMFLOAT4 position;
	DirectX::XMStoreFloat4(&position, translation);
	btVector3 newPosition = btVector3(btScalar(position.x), btScalar(position.y), btScalar(position.z));

	return newPosition;
}


btQuaternion Physics::LocalRotationToPhysics(Vector4 rotation){

	Vector4 ax;
	float angle;
	DirectX::XMQuaternionToAxisAngle(&ax, &angle, rotation);
	if (DirectX::XMVectorGetX(DirectX::XMVector3Length(ax)) <= 0.0f) {
		ax = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	}
	ax = DirectX::XMVector3Normalize(ax);
	DirectX::XMFLOAT4 axis;
	DirectX::XMStoreFloat4(&axis, ax);
	btVector3 test = btVector3(axis.x, axis.y, axis.z);
	btQuaternion newRotation;
	newRotation.setRotation(test, (btScalar)angle);
	return newRotation;
}

void Physics::DeinitPhysics(){
	if (world)
		delete world;
	if (solver)
		delete solver;
	if (collisionConfiguration)
		delete collisionConfiguration;
	if (dispatcher)
		delete dispatcher;
	if (broadphase)
		delete broadphase;
}
