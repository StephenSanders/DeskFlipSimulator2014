
#include "Floor.h"
#include <Engine/Graphics.h>

Floor::Floor() : Entity(), model(new Model(L"room.cmo"))
{
	AddTag("floor");

}

Floor::~Floor(){
	if (model)
		delete model;

}

void Floor::Update(){

}

void Floor::Render()
{

	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	Graphics::PopMatrix();
}