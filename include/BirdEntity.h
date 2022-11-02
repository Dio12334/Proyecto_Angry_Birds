#ifndef BIRDENTITY_H
#define BIRDENTITY_H

#include "Entity.h"
#include "MeshComponent.h"
#include "RigidBody.h"
#include "BoxComponent.h"


class BirdEntity: public Entity{
	public:
		BirdEntity(class Game* game);
		void addForce(Vector3 force);
	private:
		MeshComponent* _mesh;
		RigidBody* _rigid;
		BoxComponent* _box;
		
};

#endif

