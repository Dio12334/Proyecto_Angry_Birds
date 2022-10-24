#include "../include/Component.h"
#include "../include/Entity.h"

Component::Component(class Entity* owner, int updateOrder):
	_owner(owner),
	_updateOrder(updateOrder)
{
	_owner->addComponent(this);
}

Component::~Component(){
	_owner->removeComponent(this);
}

void Component::update(float deltaTime){

}
