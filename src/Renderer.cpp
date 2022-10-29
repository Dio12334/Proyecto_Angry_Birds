#include "../include/Renderer.h"
#include "../include/Game.h"
#include "../include/Shader.h"
#include "../include/VertexArray.h"
#include "../include/Texture.h"
#include "../include/Mesh.h"
#include "../include/SpriteComponent.h"
#include "../include/MeshComponent.h"
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <glm/ext/quaternion_common.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <algorithm>


Renderer::Renderer(Game* game):
	_game(game),
	_spriteShader(nullptr),
	_meshShader(nullptr)

{

}

Renderer::~Renderer(){

}

bool Renderer::initialize(float screenW, float screenH, float near , float far, float fov ){
	_screenWidth = screenW;
	_screenHeight = screenH;
    _near = near;
    _far = far;
    _fov = fov;



    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	_window = SDL_CreateWindow(
			"P.N. ENGINE", 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED,
			static_cast<int>(_screenWidth), 
			static_cast<int>(_screenHeight), 
			SDL_WINDOW_OPENGL);
	if (!_window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	_context = SDL_GL_CreateContext(_window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!loadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	createSpriteVerts();

	return true;
}

void Renderer::shutdown(){
	delete _spriteVerts;
	_spriteShader->unload();
	delete _spriteShader;
	_meshShader->unload();
	delete _meshShader;
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
}

void Renderer::unloadData(){
	for(auto i: _textures){
		i.second->unload();
		delete i.second;
	}
	_textures.clear();

	for(auto i: _meshes){
		i.second->unload();
		delete i.second;
	}
	_meshes.clear();
}

void Renderer::draw(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	_meshShader->setActive();
	_meshShader->setMatrixUniform("uViewProj", _view * _projection);
	setLightUniforms(_meshShader);
	for(auto mc: _meshComps){
		if(mc->getVisible()){
			mc->draw(_meshShader);
		}
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	
	_spriteShader->setActive();
	_spriteVerts->setActive();
	for(auto sprite: _sprites){
		if(sprite->getVisible()){
			sprite->draw(_spriteShader);
		}
	}

	SDL_GL_SwapWindow(_window);
}

void Renderer::addSprite(class SpriteComponent *sprite){
	int drawOrder = sprite->getDrawOrder();
	auto iter = _sprites.begin();

	//TODO: PROBABLEMENTE LA STL TENGA UN INSERT_IF O INSERT_ORDERED QUE HAGA ESTO PERO MÁS 
	//ESTEBAN VILLACORTA
	for(; iter != _sprites.end(); ++iter){
		if(drawOrder < (*iter)->getDrawOrder())
			break;
	}

	_sprites.insert(iter, sprite);
}

void Renderer::removeSprite(class SpriteComponent *sprite){
	auto iter = std::find(_sprites.begin(), _sprites.end(), sprite);
	_sprites.erase(iter);
}

void Renderer::addMeshComp(class MeshComponent *mesh){
	_meshComps.emplace_back(mesh);
}

void Renderer::removeMeshComp(class MeshComponent *mesh){
	auto iter = std::find(_meshComps.begin(), _meshComps.end(), mesh);
	_meshComps.erase(iter);
}

Texture* Renderer::getTexture(const std::string &fileName){
	Texture* tex = nullptr;
	auto iter = _textures.find(fileName);
	if(iter != _textures.end()){
		tex = iter->second;
	}
	else{
		tex = new Texture();
		if(tex->load(fileName)){
			_textures.emplace(fileName, tex);
		}
		else{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::getMesh(const std::string &fileName){	
	Mesh* mesh = nullptr;
	auto iter = _meshes.find(fileName);
	if(iter != _meshes.end()){
		mesh = iter->second;
	}
	else{
		mesh = new Mesh();
		if(mesh->load(fileName, this)){
			_meshes.emplace(fileName, mesh);
		}
		else{
			delete mesh;
			mesh = nullptr;
		}
	}
	return mesh;
}

glm::mat4 CreateSimpleViewProj(float width, float height){
    float temp[16] =
            {
                    2.0f/width, 0.0f, 0.0f, 0.0f ,
                     0.0f, 2.0f/height, 0.0f, 0.0f ,
                     0.0f, 0.0f, 1.0f, 0.0f ,
                     0.0f, 0.0f, 1.0f, 1.0f
            };
    return glm::make_mat4(temp);
}


bool Renderer::loadShaders(){
    _spriteShader = new Shader();
    if(!_spriteShader->load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
        return false;
    _spriteShader->setActive();
    // TODO:
    // CHECK IF VIEW PROJ IS CORREC
    glm::mat4 viewProj = CreateSimpleViewProj(_screenWidth, _screenHeight);
    _spriteShader->setMatrixUniform("uViewProj", viewProj);
    _meshShader = new Shader();
    if(!_meshShader->load("Shaders/Phong.vert", "Shaders/Phong.frag"))
        return false;
    _meshShader->setActive();
    glm::vec3 eye(0.0f, 0.0f, 0.0f), center (1.0f, 0.0f, 0.0f), up(0.0f, 0.0f, 1.0f);
    _view = glm::lookAt(eye, center, up);
    _projection  = glm::perspectiveFov(_fov, _screenWidth, _screenHeight, _near, _far);
    _meshShader->setMatrixUniform("uViewProj", _view * _projection);
    return true;
}


void Renderer::createSpriteVerts(){
	
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	_spriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::setLightUniforms(class Shader* shader){
	glm::mat4 inView = _view;
	glm::inverse(inView);
	shader->setVectorUniform("uCameraPos", glm::vec3(inView[3][0], inView[3][1], inView[3][2]));	
	shader->setVectorUniform("uAmbientLight", _ambientLight);
	shader->setVectorUniform("uDirLight.mDirection", _dirLight.direction);
	shader->setVectorUniform("uDirLight.mDiffuseColor", _dirLight.diffuseColor);
	shader->setVectorUniform("uDirLight.mSpecColor", _dirLight.specColor);
}

glm::vec3 Renderer::unproject(const glm::vec3 &screenPoint) const {
    glm::vec3 deviceCoord = screenPoint;
    glm::vec4 viewport(0.0f,0.0f, _screenWidth, _screenHeight);
    return  glm::unProject(deviceCoord, _view * _projection, _projection, viewport);
}

void Renderer::getScreenDirection(glm::vec3 &outStart, glm::vec3 &outDir) const {
	glm::vec3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = unproject(screenPoint);
	screenPoint.z = 0.9f;
	glm::vec3 end = unproject(screenPoint);
	outDir = end - outStart;
	glm::normalize(outDir);
}
