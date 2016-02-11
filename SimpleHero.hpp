#ifndef SIMPLEHERO_HPP_
#define SIMPLEHERO_HPP_

#include "Actor.hpp"
#include "GraphMap.hpp"

class SimpleHero : public Actor
{
	public:
		SimpleHero(int type);
		virtual ~SimpleHero();
		virtual int selectNeighbor(GraphMap* map, int x, int y);
		virtual Actor* duplicate();
		virtual const char* getActorId();
		virtual const char* getNetId();
		
	private:
		int connected(GraphMap* map, int* eatables, int tobechecked, int eatablecounter);
		int canBeReached(GraphMap* map, int current, int destination);
};

#endif