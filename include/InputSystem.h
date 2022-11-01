#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "Math.h"
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

enum ButtonState{
	none,
	pressed,
	released,
	held
};

class KeyboardState{
	public:
		friend class InputSystem;
		bool getKeyValue(SDL_Scancode keyCode) const;
		ButtonState getKeyState(SDL_Scancode keyCode) const;
	private:
		const Uint8* _currState;
		Uint8 _prevState[SDL_NUM_SCANCODES];
};

class MouseState{
	public:

		friend class InputSystem;

		const Vector2& getPosition() { return _mousePos; }
		const Vector2& getScrollWheel() const { return _scrollWheel; }
		bool isRelative() const { return _isRelative; }

		bool getButtonValue(int button) const;
		ButtonState getButtonState(int button) const;

	private:
		Vector2 _mousePos;
		Vector2 _scrollWheel;
		Uint32 _currButtons;
		Uint32 _prevButtons;

		bool _isRelative;

};

struct InputState{
	KeyboardState keyboard;
	MouseState mouse;
};

class InputSystem{
	public:
		bool initialize();
		void shutdown();

		void prepareForUpdate();
		void update();
		
		const InputState& getState() const { return _state; }
		
		void setRelativeMouseMode(bool value);
		void processEvent(union SDL_Event& event);
	private:
		InputState _state;
};

#endif

