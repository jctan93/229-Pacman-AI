#ifndef SMARTENEMY_HPP_
#define SMARTENEMY_HPP_

#include "Actor.hpp"
#include "GraphMap.hpp"

class SmartEnemy : public Actor
{
	public:
		SmartEnemy(int type);
		virtual ~SmartEnemy();
		virtual int selectNeighbor(GraphMap* map, int x, int y);
		virtual Actor* duplicate();
		virtual const char* getActorId();
		virtual const char* getNetId();
		
	private:
		int connected(GraphMap* map, int* eatables, int tobechecked, int eatablecounter);
		int canBeReached(GraphMap* map, int current, int destination);
};

#endif