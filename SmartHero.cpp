#include "OtherActors.hpp"
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include "SmartHero.hpp"
#include <queue>

SmartHero::SmartHero( int type ) :
		Actor(type)
{
	
}

SmartHero::~SmartHero()
{

}

int SmartHero::selectNeighbor( GraphMap* map, int x, int y )
{
		if( map->getNumNeighbors(x,y) == 1)
		{
			return 0;
		}
	
		int* eatablevertices; //list of eatables
		int* pred;
		//int* visited;
		int* pushed;
		int* reachable;
		int* distance;
		int* enemies;
		int* enemydistance;
		int a;
		int b;
		int c; 
		int d;
		int eatablecounter = 0;
		int reachablecounter = 0;
		int enemycounter = 0;
		int targetx;
		int targety;
		std::queue<int> q;
		
		eatablevertices = (int*) malloc(sizeof(int) * map->getNumVertices());
		pred = (int*) malloc(sizeof(int) * map->getNumVertices());
		//visited = (int*)malloc(sizeof(int) * map->getNumVertices());
		pushed = (int*)malloc(sizeof(int) * map->getNumVertices());
		reachable = (int*) malloc(sizeof(int)* map->getNumVertices());
		distance = (int*) malloc(sizeof(int) * map->getNumVertices());
		enemies = (int*) malloc(sizeof(int) * map->getNumVertices());;
		enemydistance = (int*) malloc(sizeof(int) * map->getNumVertices());;
		
		//COUNTS THE NUMBER OF EATABLES
		for(int i = 0;  i< map->getNumVertices(); i++)
		{
			eatablevertices[i] = 0; //Initializes the array to 0;
			pred[i] = -1; // Initializes the predecessor array to -1;
			//visited[i] = -1;
			pushed[i] = -1;
			reachable[i] = -1;
			distance[i] = -1;
			enemies[i] = -1;
			enemydistance[i] = -1;
			
			if (map->getActorType(i) & ACTOR_EATABLE)
			{
				if (map->getActorType(i) & ACTOR_DEAD)
				{
				}
				
				else
				{
				map->getActorPosition(i,a,b);
				eatablevertices[eatablecounter] = map->getVertex(a,b);
				//printf("Eatable at vertex: %d\n", eatablevertices[eatablecounter]);
				eatablecounter++;
				}
			}
			

			else if (map->getActorType(i) & ACTOR_ENEMY)
			{
				if (map->getActorType(i) & ACTOR_DEAD)
				{
				}
				
				else
				{
					map->getActorPosition(i,a,b);
					enemies[enemycounter] = map->getVertex(a,b);
					enemycounter++;
				}
			}
		}
		
		if( map->getNumNeighbors(x,y) == 1)
		{
			free(eatablevertices);
			free(pred);
			free(pushed);
			free(reachable );
			free(distance);
			free(enemies); 
			free(enemydistance);
			return 0;
		}
		
		if(eatablecounter != 0)
		{
			int smallest = 10000;
			int smallestnumber = 0;
			
			int smallestenemy = 10000;
			int smallestenemynumber = 0;
			
			int furthestfromenemies = 0;
			int furthestfromenemiesnumber = 0;
			
			int numberofenemyneighbors = 0;
			int neighborcounter = 0;
			int* enemyneighbors;
			
			if(enemycounter == 0)
			{
				for(int i = 0; i < eatablecounter; i++)
				{
					//FINDS THE REACHABLE EATABLES
					int isreachable = canBeReached(map, map->getVertex(x,y),eatablevertices[i]);
					
					if (isreachable != -1)
					{
						distance[eatablevertices[i]] = isreachable;
						reachable[reachablecounter] = eatablevertices[i];
						reachablecounter++;
					}
					
				}
			}
			
			
			if(enemycounter > 0)
			{
				for(int i = 0; i < enemycounter; i++)
				{
					int temp = canBeReached(map,enemies[i],map->getVertex(x,y));
					if (temp < smallestenemy)
					{
						smallestenemy = temp;
						smallestenemynumber = i;
						
					}
				}
				
				for(int i = 0; i < eatablecounter; i++)
				{
					//FINDS THE REACHABLE EATABLES
					int isreachable = ShelbieSimmonsMethodDistance(map, map->getVertex(x,y),eatablevertices[i], enemies, enemycounter);
					
					if (isreachable != -1)
					{
						distance[eatablevertices[i]] = isreachable;
						reachable[reachablecounter] = eatablevertices[i];
						reachablecounter++;
					}
					
				}
				
				
				for(int i = 0; i < reachablecounter ; i++)
				{
					int temp = distance[reachable[i]];
					if (temp < smallest)
					{
						smallest = temp;
						smallestnumber = i;
					}
				}
				
				for(int i = 0; i < reachablecounter; i++)
				{
					
						int isreachable = canBeReached(map, enemies[smallestenemynumber], reachable[i]);
						if (isreachable > furthestfromenemies)
						{
							furthestfromenemies = isreachable;
							furthestfromenemiesnumber = i;
						}
					
				}
			
			}
			//printf("DISTANCE FROM CLOSEST ENEMY IS %d\n", smallestenemy);
			
			//METHOD 2
			
			if(enemycounter > 0)
			{
				//COUNTS THE NUMBER OF ENEMIES
				for (int i = 0; i < enemycounter; i++)
				{
					map->getPosition(enemies[i],a,b);
					
					numberofenemyneighbors += map->getNumNeighbors(a,b);
				}
				
				enemyneighbors = (int*) malloc(numberofenemyneighbors * sizeof(int));
				
				
				//FINDS ALL THE NEIGHBORS OF THE ENEMIES
				for (int i = 0; i < enemycounter; i++)
				{
					map->getPosition(enemies[i],a,b);
					int tempnum = map->getNumNeighbors(a,b);
					//printf("Enemy at %d has %d neighbors\n", enemies[i], tempnum);
					for (int j = 0; j < tempnum; j++)
					{
						map->getNeighbor(a,b,j,c,d);
						//printf("Neighbor %d: %d\n",j, map->getVertex(c,d));
						int neighborvertex = map->getVertex(c,d);
						enemyneighbors[neighborcounter] = neighborvertex;
						neighborcounter++;
					}
				}
				
				int* enemiesandneighbors = (int*) malloc((numberofenemyneighbors + enemycounter) * sizeof(int));
				for(int i = 0; i < numberofenemyneighbors; i++)
				{
					enemiesandneighbors[i] = enemyneighbors[i];
				}
				
				int enem = 0;
				for(int i = numberofenemyneighbors; i < numberofenemyneighbors + enemycounter; i++)
				{
					enemiesandneighbors[i] = enemies[enem];
					enem++;
				}
				
				//BASED ON CERTAIN CONDITIONS, IT CHOOSES DIFFERENT EATABLES TO GO TO
				int toreturn = 0;
				if(smallestenemy < 5)
				{
					toreturn = ShelbieSimmonsMethod(map, map->getVertex(x,y), reachable[furthestfromenemiesnumber], enemiesandneighbors, numberofenemyneighbors + enemycounter);
					//printf("FURTHEST EATABLE VERTEX TO GO TO IS: %d\n", toreturn);
				}
				
				else
				{
					toreturn = ShelbieSimmonsMethod(map, map->getVertex(x,y), reachable[smallestnumber], enemiesandneighbors, numberofenemyneighbors + enemycounter);
						//printf("CLOSEST EATABLE VERTEX TO GO TO IS: %d\n", toreturn);
				}
				
				if(toreturn  == -1)
				{
					int numtoreturn = 0;
					for(int i = 0; i < map->getNumNeighbors(x,y); i++)
					{
						map->getNeighbor(x,y,i,a,b);
						
						int danger = 0;
						for(int j = 0; j < numberofenemyneighbors + enemycounter; j++)
						{
							if(map->getVertex(a,b) == enemiesandneighbors[j])
							{
								danger = 1;
								break;
							}
						}
						
						if(danger == 0)
						{
							numtoreturn = i;
						}
					}
					
					free(eatablevertices);
					free(pred);
					free(pushed);
					free(reachable);
					free(distance);
					free(enemies) ;
					free(enemydistance);
					free(enemyneighbors);
					free(enemiesandneighbors);
					return numtoreturn;
				}
				
				int danger = 0;
				
				//CHECKS TO SEE IF THE TARGET VERTEX TO MOVE TO IS A NEIGHBOR OF AN ENEMY
				for (int i = 0; i < numberofenemyneighbors; i++)
				{
					//printf("Enemy neighbors: %d\n", enemyneighbors[i]);
					if(enemyneighbors[i] == toreturn)
					{
						danger = 1;
					}
				}
				
			
				
				//IF MOVING TO THAT NEIGHBOR IS A DANGEROUS MOVE, IT CHECKS ALL OTHER NEIGHBORS TO SEE IF ANY ARE SAFE
				if(danger == 1)
				{
					//int distancefromfurthesteatable = 10000;
					int numberforneighbor = 0;
					for(int i = 0; i < map->getNumNeighbors(x,y); i++)
					{
						map->getNeighbor(x,y,i,a,b);
						map->getNeighbor(a,b,0,c,d);
						
						
						
						if(map->getVertex(a,b) != toreturn && map->getVertex(c,d) != map->getVertex(x,y))
						{
							//printf("CURRENTLY ON NEIGHBOR %d, if it returns 0, we get %d\n", map->getVertex(a,b), map->getVertex(c,d));
							int hit = 0;
							for( int j = 0; j < numberofenemyneighbors; j++)
							{
								if (map->getVertex(a,b) == enemyneighbors[j])
								{
									//printf("Neighbor %d at vertex %d is dangerous!\n", i, map->getVertex(a,b));
									hit = 1;
									break;
								}
							}
							
							
							if(hit == 0)
							{

									numberforneighbor = i;
		
							}
						}
					}
					map->getNeighbor(x,y,numberforneighbor,a,b);
					//printf("THE NUMBER FOR THE NEIGHBOR %d TO RETURN IS %d\n",map->getVertex(a,b), numberforneighbor);
					//printf("THE ORIGINAL NEIGHBOR TO RETURN IS %d\n", toreturn);
					
					if(numberforneighbor == 0)
					{
						//printf("AWAITING DEATH!\n \n");
					}
					free(eatablevertices);
					free(pred);
					free(pushed);
					free(reachable);
					free(distance);
					free(enemies) ;
					free(enemydistance);
					free(enemyneighbors);
					free(enemiesandneighbors);
					return numberforneighbor;
				}
				
				// IF THE HERO IS NOT IN DANGER, A REGULAR BFS THINGY IS DONE
				
				if(danger == 0)
				{
					if(toreturn == -1)
					{
						free(eatablevertices);
						free(pred);
						free(pushed);
						free(reachable);
						free(distance);
						free(enemies) ;
						free(enemydistance);
						free(enemyneighbors);
						return 0;
					}
					
					for(int i = 1; i < map->getNumNeighbors(x,y); i++)
					{
						map->getNeighbor(x,y,i,a,b);
						if (map->getVertex(a,b) == toreturn)
						{
							free(eatablevertices);
							free(pred);
							free(pushed);
							free(reachable);
							free(distance);
							free(enemies) ;
							free(enemydistance);
							free(enemyneighbors);
							//printf("SAFE: Returning %d which is vertex %d\n", i, map->getVertex(a,b));
							return i;
						}
					}
				}
			}
			
			//THE STUFF BELOW THIS LINE IS WHAT IS DONE IF THERE ARE NO ENEMIES
			
			//printf("SAFE: GOING TO CLOSEST EATABLE! %d\n", eatablevertices[smallestnumber]);
			map->getPosition(eatablevertices[smallestnumber],targetx, targety);
			
			
			//printf("Target Eatable(%d,%d), Distance = %d, Current Vertex %d, Eatables left = %d\n",targetx, targety, smallest,map->getVertex(x,y), eatablecounter); 
			
			pred[map->getVertex(x,y)] = map->getVertex(x,y); //sets the starting node's predecessor to itself
			
			q.push(map->getVertex(x,y));
			pushed[map->getVertex(x,y)] = 1;
			
			int reachedeatable = 0;
			
			// BFS OCCURS HERE
			while( !q.empty())
			{
				int current = q.front();
				
				
				map->getPosition(current,a,b);
				//	printf("Front is %d (%d,%d)\n", current,a,b);
				q.pop();
				
				
				int numneighbors = map->getNumNeighbors(a,b);
				
				for(int i = 0; i < numneighbors; i++)
				{
					map->getNeighbor( a,b,i,c,d);
					int neighborvertex = map->getVertex(c,d);
					//	printf("Neighbor %d is %d\n", i, neighborvertex);
					
					
					if(pushed[neighborvertex] == -1)
					{
						q.push(neighborvertex);
						//		printf("Neighbor %d was pushed onto the queue\n",neighborvertex);
						pred[neighborvertex] = current;
						pushed[neighborvertex] = 1;
					}
					//	else printf("Neighbor %d has already been pushed\n", neighborvertex);
					
					if (neighborvertex == map->getVertex(targetx,targety))
					{
						//		printf("FOUND ROUTE TO EATABLE!\n");
						reachedeatable = 1;
						break;
					}
				}
				//printf("Navigated through all neighbors!\n");
				//printf("NEXT TO POP IS %d!\n", q.front());
				
				if (reachedeatable == 1)
				{
					break;
				}
			}
			
			
			int tomoveto = map->getVertex(targetx,targety); // sets the start of the backtrace as the destination node
			while(tomoveto != map->getVertex(x,y))
			{
				int previous = pred[tomoveto];
				if(previous == map->getVertex(x,y))
				{
					break;
				}
				else
				{
					tomoveto = previous;
				}
			}
			//printf("VERTEX TO MOVE TO IS %d\n", tomoveto);
			
			
			// FINDS THE NUMBER OF THE VERTEX TO MOVE TO
			for(int i = 0; i < map->getNumNeighbors(x,y); i++)
			{
				map->getNeighbor(x,y,i,a,b);
				if (map->getVertex(a,b) == tomoveto)
				{
					free(eatablevertices);
					free(pred);
					free(pushed);
					free(reachable);
					free(distance);
					free(enemies) ;
					free(enemydistance);
					return i;
				}
			}
			
		}
		
		
		
	free(eatablevertices);
	free(pred);
	free(pushed);
	free(reachable);
	free(distance);
	free(enemies) ;
	free(enemydistance);
	//printf("return 0\n");
	return 0;
	
}

