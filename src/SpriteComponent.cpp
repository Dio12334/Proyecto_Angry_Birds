#include "../include/SpriteComponent.h"
#include "../include/Texture.h"
#include "../include/Shader.h"
#include "../include/Entity.h"
#include "../include/Game.h"
#include "../include/Renderer.h"
#include "../include/Math.h"


SpriteComponent::SpriteComponent(class Entity* owner, int drawOrder):
	Component(owner),
	_texture(nullptr),
	_drawOrder(drawOrder),
	_texWidth(0),
	_texHeight(0),
	_visible(true)
{
	_owner->getGame()->getRenderer()->addSprite(this);
}

SpriteComponent::~SpriteComponent(){
	_owner->getGame()->getRenderer()->removeSprite(this);
}

void SpriteComponent::draw(class Shader *shader){
	if(_texture){
		Matrix4 scaleMat;
		Matrix4 world = scaleMat * _owner->getWorldTransform();
		shader->setMatrixUniform("uWorldTransform", world);
		_texture->setActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::setTexture(class Texture *texture){
	_texture = texture;
	_texWidth = texture->getWidth();
	_texHeight = texture->getHeight();
}
