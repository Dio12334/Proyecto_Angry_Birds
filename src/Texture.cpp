#include "../include/Texture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

Texture::Texture():
	_textureID(0),
	_width(0),
	_height(0)
{

}

Texture::~Texture(){

}

bool Texture::load(const std::string &fileName){
	int channels = 0;
	auto image = SOIL_load_image(
			fileName.c_str(), 
			&_width, 
			&_height, 
			&channels, 
			SOIL_LOAD_AUTO);
	if(!image){
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}
	int format = GL_RGB;
	if(channels == 4){
		format = GL_RGBA;
	}
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return true;
}

void Texture::unload(){
	glDeleteTextures(1, &_textureID);
}

void Texture::setActive(){
	glBindTexture(GL_TEXTURE_2D, _textureID);
}
