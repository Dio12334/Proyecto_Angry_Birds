#include "../include/CameraEntity.h"
#include "../include/MoveComponent.h"
#include "../include/Renderer.h"
#include "../include/Game.h"
#include <SDL2/SDL_scancode.h>

CameraEntity::CameraEntity(class Game* game):
	Entity(game)
{
	_moveComp = new MoveComponent(this);
}

void CameraEntity::updateEntity(float deltaTime){
	Entity::updateEntity(deltaTime);
	Vector3 cameraPos = getPosition();
	Vector3 target = getPosition() + getForward() * 100.f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	getGame()->getRenderer()->setViewMatrix(view);
}

void CameraEntity::entityInput(const uint8_t *keys){
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	float fs = 300.f;
	float as = Math::TwoPi;

	if(keys[SDL_SCANCODE_W]){
		forwardSpeed += fs;
	}
	if(keys[SDL_SCANCODE_S]){
		forwardSpeed -= fs;
	}
	if(keys[SDL_SCANCODE_A]){
		angularSpeed -= as;
	}
	if(keys[SDL_SCANCODE_D]){
		angularSpeed += as;
	}

	_moveComp->setForwardSpeed(forwardSpeed);
	_moveComp->setAngularSpeed(angularSpeed);
}
