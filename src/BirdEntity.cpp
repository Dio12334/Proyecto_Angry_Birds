#include "../include/BirdEntity.h"
#include "../include/Game.h"
#include "../include/Renderer.h"
#include "../include/Mesh.h"
#include <cmath>

BirdEntity::BirdEntity(class Game* game):
	Entity(game)
{
	_mesh = new MeshComponent(this);
	Mesh* mesh = game->getRenderer()->getMesh("Assets/Sphere.gpmesh");
	_mesh->setMesh(mesh);

	_box = new BoxComponent(this);
	_box->setObjectBox(mesh->getBox());
	_rigid = new RigidBody(this);
}

void BirdEntity::addForce(Vector3 force){
	_rigid->addForce(force);
}
