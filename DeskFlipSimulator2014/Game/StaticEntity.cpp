#include "StaticEntity.h"
#include <Engine/Graphics.h>
#include "physics.h"


StaticEntity::StaticEntity() : PhysicsEntity()
{
	AddTag("StaticEntity");
	Rotate(0.0f, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	mass = 0.0f;
}
StaticEntity::~StaticEntity()
{

}

void StaticEntity::SetPhysicsOrigin()
{

}
void StaticEntity::Update()
{

}
void StaticEntity::Render(){
}
