/*
 * ssbros.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: stolee
 */

#include "GraphMap.hpp"
#include "GameManager.hpp"
#include "Actor.hpp"
#include "OtherActors.hpp"
#include "SimpleHero.hpp"
#include "SmartEnemy.hpp"
#include "SmartPowerup.hpp"
#include "SmartHero.hpp"
#include <stdlib.h>

int main( int argc, char** argv )
{
	Actor** actors = (Actor**) malloc(10 * sizeof(Actor*));

	int num_actors = 4;
	actors[0] = new SimpleHero(1);
	actors[1] = new SmartEnemy(2);
	actors[2] = new SmartPowerup(8);
	actors[3] = new SmartHero(1);
	GameManager* manager = new GameManager(argc, argv, actors, num_actors);
	manager->addActor(new Actor(ACTOR_HERO));
	manager->play();

	delete manager;
	manager = 0;

	for ( int i = 0; i < num_actors; i++ )
	{
		delete actors[i];
	}
	free(actors);
	actors = 0;
	return 0;
}
