#include "DynamicEntity.h"
#include <Engine/Graphics.h>
#include "physics.h"


DynamicEntity::DynamicEntity() : PhysicsEntity()
{
	AddTag("DynamicEntity");
	Rotate(0.0f, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	mass = 6.0f;
}
DynamicEntity::~DynamicEntity()
{

}

void DynamicEntity::SetPhysicsOrigin()
{
	
}
void DynamicEntity::Update()
{

}
void DynamicEntity::Render(){
}
