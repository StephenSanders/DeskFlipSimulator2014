#pragma once

#include "DynamicEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class DDesk : public DynamicEntity
{
public:
	DDesk();
	~DDesk();

	void Added();
	void Update();
	void Render();
	void SetPhysicsOrigin();
	int Die();


private:
	Model *model, *deskDrawer;
};