Actor* SmartHero::duplicate()
{
	return new SmartHero(this->getType());
}

const char* SmartHero::getActorId()
{
	return "smarthero";
}

const char* SmartHero::getNetId()
{
	return "jctan93";
}

int SmartHero::connected(GraphMap* map, int* eatables, int tobechecked, int eatablecounter)
{
	//printf("Eatable to be checked is: %d\n", tobechecked);
	int* reachable;
	reachable = (int*) malloc(eatablecounter* sizeof(int));

	
	int a;
	int b;
	int c;
	int d;
	
	for( int k = 0; k < eatablecounter; k++)
	{
		reachable[k] = 0;
		std::queue<int> q;
		q.push(tobechecked);
		int reachedeatable = 0;
		int* pushed;
		pushed = (int*)malloc(sizeof(int) * map->getNumVertices());
		for(int z = 0; z < map->getNumVertices(); z++)
		{
			pushed[z] = -1;
		}
		
		while( !q.empty())
			{
				int current = q.front();

				
				map->getPosition(current,a,b);
				//printf("Front is %d (%d,%d)\n", current,a,b);
				q.pop();
				
				
				int numneighbors = map->getNumNeighbors(a,b);
				
				for(int i = 0; i < numneighbors; i++)
				{
					map->getNeighbor( a,b,i,c,d);
					int neighborvertex = map->getVertex(c,d);
				//	printf("Neighbor %d is %d\n", i, neighborvertex);

					
					if(pushed[neighborvertex] == -1)
					{
						q.push(neighborvertex);
				//		printf("Neighbor %d was pushed onto the queue\n",neighborvertex);
						pushed[neighborvertex] = 1;
					}
				//	else printf("Neighbor %d has already been pushed\n", neighborvertex);
					
					if (neighborvertex == eatables[k])
					{
				//		printf("FOUND ROUTE TO EATABLE!\n");
						reachedeatable = 1;
						reachable[k] = 1;
						break;
					}
				}
				//printf("Navigated through all neighbors!\n");
				//printf("NEXT TO POP IS %d!\n", q.front());
				
				if (reachedeatable == 1)
				{
					break;
				}
			}
		free(pushed);
		if(reachable[k] == 0)
		{
			free(reachable);
			//printf("THIS EATABLE %d is NOT a strongly connected EATABLE\n", tobechecked);
			return 0;
		}
		
			
	}
	
	/**
	* If any eatable if cannot be reached from the current eatable vertex(tobechecked) , return 0
	**/
	for(int i = 0; i < eatablecounter; i++)
	{
		if (reachable[i] == 0)
		{
			free(reachable);
			//printf("THIS EATABLE %d is NOT a strongly connected EATABLE\n", tobechecked);
			return 0;
		}
	}
	
	free(reachable);
	//printf("THIS EATABLE %d is a strongly connected EATABLE\n", tobechecked);
	return 1;
}

