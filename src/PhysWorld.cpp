#include "../include/PhysWorld.h"
#include "../include/BoxComponent.h"

#include <SDL2/SDL.h>
#include <algorithm>
#include <cstddef>

PhysWorld::PhysWorld(class Game* game):
	_game(game)
{

}

bool PhysWorld::segmentCast(const LineSegment &l, CollisionInfo &outColl){
	bool collided = false;

	float closest = Math::Infinity;
	Vector3 norm;

	for(auto box: _boxes){
		float t;
		if(intersect(l, box->getWorldBox(), t, norm)){
			if(t < closest){
				closest = t;
				outColl.point = l.pointOnSegment(t);
				outColl.normal = norm;
				outColl.box = box;
				outColl.entity = box->getOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::testPairWise(std::function<void (class Entity *, class Entity *)> f){
	for(size_t i = 0; i < _boxes.size(); ++i){
		for(size_t j = i+1; j < _boxes.size(); ++j){
			BoxComponent* a = _boxes[i];
			BoxComponent* b = _boxes[j];
			if(intersect(a->getWorldBox(), b->getWorldBox())){
				f(a->getOwner(), b->getOwner());
			}
		}
	}
}

void PhysWorld::testSweepAndPrune(std::function<void (class Entity *, class Entity *)> f){
	std::sort(_boxes.begin(), _boxes.end(), 
			[](BoxComponent* a, BoxComponent* b) -> bool { return a->getWorldBox().min.x < b->getWorldBox().min.x; } );
	for(size_t i = 0; i < _boxes.size(); ++i){
		BoxComponent* a = _boxes[i];
		float max = a->getWorldBox().max.x;
		for(size_t j = i + 1; j < _boxes.size(); ++j){
			BoxComponent* b = _boxes[j];
			if(b->getWorldBox().min.x > max)
				break;
			else if(intersect(a->getWorldBox(), b->getWorldBox()))
				f(a->getOwner(), b->getOwner());
		}
	}
}

void PhysWorld::addBox(class BoxComponent *box){
	_boxes.emplace_back(box);
}

void PhysWorld::removeBox(class BoxComponent *box){
	auto iter = std::find(_boxes.begin(), _boxes.end(), box);
	if(iter != _boxes.end()){
		std::iter_swap(iter, _boxes.end() - 1);
		_boxes.pop_back()
	}
}
