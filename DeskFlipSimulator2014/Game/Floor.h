#pragma once

#include <Engine/Entity.h>
#include <Engine/Model.h>


class Floor : public Entity
{
public:
	Floor();
	~Floor();

	void Update();
	void Render();
private:
	Model *model;
};