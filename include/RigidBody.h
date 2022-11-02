#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "Math.h"

class RigidBody: public Component{
	public:
		RigidBody(class Entity* owner, int udpateOrder = 10);
		~RigidBody();
		void addForce(Vector3 force);
		void update(float deltaTime) override;
		void displayInfo() override;
		void setMass(float mass) { _mass = mass; }
	private:
		
		float _mass;
		Vector3 _totalForce;
};


#endif

