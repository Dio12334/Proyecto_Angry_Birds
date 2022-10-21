#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture{
	public:
		Texture();
		~Texture();

		bool load(const std::string& fileName);
		void unload();

		void setActive();

		int getWidth() const { return _width; }
		int getHeight() const { return _height; }
	private:
		unsigned int _textureID;
		int _width;
		int _height;
};

#endif

