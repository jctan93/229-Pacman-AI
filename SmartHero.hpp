#ifndef SMARTHERO_HPP_
#define SMARTHERO_HPP_

#include "Actor.hpp"
#include "GraphMap.hpp"

class SmartHero : public Actor
{
	public:
		SmartHero(int type);
		virtual ~SmartHero();
		virtual int selectNeighbor(GraphMap* map, int x, int y);
		virtual Actor* duplicate();
		virtual const char* getActorId();
		virtual const char* getNetId();
		
	private:
		int connected(GraphMap* map, int* eatables, int tobechecked, int eatablecounter);
		int canBeReached(GraphMap* map, int current, int destination);
		int vertexToReturn(GraphMap* map, int origin, int destination);
		int ShelbieSimmonsMethodDistance(GraphMap* map, int origin, int destination, int* enemies, int numenem);
		int ShelbieSimmonsMethod(GraphMap* map, int origin, int destination, int* enemies, int numenem);
};

#endif