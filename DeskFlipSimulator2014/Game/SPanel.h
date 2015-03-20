#pragma once

#include "StaticEntity.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

enum PANEL_TYPE{
	WALL = 0,
	INTERSECTION = 1,
	CORNER = 2
};

class SPanel : public StaticEntity
{
public:
	SPanel(PANEL_TYPE p);
	~SPanel();

	void Added();
	void Update();
	void Render();
	void SetPhysicsOrigin();


private:
	Model *model;
	btTransform startTransform;
};