#include "EnemyDesk.h"
#include <Engine/Graphics.h>
#include "physics.h"
#include <Engine/Time.h>


EnemyDesk::EnemyDesk() : SDesk(), model(new Model(L"desk.cmo"))
{
	AddTag("EnemyDesk");
	DirectX::BoundingBox box = model->GetBoundingBox();
	collisionShape = new btBoxShape(btVector3(box.Extents.x, box.Extents.y, box.Extents.z));
	btVector3 positionWithOffset = Physics::LocalTranslationToPhysics(position) + btVector3(box.Center.x, box.Center.y, box.Center.z);
	motionState = new btDefaultMotionState(btTransform(Physics::LocalRotationToPhysics(rotation), positionWithOffset));
	collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo bob = btRigidBody::btRigidBodyConstructionInfo(mass, motionState, collisionShape, inertia);
	collisionShape->setUserPointer((void *)(this));
	bob.m_restitution = 0.0f;
	bob.m_friction = 0.2f;
	howManyPoints = 0;
	cyclePoint = 0;
	pathX[2];
	tempX = 0.0f;
	speed = 5.0f;
	bod = new btRigidBody(bob);
	Physics::world->addRigidBody(bod);
	deskDrawer = new Model(L"desk_drawer.cmo");
}
EnemyDesk::~EnemyDesk()
{

}
void EnemyDesk::setPath(int points, float* DaPath){
	howManyPoints = points;
	pathX[0] = DaPath[0];
	pathX[1] = DaPath[1];
	DirectX::XMFLOAT4 EnPosition;
	DirectX::XMStoreFloat4(&EnPosition, position);
	tempX = abs(EnPosition.x - pathX[0]);
}


void EnemyDesk::Update()
{
	if (isEnabled){
		SDesk::Update();

		DirectX::XMFLOAT4 EnPosition;
		DirectX::XMStoreFloat4(&EnPosition, position);
		btTransform newPos = bod->getWorldTransform();

		if (pathX != NULL){
			if (EnPosition.x > pathX[cyclePoint]){

				newPos.setOrigin(btVector3(newPos.getOrigin()) - btVector3(Time::Delta() * speed, 0.0f, 0.0f));
				tempX -= Time::Delta() * speed;
				if (tempX <= 0.0f){
					//newPos.setOrigin(btVector3(newPos.getOrigin()) + btVector3(abs(tempX), 0.0f, 0.0f));
					cyclePoint++;
					cyclePoint = cyclePoint % howManyPoints;
					tempX = abs(EnPosition.x - pathX[cyclePoint]);
				}
			}
			else if (EnPosition.x < pathX[cyclePoint]){

				newPos.setOrigin(btVector3(newPos.getOrigin()) + btVector3(Time::Delta() * speed, 0.0f, 0.0f));
				tempX -= Time::Delta() * speed;
				if (tempX <= 0.0f){
					//newPos.setOrigin(btVector3(newPos.getOrigin()) - btVector3(abs(tempX), 0.0f, 0.0f));
					cyclePoint++;
					cyclePoint = cyclePoint % howManyPoints;
					tempX = abs(EnPosition.x - pathX[cyclePoint]);
				}
			}
			bod->setWorldTransform(newPos);
		}
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

int EnemyDesk::Die(){

	Physics::world->removeCollisionObject(bod);
	bod = NULL;
	this->isVisible = false;
	this->Disable();
	//returns -1 if died
	return -1;

}
