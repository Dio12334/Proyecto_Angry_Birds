#ifndef ENTITY_H
#define ENTITY_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


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

		void processInput(const uint8_t* keyState);
		virtual void EntityInput(const uint8_t* keyState);

		const glm::vec3& getPosition() const { return _position; }
		void setPosition(const glm::vec3& pos) { _position = pos; _recomputeWorldTransform = true; }
		float getScale() const { return _scale; }
		void setScale(float scale) { _scale = scale; _recomputeWorldTransform = true; }
		
		const glm::quat& getRotation() const { return _rotation; }
		void setRotation(const glm::quat& rotation) { _rotation = rotation; _recomputeWorldTransform = true; }

		void computeWorldTransform();
		const glm::mat4& getWorldTransform() const { return _worldTransform; }

		/* glm::vec3 getForward() const { return glm::vec3(glm::vec3(1.0f, 0.0f, 0.0f), _rotation); } */
		glm::vec3 getForward() const;
		glm::vec3 getRight() const;

		void rotateToNewForward(const glm::vec3& forward);

		State getState() const { return _state; }
		void setStateI(State state) { _state = state; }

		class Game* getGame() { return _game; }

	private:
		State _state;
		glm::mat4 _worldTransform;
		glm::vec3 _position;
		glm::quat _rotation;
		float _scale;
		bool _recomputeWorldTransform;

		std::vector<class Component*> _components;
		class Game* _game;
};

#endif

