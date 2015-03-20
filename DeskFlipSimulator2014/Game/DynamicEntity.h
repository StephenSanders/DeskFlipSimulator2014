#pragma once

#include "PhysicsEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>


class DynamicEntity : public PhysicsEntity
{
public:
	DynamicEntity();
	~DynamicEntity();

	void Update();
	void Render();
	void SetPhysicsOrigin();


};