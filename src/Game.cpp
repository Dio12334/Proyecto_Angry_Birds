#include "../include/Game.h"
#include "../include/Renderer.h"
#include "../include/Entity.h"
#include "../include/ImguiHeaders.h"
#include "../include/CameraEntity.h"
#include "../include/MeshComponent.h"
#include "../include/MoveComponent.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_timer.h>
#include <algorithm>


Game::Game():
	_renderer(nullptr),
	/* _audioSystem(nullptr), */
	/* _physWorld(nullptr), */
	_isRunning(true),
	_updatingEntities(false)
{

}

bool Game::initialize(){
	SDL_Log("Begin initialization...\n");
	int sdlInitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	SDL_Log("Initialize SDL...\n");
	if(SDL_Init(sdlInitFlags) != 0){
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	SDL_Log("SDL initialized...\n");

	SDL_Log("Initialize Renderer...\n");
	_renderer = new Renderer(this);
	if(!_renderer->initialize(1024.0f, 768.0f)){
		SDL_Log("Failed to initialize renderer");
		delete _renderer;
		_renderer = nullptr;
		return false;
	}
	SDL_Log("Renderer initialized...\n");

	SDL_Log("Loading data...\n");
	loadData();
	SDL_Log("Data loaded...\n");

	_ticksCount = SDL_GetTicks();
	SDL_Log("Initialized Successfully \n");
	return true;
}

void Game::runloop(){
	while(_isRunning){
		processInput();
		updateGame();
		generateOutput();
	}
}

void Game::processInput(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				_isRunning = false;
				break;
		}
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if(state[SDL_SCANCODE_ESCAPE]){
		_isRunning = false;
	}

	for(auto entity: _entities){
		entity->processInput(state);
	}
}

void Game::updateGame(){
	while(!SDL_TICKS_PASSED(SDL_GetTicks(), _ticksCount + 16));

	float deltaTime = (SDL_GetTicks() - _ticksCount)/1000.0f;
	if(deltaTime > 0.05f){
		deltaTime = 0.05f;
	}
	_ticksCount = SDL_GetTicks();

	_updatingEntities = true;
	for(auto entity: _entities){
		entity->update(deltaTime);
	}
	_updatingEntities = false;
	for(auto pending: _pendingEntities){
		pending->computeWorldTransform();
		_entities.emplace_back(pending);
	}
	_pendingEntities.clear();

	std::vector<Entity*> deadEntities;
	for(auto entity: _entities){
		if(entity->getState() == Entity::eDead){
			deadEntities.emplace_back(entity);
		}
	}

	for(auto entity: deadEntities){
		delete entity;
	}
}

void Game::generateOutput(){
	for(auto entity: _entities){
		if(entity->getDisplayInfo()){
			entity->displayInfo();
		}
	}
	_renderer->draw();
}

void Game::loadData(){
	
	Entity* e = new Entity(this);
	e->setPosition(Vector3(200.f, 75.f, 0.f));
	e->setScale(100.0f);
	e->setDisplayInfo(true);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi/4.f));
	MeshComponent* me = new MeshComponent(e);
	me->setMesh(_renderer->getMesh("Assets/Cube.gpmesh"));
	MoveComponent* mv = new MoveComponent(e);

	_renderer->setAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = _renderer->getDirectionalLight();
	dir.direction = Vector3(0.0f, -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.f);
	dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

	_cameraEntity = new CameraEntity(this);
	_cameraEntity->setDisplayInfo(true);
}

void Game::unloadData(){
	while(!_entities.empty())
		delete _entities.back();

	if(_renderer)
		_renderer->unloadData();
}

void Game::shutdown(){
	unloadData();
	if(_renderer){
		_renderer->shutdown();
	}
	SDL_Quit();
}

void Game::addEntity(class Entity *entity){
	if(_updatingEntities)
		_pendingEntities.emplace_back(entity);
	else
		_entities.emplace_back(entity);
}

void Game::removeEntity(class Entity *entity){
	auto iter = std::find(_pendingEntities.begin(), _pendingEntities.end(), entity);
	if(iter != _pendingEntities.end()){
		std::iter_swap(iter, _pendingEntities.end() - 1);
		_pendingEntities.pop_back();
	}

	iter = std::find(_entities.begin(), _entities.end(), entity);
	if(iter != _entities.end()){
		std::iter_swap(iter, _entities.end() - 1);
		_entities.pop_back();
	}
}
