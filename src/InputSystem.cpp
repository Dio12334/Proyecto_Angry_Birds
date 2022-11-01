#include "../include/InputSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
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

bool MouseState::getButtonValue(int button) const {
	return (SDL_BUTTON(button) & _currButtons) == 1;
}

ButtonState MouseState::getButtonState(int button) const {
	int mask = SDL_BUTTON(button);
	if((mask & _prevButtons) == 0){
		if((mask & _currButtons) == 0)
			return none;
		else
			return pressed;
	}
	else{
		if((mask & _currButtons) == 0)
			return released;
		else
			return held;
	}
}

bool InputSystem::initialize(){
	_state.keyboard._currState = SDL_GetKeyboardState(NULL);
	memset(_state.keyboard._prevState, 0, SDL_NUM_SCANCODES);
	_state.mouse._currButtons = 0;
	_state.mouse._prevButtons = 0;
	return true;
}

void InputSystem::shutdown(){

}

void InputSystem::prepareForUpdate(){
	memcpy(_state.keyboard._prevState, _state.keyboard._currState, SDL_NUM_SCANCODES);
	_state.mouse._prevButtons = _state.mouse._currButtons;
	_state.mouse._scrollWheel = Vector2::Zero;
}

void InputSystem::update(){
	int x = 0, y = 0;

	if(_state.mouse._isRelative)
		_state.mouse._currButtons = SDL_GetRelativeMouseState(&x, &y);
	else
		_state.mouse._currButtons = SDL_GetMouseState(&x, &y);

	_state.mouse._mousePos.x = static_cast<float>(x);
	_state.mouse._mousePos.y = static_cast<float>(y);
}

void InputSystem::setRelativeMouseMode(bool value){
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	_state.mouse._isRelative = value;
}

void InputSystem::processEvent(union SDL_Event &event){
	switch(event.type){
		case SDL_MOUSEWHEEL:
			_state.mouse._scrollWheel = Vector2(
					static_cast<float>(event.wheel.x),
					static_cast<float>(event.wheel.y));
			break;
		default:
			break;
	}
}
