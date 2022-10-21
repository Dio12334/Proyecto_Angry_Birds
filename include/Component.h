#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>

class Component{
	public:
		Component(class Entity* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void update(float deltaTime);
		virtual void processInput(const uint8_t* keyState) {}

		virtual void onUpdateWorldTransform() {}

		class Entity* getOwner() { return _owner; }
		int getUpdateOrder() const { return _updateOrder; }
	protected:
		class Entity* _owner;
		int _updateOrder;
};

#endif

