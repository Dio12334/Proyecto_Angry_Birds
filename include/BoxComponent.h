#ifndef BOXCOMPONENT_H
#define BOXCOMPONENT_H

#include "Component.h"
#include "Collision.h"
#include <iostream>

class BoxComponent: public Component{
	public:
		BoxComponent(class Entity* entity, int updateOrder = 100);
		~BoxComponent();

		void onUpdateWorldTransform() override;

		void setObjectBox(const AABB& model) { _objectBox = model; }
		const AABB& getWorldBox() const { return _worldBox; }

		void setShouldRotate(bool value) { _shouldRotate = value; }

	private:
		AABB _objectBox;
		AABB _worldBox;
		bool _shouldRotate;
};

#endif

