#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

class VertexArray{
	public:

		VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
		~VertexArray();

		void setActive();
		unsigned int getNumIndices() const { return _numIndices; }
		unsigned int getNumVerts() const { return _numVerts; }
	private:
		unsigned int _numVerts;
		unsigned int _numIndices;
		unsigned int _vertexBuffer;
		unsigned int _indexBuffer;
		
		unsigned int _vertexArray;
};

#endif

