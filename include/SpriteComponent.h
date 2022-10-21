#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Component.h"
#include <SDL2/SDL.h>

class SpriteComponent: public Component{
	public:
		SpriteComponent(class Entity* owner, int drawOrder = 100);
		~SpriteComponent();

		virtual void draw(class Shader* shader);
		virtual void setTexture(class Texture* texture);

		int getDrawOrder() const { return _drawOrder; }
		int getTexHeight() const { return _texHeight; }
		int getTexWidth() const { return _texWidth; }

		void setVisible(bool visible) { _visible = visible; }
		bool getVisible() const { return _visible; }

	protected:
		class Texture* _texture;
		int _drawOrder;
		int _texWidth;
		int _texHeight;
		bool _visible;
};

#endif

