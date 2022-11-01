#include "../include/InputSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <cstring>

bool KeyboardState::getKeyValue(SDL_Scancode keyCode) const {
	return _currState[keyCode] == 1;
}

ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const {
	if(_prevState[keyCode] == 0){
		if(_currState[keyCode] == 0)
			return none;
		else
			return pressed;
	}
	else{
		if(_currState[keyCode] == 0)
			return released;
		else
			return held;
	}
}

bool InputSystem::initialize(){
	_state.keyboard._currState = SDL_GetKeyboardState(NULL);
	memset(_state.keyboard._prevState, 0, SDL_NUM_SCANCODES);

	return true;
}

void InputSystem::shutdown(){

}

void InputSystem::prepareForUpdate(){
	memcpy(_state.keyboard._prevState, _state.keyboard._currState, SDL_NUM_SCANCODES);
}

void InputSystem::update(){

}

