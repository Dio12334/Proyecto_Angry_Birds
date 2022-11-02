#ifndef GAME_H
#define GAME_H


#include <SDL2/SDL.h>
#include <vector>

class Game{
	public:
		Game();
		bool initialize();
		void runloop();
		void shutdown();

		void addEntity(class Entity* entity);
		void removeEntity(class Entity* entity);

		class Renderer* getRenderer() { return _renderer; }
		/* class AudioSystem* getAudioSystem() { return _audioSystem; } */
		class PhysWorld* getPhysWorld() { return _physWorld; }

		bool debugMode() const { return _debugMode; }

	private:
		void processInput();
		void handleKeyPress(int key);
		void updateGame();
		void generateOutput();
		void loadData();
		void unloadData();
		std::vector<class Entity*> _entities, _pendingEntities;

		class Renderer* _renderer;
		class InputSystem* _inputSystem;
		/* class AudioSystem* _audioSystem; */
		class PhysWorld* _physWorld;

		Uint32 _ticksCount;
		bool _isRunning;
		bool _updatingEntities;

		class CameraEntity* _cameraEntity;

		bool _debugMode;
};

#endif

