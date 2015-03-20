#include "crate.h"
#include <Engine/Graphics.h>
#include "physics.h"


crate::crate() : Entity(), model(new Model(L"desk.cmo"))
{
	AddTag("crate");
	Rotate(0.0f, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::BoundingBox box = model->GetBoundingBox();
	collisionShape = new btBoxShape(btVector3(box.Extents.x, box.Extents.y, box.Extents.z));
	collisionShape->setUserPointer((void *)(this));
	mass = 1.0f;
	inertia = btVector3(0, 0, 0);
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + btVector3(box.Center.x, box.Center.y, box.Center.z);
	motionState = new btDefaultMotionState(btTransform(Physics::LocalRotationToPhysics(rotation), positionWithOffset));
	collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo bob = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape, inertia);
	bob.m_restitution = 0.0f;
	bob.m_friction = 0.2f;
	bod = new btRigidBody(bob);
	Physics::world->addRigidBody(bod);
}
crate::~crate()
{
	if (model) delete model;

	if (bod)
		delete bod;
	if (motionState)
		delete collisionShape;
	if (collisionShape)
		delete motionState;
	
}

void crate::SetPhysicsOrigin()
{
	btTransform trans = bod->getWorldTransform();
	DirectX::BoundingBox box = model->GetBoundingBox();
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + btVector3(box.Center.x, box.Center.y, box.Center.z);
	trans.setOrigin(positionWithOffset);
	bod->setWorldTransform(trans);
}
void crate::Update()
{
	/*physics update code so it'll render correctly goes here*/
	btTransform worldTransform;
	worldTransform = bod->getWorldTransform();
	DirectX::BoundingBox box = model->GetBoundingBox();
	position = DirectX::XMVectorSet(worldTransform.getOrigin().x() - box.Center.x , worldTransform.getOrigin().y() - box.Center.y, worldTransform.getOrigin().z() - box.Center.z, 1.0f);
}
void crate::Render()
{

	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	Graphics::PopMatrix();
}

void crate::applyForce(float x, float y, float z)
{
	bod->applyCentralForce(btVector3(x, y, z));

}