int SmartHero::canBeReached(GraphMap* map, int origin, int destination)
{
	std::queue<int> q;
	int a;
	int b;
	int c;
	int d;
	int* pushed;
	int* pred;
	
	
	pushed = (int*) malloc(map->getNumVertices() * sizeof(int));
	pred = (int*) malloc(map->getNumVertices() * sizeof(int));
	
	for(int i = 0; i < map->getNumVertices(); i++)
	{
		pushed[i] = -1;
		pred[i] = -1;
	}
	
	q.push(origin);
	pushed[origin] = 1;
	pred[origin] = origin;
	
	while(!q.empty())
	{
		int current = q.front();
		//printf("CURRENT VERTEX IS %d\n", current);
		q.pop();
		map->getPosition(current,a,b);
		
		int numneighbors = map->getNumNeighbors(a,b);
		
		// FOR EACH NEIGHBOR, CHECK TO SEE IF IT HAS BEEN PUSHED, IF NOT, PUSH IT
		for(int i = 0; i < numneighbors; i++)
		{
			map->getNeighbor(a,b,i,c,d);
			int neighborvertex = map->getVertex(c,d);
			
			if (pushed[neighborvertex] == -1)
			{
				q.push(neighborvertex);
				pushed[neighborvertex] = 1;
				pred[neighborvertex] = current;
			}
			
			
			//IF THE NEIGHBOR IS THE DESTINATION
			if (neighborvertex == destination)
			{
				//printf("Eatable: %d CAN BE REACHED!\n", destination);
				
				int tomoveto = destination;
				int distance = 0;
				
				//CALCULATES THE DISTANCE
				while (tomoveto != origin)
				{
					tomoveto = pred[tomoveto];
					distance++;
				}
				
				free(pred);
				free(pushed);
				return distance;
			}
		}
	}
	
	free(pushed);
	free(pred);
	//printf("DIDN'T FIND A PATH TO THE EATABLE!\n");
	return -1; //NOT REACHABLE
}

