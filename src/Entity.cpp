#include "../include/Entity.h"
#include "../include/Game.h"
#include "../include/Component.h"
#include "../include/ImguiHeaders.h"

#include <string>
#include <algorithm>


Entity::Entity(class Game* game):
	_state(eActive),
	_worldTransform(Matrix4::Identity),
	_position(Vector3::Zero),
	_rotation(Quaternion::Identity),
	_scale(1.0f),
	_recomputeWorldTransform(true),
	_game(game),
	_displayInfo(false),
	_ID(Entity::IDAssigner++)
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

void Entity::processInput(const struct InputState& state){
	if(_state == eActive){
		for(auto comp: _components){
			comp->processInput(state);
		}

		entityInput(state);
	}
}

void Entity::entityInput(const struct InputState& state){

}

void Entity::rotateToNewForward(const Vector3 &forward){
    auto dot = Vector3::Dot(Vector3::UnitX, forward);
    auto angle = Math::Acos(dot);

    if(dot > 0.9999f)
        setRotation(Quaternion::Identity);
    else if (dot < -0.9999f){
        setRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    }
    else{
        Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
        setRotation(Quaternion(axis, angle));
    }
}

void Entity::computeWorldTransform(){

    if(_recomputeWorldTransform){
        _recomputeWorldTransform = false;
        _worldTransform = Matrix4::CreateScale(_scale);
        _worldTransform *=  Matrix4::CreateFromQuaternion(_rotation);
        _worldTransform *= Matrix4::CreateTranslation(_position);
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

void Entity::displayInfo(){
	std::string windowName = std::string("Entity Info ") + std::to_string(_ID);
	if(ImGui::Begin(windowName.c_str(), &_displayInfo)){
		if(ImGui::CollapsingHeader("General Information")){
			ImGui::InputFloat3("Position", &_position.x);
			ImGui::SliderFloat("Scale", &_scale,0, 1000);
			_recomputeWorldTransform = true;
			computeWorldTransform();
		}

		for(auto comp: _components){
			comp->displayInfo();
		}

		ImGui::End();
	}

}
