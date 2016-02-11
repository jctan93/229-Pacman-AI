#ifndef SMARTPOWERUP_HPP_
#define SMARTPOWERUP_HPP_

#include "Actor.hpp"
#include "GraphMap.hpp"

class SmartPowerup : public Actor
{
	public:
		SmartPowerup(int type);
		virtual ~SmartPowerup();
		virtual int selectNeighbor(GraphMap* map, int x, int y);
		virtual Actor* duplicate();
		virtual const char* getActorId();
		virtual const char* getNetId();
		
	private:
		int connected(GraphMap* map, int* eatables, int tobechecked, int eatablecounter);
		int canBeReached(GraphMap* map, int current, int destination);
		int vertexToReturn(GraphMap* map, int origin, int destination);
};

#endif