int SmartHero::vertexToReturn(GraphMap* map, int origin, int destination)
{
	std::queue<int> q;
	int a;
	int b;
	int c;
	int d;
	int* pushed;
	int* pred;
	
	
	pushed = (int*) malloc(map->getNumVertices() * sizeof(int));
	pred = (int*) malloc(map->getNumVertices() * sizeof(int));
	
	for(int i = 0; i < map->getNumVertices(); i++)
	{
		pushed[i] = -1;
		pred[i] = -1;
	}
	
	q.push(origin);
	pushed[origin] = 1;
	pred[origin] = origin;
	
	while(!q.empty())
	{
		int current = q.front();
		//printf("CURRENT VERTEX IS %d\n", current);
		q.pop();
		map->getPosition(current,a,b);
		
		int numneighbors = map->getNumNeighbors(a,b);
		
		// FOR EACH NEIGHBOR, CHECK TO SEE IF IT HAS BEEN PUSHED, IF NOT, PUSH IT
		for(int i = 0; i < numneighbors; i++)
		{
			map->getNeighbor(a,b,i,c,d);
			int neighborvertex = map->getVertex(c,d);
			
			if (pushed[neighborvertex] == -1)
			{
				q.push(neighborvertex);
				pushed[neighborvertex] = 1;
				pred[neighborvertex] = current;
			}
			
			
			//IF THE NEIGHBOR IS THE DESTINATION
			if (neighborvertex == destination)
			{
				//printf("Eatable: %d CAN BE REACHED!\n", destination);
				
				int tomoveto = destination;
				int distance = 0;
				
				//CALCULATES THE DISTANCE
				while (tomoveto != origin)
				{
					if(pred[tomoveto] == origin)
					{
						break;
					}
					tomoveto = pred[tomoveto];
					distance++;
				}
				
				free(pred);
				free(pushed);
				return tomoveto;
			}
		}
	}
	
	free(pushed);
	free(pred);
	//printf("DIDN'T FIND A PATH TO THE Target!\n");
	return -1; //NOT REACHABLE
}

