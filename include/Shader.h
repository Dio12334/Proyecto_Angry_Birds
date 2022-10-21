#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class Shader{
	public:
		Shader();
		~Shader();

		bool load(const std::string& vertName, const std::string& fragName);
		void unload();
		void setActive();
		void setMatrixUniform(const char* name, const glm::mat4& matrix);
		void setVectorUniform(const char* name, const glm::vec3& vector);
		void setFloatUniform(const char* name, float value);
	private:
		bool compileShader(const std::string& filaName, GLenum shaderType, GLuint& outShader);
		bool isCompiled(GLuint shader);
		bool isValidProgram();

		GLuint _shaderProgram;
		GLuint _vertexShader;
		GLuint _fragShader;
};

#endif

