#ifndef ENTITY_H
#define ENTITY_H

#include "Math.h"
#include <vector>
#include <cstdint>

class Entity{
	public:
		enum State{
			eActive,
			ePaused,
			eDead
		};

		Entity(class Game* game);
		virtual ~Entity();

		void update(float deltaTime);
		void updateComponents(float deltaTime);
		virtual void updateEntity(float deltaTime);

		void processInput(const struct InputState& state);
		virtual void entityInput(const struct InputState& state);

		const Vector3& getPosition() const { return _position; }
		void setPosition(const Vector3& pos) { _position = pos; _recomputeWorldTransform = true; }

		float getScale() const { return _scale; }
		void setScale(float scale) { _scale = scale; _recomputeWorldTransform = true; }
		
		const Quaternion& getRotation() const { return _rotation; }
		void setRotation(const Quaternion& rotation) { _rotation = rotation; _recomputeWorldTransform = true; }

		void computeWorldTransform();
		const Matrix4& getWorldTransform() const { return _worldTransform; }

		Vector3 getForward() const { return Vector3::Transform(Vector3::UnitX, _rotation); } 
		Vector3 getRight() const { return Vector3::Transform(Vector3::UnitY, _rotation); }

		void rotateToNewForward(const Vector3& forward);

		State getState() const { return _state; }
		void setStateI(State state) { _state = state; }

		class Game* getGame() { return _game; }
		
		void addComponent(class Component* component);
		void removeComponent(class Component* component);

		void displayInfo();

		void setDisplayInfo(bool info) { _displayInfo = info; }
		bool getDisplayInfo() const { return _displayInfo; }

	private:
		State _state;
		Matrix4 _worldTransform;
		Vector3 _position;
		Quaternion _rotation;
		float _scale;
		bool _recomputeWorldTransform;

		std::vector<class Component*> _components;
		class Game* _game;
	
		bool _displayInfo;

		unsigned int _ID;

		static unsigned int IDAssigner;
};

inline unsigned int Entity::IDAssigner = 0;

#endif

