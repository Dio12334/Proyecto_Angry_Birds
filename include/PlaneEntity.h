#ifndef PLANEENTITY_H
#define PLANEENTITY_H

#include "Entity.h"

class PlaneEntity: public Entity{
	public:
		PlaneEntity(class Game* game);
		~PlaneEntity();
		class BoxComponent* getBox() { return _box; }

	private:
		class BoxComponent* _box;
};

#endif

