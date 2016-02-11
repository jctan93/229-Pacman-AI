#include "OtherActors.hpp"
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include "SimpleHero.hpp"
#include <queue>

SimpleHero::SimpleHero( int type ) :
		Actor(type)
{
	
}

SimpleHero::~SimpleHero()
{

}

int SimpleHero::selectNeighbor( GraphMap* map, int x, int y )
{
		int* eatablevertices; //list of eatables
		int* pred;
		//int* visited;
		int* pushed;
		int* reachable;
		int* distance;
		int a;
		int b;
		int c; 
		int d;
		int eatablecounter = 0;
		int reachablecounter = 0;
		int targetx;
		int targety;
		std::queue<int> q;
		
		eatablevertices = (int*) malloc(sizeof(int) * map->getNumVertices());
		pred = (int*) malloc(sizeof(int) * map->getNumVertices());
		//visited = (int*)malloc(sizeof(int) * map->getNumVertices());
		pushed = (int*)malloc(sizeof(int) * map->getNumVertices());
		reachable = (int*) malloc(sizeof(int)* map->getNumVertices());
		distance = (int*) malloc(sizeof(int) * map->getNumVertices());
		
		//COUNTS THE NUMBER OF EATABLES
		for(int i = 0;  i< map->getNumVertices(); i++)
		{
			eatablevertices[i] = 0; //Initializes the array to 0;
			pred[i] = -1; // Initializes the predecessor array to -1;
			//visited[i] = -1;
			pushed[i] = -1;
			reachable[i] = -1;
			distance[i] = -1;
			
			if (map->getActorType(i) & 4)
			{
				if( map->getActorType(i) & ACTOR_DEAD)
				{
				}
				else
				{
					map->getActorPosition(i,a,b);
					eatablevertices[eatablecounter] = map->getVertex(a,b);
				//	printf("Eatable at vertex: %d\n", eatablevertices[eatablecounter]);
					eatablecounter++;
				}
			}
			
			else if (map->getActorType(i) & 8)
			{
				if( map->getActorType(i) & ACTOR_DEAD)
				{
				}
				else
				{
					map->getActorPosition(i,a,b);
					eatablevertices[eatablecounter] = map->getVertex(a,b);
				//	printf("POWERUP at vertex: %d\n", eatablevertices[eatablecounter]);
					eatablecounter++;
				}
			}
		}
		
		
	//	printf("TOTAL NUMBER OF EATABLES %d\n", eatablecounter);
		if(eatablecounter != 0)
		{
			
			//printf("NUMBER OF EATABLES: %d\n", eatablecounter);
			int smallest = 10000;
			int smallestnumber = -1;
			
			
			for(int i = 0; i < eatablecounter; i++)
			{
			/*
				int temp = map->getVertex(x,y) - eatablevertices[i];
				if (temp < 0)
				{
					temp = temp + temp+ temp;
				}
				
				if (temp < smallest)
				{
					smallest = temp;
					smallestnumber = i;
				}
			
			int isconnected = connected(map, eatablevertices, eatablevertices[i], eatablecounter);
			
				if (isconnected == 1)
				{
					smallestnumber = i;
					break;
				}
			*/
			
			
			//FINDS THE REACHABLE EATABLES
			int isreachable = canBeReached(map, map->getVertex(x,y),eatablevertices[i]);
			
			if (isreachable != -1)
			{
				//printf("Eatable %d can be reached\n", eatablevertices[i]);
				distance[eatablevertices[i]] = isreachable;
				reachable[reachablecounter] = eatablevertices[i];
				reachablecounter++;
			}
			
		   }
		  // printf("FOUND ALL THE REACHABLE EATABLES!\n");
		   
		   //CHECKS REACHABLE FOR STRONGLY CONNECTED REACHABLE EATABLES
		   for (int i = 0; i < reachablecounter; i++)
		   {
				int temp = distance[reachable[i]];
				
				//IF THE CURRENT REACHABLE HAS A SHORTER DISTANCE THAT THE CURRENT SHORTEST DISTANCE, SHORTEST = CURRENT
				if (temp < smallest)
				{
					//printf("Temp is %d < smallest is %d\n", temp, smallest);
					int isconnected = connected(map, reachable, reachable[i], reachablecounter);
					
					
					if (isconnected == 1)
					{
						
						if (temp < smallest)
						{
							smallest = temp;
							smallestnumber = i;
						}
						
						
						smallestnumber = i;
						//break;
					}
					//break;
					
				}
		   }
		   
		   
		   if(smallestnumber ==-1)
		   {
			free(eatablevertices);
			free(pred);
			free(pushed);
			free(reachable);
			free(distance);
			return 0;
		   }
		   
		   
			map->getPosition(reachable[smallestnumber],targetx, targety);
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
					return i;
				}
			}
			
		}
		
	free(eatablevertices);
	free(pred);
	free(pushed);
	free(reachable);
	free(distance);	
	//printf("return 0\n");
	return 0;
}

Actor* SimpleHero::duplicate()
{
	return new SimpleHero(this->getType());
}

const char* SimpleHero::getActorId()
{
	return "simplehero";
}

const char* SimpleHero::getNetId()
{
	return "jctan93";
}

int SimpleHero::connected(GraphMap* map, int* eatables, int tobechecked, int eatablecounter)
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

int SimpleHero::canBeReached(GraphMap* map, int origin, int destination)
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