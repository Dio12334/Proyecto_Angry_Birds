#include "../include/Shader.h"
#include "../include/Math.h"
#include <SDL2/SDL.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader():
	_shaderProgram(0),
	_vertexShader(0),
	_fragShader(0)
{

}

Shader::~Shader(){

}

bool Shader::load(const std::string &vertName, const std::string &fragName){
	if(!compileShader(vertName, GL_VERTEX_SHADER, _vertexShader) 
			|| !compileShader(fragName, GL_FRAGMENT_SHADER, _fragShader)){
		return false;
	}

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragShader);
	glLinkProgram(_shaderProgram);

	if(!isValidProgram())
		return false;

	return true;
}

void Shader::unload(){
	glDeleteProgram(_shaderProgram);
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragShader);
}

void Shader::setActive(){
	glUseProgram(_shaderProgram);
}

void Shader::setMatrixUniform(const char *name, const Matrix4 &matrix){
	GLuint loc = glGetUniformLocation(_shaderProgram, name);	
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::setVectorUniform(const char *name, const Vector3 &vector){
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::setFloatUniform(const char *name, float value){
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform1f(loc, value);
}

bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader){
	std::ifstream shaderFile(fileName);
	if(shaderFile.is_open()){
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(shaderType);

		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if(!isCompiled(outShader)){
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}
	return true;
}

bool Shader::isCompiled(GLuint shader){
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if(status != GL_TRUE){
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader,511,nullptr,buffer);
		SDL_Log("GLSL Compile Failed: \n%s", buffer);
		return false;
	}
	return true;
}

bool Shader::isValidProgram(){	
	GLint status;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &status);

	if(status != GL_TRUE){
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(_shaderProgram,511,nullptr,buffer);
		SDL_Log("GLSL Link Status: \n%s", buffer);
		return false;
	}
	return true;
}
