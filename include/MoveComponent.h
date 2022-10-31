#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include "Component.h"

class MoveComponent: public Component{
	public:
		MoveComponent(class Entity* owner, int updateOrder = 10);
		void update(float deltaTime) override;

		float getAngularSpeed() const { return _angularSpeed; }
		float getForwardSpeed() const { return _forwardSpeed; }
		float getStrafeSpeed() const { return _strafeSpeed; }

		void setAngularSpeed(float speed) { _angularSpeed = speed; }
		void setForwardSpeed(float speed) { _forwardSpeed = speed; }
		void setStrafeSpeed(float speed) { _strafeSpeed = speed; }

		void displayInfo() override;
	private:
		float _angularSpeed;
		float _forwardSpeed;
		float _strafeSpeed;
};
#endif

