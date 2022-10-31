#include "../include/MoveComponent.h"
#include "../include/Entity.h"
#include "../include/ImguiHeaders.h"

MoveComponent::MoveComponent(class Entity* owner, int updateOrder):
	Component(owner, updateOrder),
	_angularSpeed(0.0f),
	_forwardSpeed(0.0f),
	_strafeSpeed(0.0f)
{

}

void MoveComponent::update(float deltaTime){
	if(!Math::NearZero(_angularSpeed)){
		Quaternion rot = _owner->getRotation();
		float angle = _angularSpeed * deltaTime;
		Quaternion inc(Vector3::UnitZ, angle);
		rot = Quaternion::Concatenate(rot, inc);
		_owner->setRotation(rot);
	}

	if(!Math::NearZero(_forwardSpeed) || !Math::NearZero(_strafeSpeed)){
		Vector3 pos = _owner->getPosition();
		pos += _owner->getForward() * _forwardSpeed * deltaTime;
		pos += _owner->getRight() * _strafeSpeed * deltaTime;
		_owner->setPosition(pos);
	}
}

void MoveComponent::displayInfo(){
	if(ImGui::CollapsingHeader("MoveComponent")){
		ImGui::InputFloat("angularSpeed", &_angularSpeed);
		ImGui::InputFloat("forwardSpeed", &_forwardSpeed);
	}
}
