#include "SCornerDesk.h"
#include <Engine/Graphics.h>
#include "physics.h"


SCornerDesk::SCornerDesk() : StaticEntity(), model(new Model(L"desk_corner.cmo"))
{
	AddTag("SCorner Desk");
	//DirectX::BoundingBox box = model->GetBoundingBox();
	//collisionShape = new btBoxShape(btVector3(box.Extents.x, box.Extents.y, box.Extents.z));
	collisionShapeSpecific = new btCompoundShape();
	btTransform shape1 = btTransform();
	shape1.setIdentity();
	btTransform shape2 = btTransform();
	shape2.setIdentity();
	shape1.setOrigin(btVector3(0.0f, 0.368f, -0.3095f));
	shape1Shape = new btBoxShape(btVector3(0.6095f, 0.368f, 0.3f));// 0.3f is technically what it should be but I changed it so it would look better
	shape2.setOrigin(btVector3(-0.3095f, 0.368f, 0.0f));
	shape2Shape = new btBoxShape(btVector3(0.3f, 0.368f, .6095f));// 0.3f is technically what it should be but I changed it so it would look better

	mass = 6.0f;
	shape1Shape->calculateLocalInertia(mass, inertia);
	shape2Shape->calculateLocalInertia(mass, inertia);
	collisionShapeSpecific->addChildShape(shape1, shape1Shape);
	collisionShapeSpecific->addChildShape(shape2, shape2Shape);

	shape1.setIdentity();
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position);
	motionState = new btDefaultMotionState(shape1);
	collisionShapeSpecific->calculateLocalInertia(6.0f, inertia);
	btRigidBody::btRigidBodyConstructionInfo bob = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShapeSpecific, inertia);
	bob.m_restitution = 0.0f;
	bob.m_friction = 0.2f;
	bod = new btRigidBody(bob);
	shape1.setIdentity();
	bod->setCenterOfMassTransform(shape1);
	Physics::world->addRigidBody(bod);
	collisionShape = collisionShapeSpecific;
	collisionShape->setUserPointer((void *)(this));
}
SCornerDesk::~SCornerDesk()
{
	if (model) delete model;

	if (bod)
		delete bod;
	if (motionState)
		delete motionState;
	if (collisionShapeSpecific)
		delete collisionShapeSpecific;

}

void SCornerDesk::Added(){

	SetPhysicsOrigin();

}

void SCornerDesk::SetPhysicsOrigin()
{
	btTransform trans = bod->getWorldTransform();
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position);
	trans.setOrigin(positionWithOffset);
	trans.setRotation(Physics::LocalRotationToPhysics(rotation));
	bod->setWorldTransform(trans);
}
void SCornerDesk::Update()
{

}
void SCornerDesk::Render()
{

	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	Graphics::PopMatrix();
}

int SCornerDesk::Die(){

	Physics::world->removeCollisionObject(bod);
	bod = NULL;
	this->isVisible = false;
	this->Disable();
	return -1;
}