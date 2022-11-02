#include "../include/RigidBody.h"
#include "../include/Game.h"
#include "../include/PhysWorld.h"
#include "../include/Entity.h"
#include "../include/ImguiHeaders.h"

RigidBody::RigidBody(class Entity* owner, int updateOrder):
	Component(owner, updateOrder),	
	_mass(1),
	_totalForce(Vector3::Zero)
{
	_owner->getGame()->getPhysWorld()->addRigid(this);	
}

RigidBody::~RigidBody(){
	_owner->getGame()->getPhysWorld()->removeRigid(this);
}
void RigidBody::addForce(Vector3 force){
	_totalForce += force;
}

void RigidBody::update(float deltaTime){
	Vector3 acceleration = _totalForce*(1/_mass);
	Vector3 velocity = acceleration * deltaTime;
	_owner->setPosition(_owner->getPosition() + velocity * deltaTime);
}

void RigidBody::displayInfo(){
	if(ImGui::CollapsingHeader("Rigid Body")){
		ImGui::InputFloat3("Resultant force", &_totalForce.x);
		ImGui::InputFloat("Mass", &_mass);
	}
}


