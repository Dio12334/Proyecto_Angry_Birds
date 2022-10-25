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
    auto unit_x = glm::vec3(1.0f, 0.0f, 0.0f);
    auto unit_z = glm::vec3(0.0f, 0.0f, 1.0f);

    auto dot = glm::dot(unit_x, forward);
    auto angle = acosf(dot);

    auto identity = glm::quat_identity<float, glm::defaultp>();

    if(dot > 0.9999f)
        setRotation(identity);
    else if (dot < -0.9999f){

        setRotation( glm::angleAxis(180.0f, unit_z) ) ;
    }
    else{
        glm::vec3 axis = glm::normalize( glm::cross(unit_x, forward) );
        setRotation( glm::angleAxis(angle, axis) ) ;
    }
}

void Entity::computeWorldTransform(){

    if(_recomputeWorldTransform){
        _recomputeWorldTransform = false;
        _worldTransform = glm::scale(glm::mat4(0.0f), glm::vec3(_scale,_scale,_scale));
        _worldTransform *=  glm::toMat4(_rotation);
        _worldTransform *= glm::translate(glm::mat4(0.0f), _position);

        for(auto comp: _components){
            comp->onUpdateWorldTransform();
        }
    }
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
