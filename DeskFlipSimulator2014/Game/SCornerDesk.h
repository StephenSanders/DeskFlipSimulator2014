#pragma once

#include "StaticEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class SCornerDesk : public StaticEntity
{
public:
	SCornerDesk();
	~SCornerDesk();

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