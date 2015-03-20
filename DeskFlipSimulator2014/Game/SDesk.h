#pragma once

#include "StaticEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class SDesk : public StaticEntity
{
public:
	SDesk();
	~SDesk();

	void Added();
	void Update();
	void Render();
	void SetPhysicsOrigin();
	int Die();


private:
	Model *model, *deskDrawer;
};