#ifndef RENDERER_H
#define RENDERER_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <string>


struct DirectionalLight{
	glm::vec3 direction;
	glm::vec3 diffuseColor;
	glm::vec3 specColor;
};

class Renderer{
	public:
		Renderer(class Game* game);
		~Renderer();

		bool initialize(float screenW, float screenH);
		void shutdown();
		void unloadData();

		void draw();

		void addSprite(class SpriteComponent* sprite);
		void removeSprite(class SpriteComponent* sprite);

		void addMeshComp(class MeshComponent* mesh);
		void removeMeshComp(class MeshComponent* mesh);

		class Texture* getTexture(const std::string& fileName);
		class Mesh* getMesh(const std::string& fileName);

		void setViewMatrix(const glm::mat4& view) { _view = view; }
		void setAmbientLight(const glm::vec3& ambient) { _ambientLight = ambient; }

		DirectionalLight& getDirectionalLight() { return _dirLight; }

		glm::vec3 unproject(const glm::vec3& screenPoint) const;
		void getScreenDirection(glm::vec3& outStart, glm::vec3& outDir) const;
		float getScreenWidth() const { return _screenWidth; }
		float getScreenHeight() const { return _screenHeight; }

	private:
		bool loadShaders();
		void createSpriteVerts();
		void setLightUniforms(class Shader* shader);

		std::unordered_map<std::string, class Texture*> _textures;

		std::unordered_map<std::string, class Mesh*> _meshes;

		std::vector<class SpriteComponent*> _sprites;

		std::vector<class MeshComponent*> _meshComps;

		class Game* _game;

		class Shader* _spriteShader;

		class VertexArray* _spriteVerts;

		class Shader* _meshShader;

		glm::mat4 _view;
		glm::mat4 _projection;

		float _screenWidth;
		float _screenHeight;

		glm::vec3 _ambientLight;
		DirectionalLight _dirLight;

		SDL_Window* _window;
		SDL_GLContext _context;
};

#endif

