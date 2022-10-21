#ifndef MESH_H
#define MESH_H

#include "Collision.h"
#include <vector>
#include <string>

class Mesh{
	public:
		Mesh();
		~Mesh();

		bool load(const std::string& fileName, class Renderer* renderer);
		void unload();

		class VertexArray* getVertexArray() { return _vertexArray; }
		class Texture* getTexture(size_t index);

		const std::string& getShaderName() const { return _shaderName; }
		float getRadius() const { return _radius; }
		const AABB& getBox() const { return _box; }
		float getSpecPower() const { return _specPower; }
	private:
		AABB _box;
		std::vector<class Texture*> _textures;
		class VertexArray* _vertexArray;
		std::string _shaderName;
		float _radius;
		float _specPower;
};

#endif

