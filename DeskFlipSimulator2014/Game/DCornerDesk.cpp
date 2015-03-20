#include "DCornerDesk.h"
#include <Engine/Graphics.h>
#include "physics.h"


DCornerDesk::DCornerDesk() : DynamicEntity(), model(new Model(L"desk_corner.cmo"))
{
	AddTag("Corner Desk");
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
DCornerDesk::~DCornerDesk()
{
	if (model) delete model;

	if (bod)
		delete bod;
	if (motionState)
		delete motionState;
	if (collisionShapeSpecific)
		delete collisionShapeSpecific;

}

void DCornerDesk::Added(){

	SetPhysicsOrigin();

}

void DCornerDesk::SetPhysicsOrigin()
{
	btTransform trans = bod->getWorldTransform();
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position);
	trans.setOrigin(positionWithOffset);
	trans.setRotation(Physics::LocalRotationToPhysics(rotation));
	bod->setWorldTransform(trans);
}
void DCornerDesk::Update()
{
	if (isEnabled){
		/*physics update code so it'll render correctly goes here*/
		btTransform worldTransform;
		worldTransform = bod->getWorldTransform();
		rotation = DirectX::XMQuaternionRotationAxis(
			DirectX::XMVectorSet(worldTransform.getRotation().getAxis().x(), worldTransform.getRotation().getAxis().y(), worldTransform.getRotation().getAxis().z(), 0.0f)
			, worldTransform.getRotation().getAngle());
		position = DirectX::XMVectorSet(worldTransform.getOrigin().x(), worldTransform.getOrigin().y(), worldTransform.getOrigin().z(), 1.0f);

	}
}
void DCornerDesk::Render()
{

	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	Graphics::PopMatrix();
}

int DCornerDesk::Die(){

	Physics::world->removeCollisionObject(bod);
	bod = NULL;
	this->isVisible = false;
	this->Disable();
	return -1;
}