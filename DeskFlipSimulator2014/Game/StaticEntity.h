#pragma once

#include "PhysicsEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>


class StaticEntity : public PhysicsEntity
{
public:
	StaticEntity();
	~StaticEntity();

	void Update();
	void Render();
	void SetPhysicsOrigin();


};