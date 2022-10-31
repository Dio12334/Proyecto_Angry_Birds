#ifndef RENDERER_H
#define RENDERER_H

#include "Math.h"
#include "ImguiHeaders.h"
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <string>

struct DirectionalLight{
	Vector3 direction;
	Vector3 diffuseColor;
	Vector3 specColor;
};

class Renderer{
	public:
		Renderer(class Game* game);
		~Renderer();

        bool initialize(float screenW, float screenH, float near = 10.0f, float far = 10000.0f, float fov = 70.0f);
		void shutdown();
		void unloadData();

		void draw();

		void addSprite(class SpriteComponent* sprite);
		void removeSprite(class SpriteComponent* sprite);

		void addMeshComp(class MeshComponent* mesh);
		void removeMeshComp(class MeshComponent* mesh);

		class Texture* getTexture(const std::string& fileName);
		class Mesh* getMesh(const std::string& fileName);

		void setViewMatrix(const Matrix4& view) { _view = view; }
		void setAmbientLight(const Vector3& ambient) { _ambientLight = ambient; }

		DirectionalLight& getDirectionalLight() { return _dirLight; }

		Vector3 unproject(const Vector3& screenPoint) const;
		void getScreenDirection(Vector3& outStart, Vector3& outDir) const;
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

		Matrix4 _view;
		Matrix4 _projection;

        float _screenWidth;
        float _screenHeight;
        float _far;
        float _near;
        float _fov;  //degree unless GLM_FORCE_RADIANS  is used

		Vector3 _ambientLight;
		DirectionalLight _dirLight;

		SDL_Window* _window;
		SDL_GLContext _context;
		ImGuiIO* _io;
};

#endif