int SmartHero::ShelbieSimmonsMethod(GraphMap* map, int origin, int destination, int* enemies, int numenem)
{
	std::queue<int> q;
	int a;
	int b;
	int c;
	int d;
	int* pushed;
	int* pred;
 
	
	pushed = (int*) malloc(map->getNumVertices() * sizeof(int));
	pred = (int*) malloc(map->getNumVertices() * sizeof(int));
	
	for(int i = 0; i < map->getNumVertices(); i++)
	{
		pushed[i] = -1;
		pred[i] = -1;
	}
	
	q.push(origin);
	pushed[origin] = 1;
	pred[origin] = origin;
	
	while(!q.empty())
	{
		int current = q.front();
		//printf("CURRENT VERTEX IS %d\n", current);
		q.pop();
		map->getPosition(current,a,b);
		
		int numneighbors = map->getNumNeighbors(a,b);
		
		// FOR EACH NEIGHBOR, CHECK TO SEE IF IT HAS BEEN PUSHED, IF NOT, PUSH IT
		for(int i = 0; i < numneighbors; i++)
		{
			map->getNeighbor(a,b,i,c,d);
			int neighborvertex = map->getVertex(c,d);
			
			
			if (pushed[neighborvertex] == -1)
			{
				if (neighborvertex == destination)
				{
					pred[neighborvertex] = current;
				}
				
				int hitenemy = 0;
				for(int z = 0; z < numenem; z++)
				{
					if( neighborvertex == enemies[z])
					{
						hitenemy = 1;
						break;
					}
				}
				
				if(hitenemy == 0)
				{
					//printf("PUSHING %d which hasn't been pushed\n", neighborvertex);
					q.push(neighborvertex);
					pushed[neighborvertex] = 1;
					pred[neighborvertex] = current;
				}
			}
			
			
			//IF THE NEIGHBOR IS THE DESTINATION
			if (neighborvertex == destination)
			{
				//printf("Eatable: %d CAN BE REACHED!\n", destination);
				
				int tomoveto = destination;
				int distance = 0;
				
				//CALCULATES THE DISTANCE
				while (tomoveto != origin)
				{
					if(pred[tomoveto] == origin)
					{
						break;
					}
					tomoveto = pred[tomoveto];
					distance++;
				}
				
				free(pred);
				free(pushed);
				return tomoveto;
			}
		}
	}
	
	free(pushed);
	free(pred);
	//printf("DIDN'T FIND A PATH TO THE Target!\n");
	return -1; //NOT REACHABLE
}

