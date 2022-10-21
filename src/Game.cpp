#include "../include/Game.h"
#include "../include/Renderer.h"
#include "../include/Entity.h"
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
	int sdlInitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	if(SDL_Init(sdlInitFlags) != 0){
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	_renderer = new Renderer(this);
	if(!_renderer->initialize(1024.0f, 768.0f)){
		SDL_Log("Failed to initialize renderer");
		delete _renderer;
		_renderer = nullptr;
		return false;
	}

	loadData();
	_ticksCount = SDL_GetTicks();
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
		switch(event.type){
			case SDL_QUIT:
				_isRunning = false;
				break;
		}
	}
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
	_renderer->draw();
}

void Game::loadData(){

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
