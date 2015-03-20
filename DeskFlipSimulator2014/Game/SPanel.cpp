#include "SPanel.h"
#include <Engine/Graphics.h>
#include "physics.h"


SPanel::SPanel(PANEL_TYPE p) : StaticEntity()
{
	AddTag("Panel");
	if (p == WALL){
		model = new Model(L"panel.cmo");
	}
	else if (p == CORNER){
		model = new Model(L"panel_corner.cmo");
	}
	else if (p == INTERSECTION){
		model = new Model(L"panel_intersection.cmo");
	}
	hp = 0;
	DirectX::BoundingBox box = model->GetBoundingBox();
	if (p == WALL){
		collisionShape = new btBoxShape(btVector3(box.Extents.x, box.Extents.y, 0.0255f));
	}
	else
		collisionShape = new btBoxShape(btVector3(0.0255f, 0.0255f, 0.0255f));
	collisionShape->setUserPointer((void *)(this));
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + btVector3(box.Center.x, box.Center.y, -0.6349);
	motionState = new btDefaultMotionState(btTransform(Physics::LocalRotationToPhysics(rotation), positionWithOffset));
	collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo bob = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape, inertia);
	bob.m_restitution = 0.0f;
	bob.m_friction = 0.2f;
	bod = new btRigidBody(bob);
	Physics::world->addRigidBody(bod);
}
SPanel::~SPanel()
{
	if (model) delete model;


}

void SPanel::Added(){

	SetPhysicsOrigin();

}

void SPanel::SetPhysicsOrigin()
{
	btTransform trans = bod->getWorldTransform();
	DirectX::BoundingBox box = model->GetBoundingBox();

	Vector4 offset = DirectX::XMVectorSet(box.Center.x, box.Center.y, -0.6349, 0.0);


	trans.setRotation(Physics::LocalRotationToPhysics(rotation));

	offset = DirectX::XMVector4Transform(offset, DirectX::XMMatrixRotationQuaternion(rotation));
	DirectX::XMFLOAT4 fOffset;
	DirectX::XMStoreFloat4(&fOffset, offset);

	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + Physics::LocalTranslationToPhysics(offset);
	trans.setOrigin(positionWithOffset);
	bod->setWorldTransform(trans);
}
void SPanel::Update()
{
	/*physics update code so it'll render correctly goes here*/
	btTransform worldTransform;
	worldTransform = bod->getWorldTransform();
	DirectX::BoundingBox box = model->GetBoundingBox();
	Vector4 offset = DirectX::XMVectorSet(box.Center.x, box.Center.y, -0.6349, 0.0);

	rotation = DirectX::XMQuaternionRotationAxis(
		DirectX::XMVectorSet(worldTransform.getRotation().getAxis().x(), worldTransform.getRotation().getAxis().y(), worldTransform.getRotation().getAxis().z(), 0.0f)
		, worldTransform.getRotation().getAngle());
	offset = DirectX::XMVector4Transform(offset, DirectX::XMMatrixRotationQuaternion(rotation));
	DirectX::XMFLOAT4 fOffset;
	DirectX::XMStoreFloat4(&fOffset, offset);
	position = DirectX::XMVectorSet(worldTransform.getOrigin().x() - fOffset.x, worldTransform.getOrigin().y() - fOffset.y, worldTransform.getOrigin().z() - fOffset.z, 1.0f);


}
void SPanel::Render()
{
	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	Graphics::PopMatrix();
}
