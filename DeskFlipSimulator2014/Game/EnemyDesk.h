#pragma once

#include "SDesk.h"
#include <Engine/Model.h>
#include <Bullet/btBulletDynamicsCommon.h>

class EnemyDesk : public SDesk
{
public:
	EnemyDesk();
	~EnemyDesk();

	void Update();
	void setPath(int points, float* DaPath);
	int Die();
	int howManyPoints;
	float pathX[2];
	float *pathY;
	float *pathZ;
	float tempX;

	float speed;

private:
	Model *model, *deskDrawer;
	int cyclePoint;
};