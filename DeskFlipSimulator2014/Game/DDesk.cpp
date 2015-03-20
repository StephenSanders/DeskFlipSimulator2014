#include "DDesk.h"
#include <Engine/Graphics.h>
#include "physics.h"


DDesk::DDesk() : DynamicEntity(), model(new Model(L"desk.cmo"))
{
	AddTag("DDesk");
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
DDesk::~DDesk()
{
	if (model) delete model;
	if (deskDrawer) delete deskDrawer;

	if (bod)
		delete bod;
	if (motionState)
		delete collisionShape;
	if (collisionShape)
		delete motionState;

}

void DDesk::SetPhysicsOrigin()
{
	btTransform trans = bod->getWorldTransform();
	DirectX::BoundingBox box = model->GetBoundingBox();

	Vector4 offset = DirectX::XMVectorSet(box.Center.x, -box.Center.y, box.Center.z, 0.0);


	trans.setRotation(Physics::LocalRotationToPhysics(rotation));

	offset = DirectX::XMVector4Transform(offset, DirectX::XMMatrixRotationQuaternion(rotation));
	DirectX::XMFLOAT4 fOffset;
	DirectX::XMStoreFloat4(&fOffset, offset);

	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + Physics::LocalTranslationToPhysics(offset);
	trans.setOrigin(positionWithOffset);
	bod->setWorldTransform(trans);
}

void DDesk::Added(){

	SetPhysicsOrigin();


}

void DDesk::Update()
{
	if (isEnabled){
		/*physics update code so it'll render correctly goes here*/
		btTransform worldTransform;
		worldTransform = bod->getWorldTransform();
		DirectX::BoundingBox box = model->GetBoundingBox();
		Vector4 offset = DirectX::XMVectorSet(box.Center.x, box.Center.y, box.Center.z, 0.0);

		rotation = DirectX::XMQuaternionRotationAxis(
			DirectX::XMVectorSet(worldTransform.getRotation().getAxis().x(), worldTransform.getRotation().getAxis().y(), worldTransform.getRotation().getAxis().z(), 0.0f)
			, worldTransform.getRotation().getAngle());
		offset = DirectX::XMVector4Transform(offset, DirectX::XMMatrixRotationQuaternion(rotation));
		DirectX::XMFLOAT4 fOffset;
		DirectX::XMStoreFloat4(&fOffset, offset);
		position = DirectX::XMVectorSet(worldTransform.getOrigin().x() - fOffset.x, worldTransform.getOrigin().y() - fOffset.y, worldTransform.getOrigin().z() - fOffset.z, 1.0f);
	}

}
void DDesk::Render()
{

	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	if (deskDrawer)
		deskDrawer->Draw();
	Graphics::PopMatrix();
}

int DDesk::Die(){

	Physics::world->removeCollisionObject(bod);
	bod = NULL;
	this->isVisible = false;
	this->Disable();
	//returns -1 if died
	return -1;

}
