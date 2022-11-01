#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

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

struct InputState{
	KeyboardState keyboard;
};

class InputSystem{
	public:
		bool initialize();
		void shutdown();

		void prepareForUpdate();
		void update();

		const InputState& getState() const { return _state; }

	private:
		InputState _state;
};

#endif

