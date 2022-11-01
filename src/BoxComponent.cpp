#include "../include/BoxComponent.h"
#include "../include/Entity.h"
#include "../include/Game.h"
#include "../include/PhysWorld.h"

BoxComponent::BoxComponent(class Entity* owner, int updateOrder):
	Component(owner, updateOrder),
	_objectBox(Vector3::Zero, Vector3::Zero),
	_worldBox(Vector3::Zero, Vector3::Zero ),
	_shouldRotate(true)
{
	_owner->getGame()->getPhysWorld()->addBox(this);
}

BoxComponent::~BoxComponent(){
	_owner->getGame()->getPhysWorld()->removeBox(this);
}

void BoxComponent::onUpdateWorldTransform(){
	_worldBox = _objectBox;

	_worldBox.min *= _owner->getScale();
	_worldBox.max *= _owner->getScale();

	if(_shouldRotate){
		_worldBox.rotate(_owner->getRotation());
	}

	_worldBox.min += _owner->getPosition();
	_worldBox.max += _owner->getPosition();
}