int SmartHero::ShelbieSimmonsMethodDistance(GraphMap* map, int origin, int destination, int* enemies, int numenem)
{
	std::queue<int> q;
	int a;
	int b;
	int c;
	int d;
	int* pushed;
	int* pred;
	
	
	pushed = (int*) malloc(map->getNumVertices() * sizeof(int));
	pred = (int*) malloc(map->getNumVertices() * sizeof(int));
	
	for(int i = 0; i < map->getNumVertices(); i++)
	{
		pushed[i] = -1;
		pred[i] = -1;
	}
	
	q.push(origin);
	pushed[origin] = 1;
	pred[origin] = origin;
	
	while(!q.empty())
	{
		int current = q.front();
		q.pop();
		map->getPosition(current,a,b);
		
		
		int numneighbors = map->getNumNeighbors(a,b);
		//printf("TARGET: %d ; Popped VERTEX IS %d and has %d neighbors\n",destination, current, numneighbors);
		
		// FOR EACH NEIGHBOR, CHECK TO SEE IF IT HAS BEEN PUSHED, IF NOT, PUSH IT
		for(int i = 0; i < numneighbors; i++)
		{
			map->getNeighbor(a,b,i,c,d);
			int neighborvertex = map->getVertex(c,d);
			//printf("TARGET: %d ; NEIGHBOR %d is %d\n",destination, i, neighborvertex);
			
			if (pushed[neighborvertex] == -1)
			{
				if (neighborvertex == destination)
				{
					pred[neighborvertex] = current;
				}
				
				int hitenemy = 0;
				for(int z = 0; z < numenem; z++)
				{
					if( neighborvertex == enemies[z])
					{
						hitenemy = 1;
						break;
					}
				}
				
				if(hitenemy == 0)
				{
					//printf("PUSHING %d which hasn't been pushed\n", neighborvertex);
					q.push(neighborvertex);
					pushed[neighborvertex] = 1;
					pred[neighborvertex] = current;
				}
			}
			
			
			//IF THE NEIGHBOR IS THE DESTINATION
			if (neighborvertex == destination)
			{
				int tomoveto = destination;
				int distance = 0;
				
				//CALCULATES THE DISTANCE
				//printf("STARTS AT TARGET: %d, it's predecessor is %d\n", tomoveto, pred[tomoveto]);
				while (tomoveto != origin)
				{
					tomoveto = pred[tomoveto];
					distance++;
				}
				
				free(pred);
				free(pushed);
				//printf("END OF METHOD\n");
				return distance;
			}
		}
	}
	
	free(pushed);
	free(pred);
	//printf("DIDN'T FIND A PATH TO THE EATABLE!\n");
	return -1; //NOT REACHABLE
}

