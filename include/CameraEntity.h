#ifndef CAMERAENTITY_H
#define CAMERAENTITY_H

#include "Entity.h"

class CameraEntity: public Entity{
	public:
		CameraEntity(class Game* game);

		void updateEntity(float deltaTime) override;
		void entityInput(const struct InputState& state) override;
	private:
		class MoveComponent* _moveComp;
};

#endif

