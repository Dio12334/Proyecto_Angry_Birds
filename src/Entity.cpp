#include "../include/Entity.h"
#include "../include/Game.h"
#include "../include/Component.h"
#include <algorithm>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

Entity::Entity(class Game* game):
	_state(eActive),
	_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	_rotation(glm::quat()),
	_scale(1.0f),
	_game(game),
	_recomputeWorldTransform(true)
{
	_game->addEntity(this);
}

Entity::~Entity(){
	_game->removeEntity(this);
	while(!_components.empty()){
		delete _components.back();
	}
}

void Entity::update(float deltaTime){
	if(_state == eActive){
		computeWorldTransform();

		updateComponents(deltaTime);
		updateEntity(deltaTime);

		computeWorldTransform();
	}
}

void Entity::updateComponents(float deltaTime){
	for(auto comp: _components){
		comp->update(deltaTime);
	}
}

void Entity::updateEntity(float deltaTime){

}

void Entity::processInput(const uint8_t *keyState){
	if(_state == eActive){
		for(auto comp: _components){
			comp->processInput(keyState);
		}

		EntityInput(keyState);
	}
}

void Entity::EntityInput(const uint8_t *keyState){

}

void Entity::rotateToNewForward(const glm::vec3 &forward){
	//TODO: toda esta wea
}

void Entity::computeWorldTransform(){
	//TODO: toda esta wea
}

void Entity::addComponent(class Component *component){
	int myOrder = component->getUpdateOrder();
	auto iter = _components.begin();
	for(; iter != _components.end(); ++iter){
		if(myOrder < (*iter)->getUpdateOrder())
			break;
	}
	_components.insert(iter, component);
}

void Entity::removeComponent(class Component *component){
	auto iter = std::find(_components.begin(), _components.end(), component);
	if(iter != _components.end()){
		_components.erase(iter);
	}
}
