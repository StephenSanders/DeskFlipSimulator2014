#include "PhysicsEntity.h"
#include <Engine/Graphics.h>
#include "physics.h"


PhysicsEntity::PhysicsEntity() : Entity()
{
	AddTag("PhysicsEntity");
	Rotate(0.0f, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	inertia = btVector3(0, 0, 0);
	compoundShape = NULL;
	collisionShape = NULL;
	hp = 100;
	isEnemy = 0;
}
PhysicsEntity::~PhysicsEntity()
{

	/*if (bod)
		delete bod;
	if (motionState)
		delete collisionShape;
	if (collisionShape)
		delete motionState;
		*/

}

void PhysicsEntity::SetPhysicsOrigin()
{

}
void PhysicsEntity::Update()
{

}
void PhysicsEntity::Render(){
}

int PhysicsEntity::Die(){
	
	return 0;
}