//METHOD 2
/*
			if(enemycounter > 0)
			{
				//printf("THERE ARE %d enemies on the board\n", enemycounter);
				for(int i = 0; i < map->getNumNeighbors(x,y); i++)
				{
					map->getNeighbor(x,y,i,c,d);
					int neighborvertex = map->getVertex(c,d);
					//printf("NEIGHBOR %d OF HERO: %d\n", i, neighborvertex);
				}
				
				for (int i = 0; i < enemycounter; i++)
				{
					map->getPosition(enemies[i],a,b);
					
					numberofenemyneighbors += map->getNumNeighbors(a,b);
				}
				
				enemyneighbors = (int*) malloc(numberofenemyneighbors * sizeof(int));
				
				//FINDS ALL THE NEIGHBORS OF THE ENEMIES
				for (int i = 0; i < enemycounter; i++)
				{
					map->getPosition(enemies[i],a,b);
					int tempnum = map->getNumNeighbors(a,b);
					//printf("Enemy at %d has %d neighbors\n", enemies[i], tempnum);
					for (int j = 0; j < tempnum; j++)
					{
						map->getNeighbor(a,b,j,c,d);
						//printf("Neighbor %d: %d\n",j, map->getVertex(c,d));
						int neighborvertex = map->getVertex(c,d);
						enemyneighbors[neighborcounter] = neighborvertex;
						neighborcounter++;
					}
				}
				
				int toreturn = vertexToReturn(map, map->getVertex(x,y), reachable[smallestnumber]);
				
				int danger = 0;
				
				//CHECKS TO SEE IF THE TARGET VERTEX TO MOVE TO IS A NEIGHBOR OF AN ENEMY
				for (int i = 0; i < numberofenemyneighbors; i++)
				{
					//printf("Enemy neighbors: %d\n", enemyneighbors[i]);
					if(enemyneighbors[i] == toreturn)
					{
						danger = 1;
					}
				}
				
				if(danger == 1)
				{
					//printf("DANGER DANGER! DON'T MOVE TO %d\n", toreturn);
					for(int i = 1; i < map->getNumNeighbors(x,y); i++)
					{
						map->getNeighbor(x,y,i,a,b);
						int hit = 0;
						for( int j = 0; j < numberofenemyneighbors; j++)
						{
							if (map->getVertex(a,b) == enemyneighbors[j])
							{
								hit = 1;
								break;
							}
						}
						if (hit == 0)
						{
							free(eatablevertices);
							free(pred);
							free(pushed);
							free(reachable);
							free(distance);
							free(enemies) ;
							free(enemydistance);
							free(enemyneighbors);
							//printf("DANGER: Returning %d which is vertex %d\n", i, map->getVertex(a,b));
							return i;
						}
					}
					
					//INSERT FREE HERE
					//printf("AWAITING DEATH!\n");
					return 0;
				}
				
				if(danger == 0)
				{
					//printf("NO DANGER! VERTEX TO MOVE TO IS %d\n", toreturn);
					for(int i = 1; i < map->getNumNeighbors(x,y); i++)
					{
						map->getNeighbor(x,y,i,a,b);
						if (map->getVertex(a,b) == toreturn)
						{
							free(eatablevertices);
							free(pred);
							free(pushed);
							free(reachable);
							free(distance);
							free(enemies) ;
							free(enemydistance);
							free(enemyneighbors);
							//printf("SAFE: Returning %d which is vertex %d\n", i, map->getVertex(a,b));
							return i;
						}
					}
				}
				
				
				//free(eatablevertices);
				//free(pred);
				//free(pushed);
				//free(reachable);
				//free(distance);
				//free(enemies) ;
				//free(enemydistance);
				//free(enemyneighbors);
				//printf("Staying put like an idiot!\n");
				//return 0;
				
			}
			
*/			