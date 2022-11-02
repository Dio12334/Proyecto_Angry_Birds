#include "../include/CameraEntity.h"
#include "../include/MoveComponent.h"
#include "../include/Renderer.h"
#include "../include/Game.h"
#include "../include/InputSystem.h"
#include "../include/Collision.h"
#include "../include/PhysWorld.h"
#include "../include/Math.h"
#include "../include/MeshComponent.h"

#include "../include/BirdEntity.h"
#include <SDL2/SDL_mouse.h>


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

void CameraEntity::entityInput(const struct InputState& state){
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	float fs = 300.f;
	float as = Math::TwoPi;

	if(state.keyboard.getKeyValue(SDL_SCANCODE_W)){
		forwardSpeed += fs;
	}
	if(state.keyboard.getKeyValue(SDL_SCANCODE_S)){
		forwardSpeed -= fs;
	}
	if(state.keyboard.getKeyValue(SDL_SCANCODE_A)){
		angularSpeed -= as;
	}
	if(state.keyboard.getKeyValue(SDL_SCANCODE_D)){
		angularSpeed += as;
	}

	_moveComp->setForwardSpeed(forwardSpeed);
	_moveComp->setAngularSpeed(angularSpeed);

	if(getGame()->debugMode() && state.mouse.getButtonState(SDL_BUTTON_LEFT) == ButtonState::pressed){
		/* const float segmentLength = 100.f; */
		int ix, iy;
		SDL_GetMouseState(&ix, &iy);
		float x, y;
		x = ix - getGame()->getRenderer()->getScreenWidth()/2;
		y = getGame()->getRenderer()->getScreenHeight()/2 - iy;
		
		Vector3 screenPoint(x, y, 0.0f);
		Vector3 start = getGame()->getRenderer()->unproject(screenPoint);
		// Get end point (in center of screen, between near and far)
		screenPoint.z = 0.9f;
		Vector3 end = getGame()->getRenderer()->unproject(screenPoint);
		// Get direction vector
		Vector3 dir = end - start;
		dir.Normalize();
		

		LineSegment l(start, start + dir*800.f);
		PhysWorld* phys = getGame()->getPhysWorld();
		PhysWorld::CollisionInfo info;
		if(phys->segmentCast(l, info)){
			info.entity->setDisplayInfo(true);	
		}
	}
	else if(!getGame()->debugMode() && state.mouse.getButtonState(SDL_BUTTON_LEFT) == ButtonState::released){
		BirdEntity* b = new BirdEntity(getGame());

		int ix, iy;
		SDL_GetMouseState(&ix, &iy);
		float x, y;
		x = ix - getGame()->getRenderer()->getScreenWidth()/2;
		y = getGame()->getRenderer()->getScreenHeight()/2 - iy;
		
		Vector3 screenPoint(x, y, 0.0f);
		Vector3 start = getGame()->getRenderer()->unproject(screenPoint);
		// Get end point (in center of screen, between near and far)
		screenPoint.z = 0.9f;
		Vector3 end = getGame()->getRenderer()->unproject(screenPoint);
		// Get direction vector
		Vector3 dir = end - start;
		dir.Normalize();

		b->setPosition(end);
		b->addForce(dir*30000);
	}

	
}
