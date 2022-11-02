#include "../include/PlaneEntity.h"
#include "../include/Game.h"
#include "../include/Renderer.h"
#include "../include/MeshComponent.h"
#include "../include/BoxComponent.h"
#include "../include/Mesh.h"

PlaneEntity::PlaneEntity(class Game* game):
	Entity(game)
{
	setScale(100.f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = getGame()->getRenderer()->getMesh("Assets/Plane.gpmesh");
	mc->setMesh(mesh);

	_box = new BoxComponent(this);
	_box->setObjectBox(mesh->getBox());
}

PlaneEntity::~PlaneEntity(){

}
