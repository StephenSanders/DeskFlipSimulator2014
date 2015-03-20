#pragma once

#include "DynamicEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class DCornerDesk : public DynamicEntity
{
public:
	DCornerDesk();
	~DCornerDesk();

	void Added();
	void Update();
	void Render();
	void SetPhysicsOrigin();
	int Die();

	btCompoundShape *collisionShapeSpecific;
	btCollisionShape* shape1Shape, *shape2Shape;

private:
	Model *model;
};