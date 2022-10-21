#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Component.h"
#include <cstddef>

class MeshComponent: public Component{
	public:
		MeshComponent(class Entity* owner);
		~MeshComponent();

		virtual void draw(class Shader* shader);
		virtual void setMesh(class Mesh* mesh) { _mesh = mesh;}
		void setTextureIndex(size_t index) { _textureIndex = index; }

		void setVisible(bool visible) { _visible = visible; }
		bool getVisible() const { return _visible; }
	protected:
		class Mesh* _mesh;
		size_t _textureIndex;
		bool _visible;
};

#endif

