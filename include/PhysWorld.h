#ifndef PHYSWORLD_H
#define PHYSWORLD_H

#include "Math.h"
#include "Collision.h"
#include <vector>
#include <functional>

class PhysWorld{
	public:
		PhysWorld(class Game* game);

		struct CollisionInfo{
			Vector3 point;
			Vector3 normal;
			class BoxComponent* box;
			class Entity* entity;
		};

		bool segmentCast(const LineSegment& l, CollisionInfo& outColl);
		void testPairWise(std::function<void(class Entity*, class Entity*)> f);
		void testSweepAndPrune(std::function<void(class Entity*, class Entity*)> f);

		void addBox(class BoxComponent* box);
		void removeBox(class BoxComponent* box);
		void addRigid(class RigidBody* rigid);
		void removeRigid(class RigidBody* rigid);

		void addGlobalForce(Vector3 force);
		void subtractGlobalForce(Vector3 force);

	private:
		class Game* _game;
		std::vector<class BoxComponent*> _boxes;
		std::vector<class RigidBody*> _rigids;
};

#endif

