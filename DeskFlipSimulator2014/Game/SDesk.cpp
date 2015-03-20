#include "SDesk.h"
#include <Engine/Graphics.h>
#include "physics.h"


SDesk::SDesk() : StaticEntity(), model(new Model(L"desk.cmo"))
{
	AddTag("SDesk");
	DirectX::BoundingBox box = model->GetBoundingBox();
	collisionShape = new btBoxShape(btVector3(box.Extents.x, box.Extents.y, box.Extents.z));
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + btVector3(box.Center.x, box.Center.y, box.Center.z);
	motionState = new btDefaultMotionState(btTransform(Physics::LocalRotationToPhysics(rotation), positionWithOffset));
	collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo bob = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape, inertia);
	collisionShape->setUserPointer((void *)(this));
	bob.m_restitution = 0.0f;
	bob.m_friction = 0.2f;
	bod = new btRigidBody(bob);
	Physics::world->addRigidBody(bod);
	deskDrawer = new Model(L"desk_drawer.cmo");
}
SDesk::~SDesk()
{
	hp = 0;
	if (model) delete model;
	if (deskDrawer) delete deskDrawer;

	if (bod)
		delete bod;
	if (motionState)
		delete collisionShape;
	if (collisionShape)
		delete motionState;

}

void SDesk::SetPhysicsOrigin()
{
	btTransform trans = bod->getWorldTransform();
	DirectX::BoundingBox box = model->GetBoundingBox();

	Vector4 offset = DirectX::XMVectorSet(box.Center.x, box.Center.y, box.Center.z, 0.0);


	trans.setRotation(Physics::LocalRotationToPhysics(rotation));

	offset = DirectX::XMVector4Transform(offset, DirectX::XMMatrixRotationQuaternion(rotation));
	DirectX::XMFLOAT4 fOffset;
	DirectX::XMStoreFloat4(&fOffset, offset);

	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + Physics::LocalTranslationToPhysics(offset);
	trans.setOrigin(positionWithOffset);
	bod->setWorldTransform(trans);
}

void SDesk::Added(){

	SetPhysicsOrigin();


}

void SDesk::Update()
{
	
}
void SDesk::Render()
{

	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	if (deskDrawer)
		deskDrawer->Draw();
	Graphics::PopMatrix();
}

int SDesk::Die(){

	Physics::world->removeCollisionObject(bod);
	bod = NULL;
	this->isVisible = false;
	this->Disable();
	return -1;

}
