


all : ssbros


objects : 
	g++ -g -c GameManager.cpp
	g++ -g -c Pursuer.cpp
	g++ -g -c OtherActors.cpp
	g++ -g -c GraphMap.cpp
	g++ -g -c Actor.cpp
	g++ -g -c SimpleHero.cpp
	g++ -g -c SmartEnemy.cpp
	g++ -g -c SmartPowerup.cpp
	g++ -g -c SmartHero.cpp

ssbros : ssbros.cpp OtherActors.o SimpleHero.o SmartEnemy.o SmartPowerup.o SmartHero.o
	g++ -g -Wall -o ssbros ssbros.cpp Actor.o GraphMap.o GameManager.o OtherActors.o Pursuer.o SimpleHero.o SmartEnemy.o SmartPowerup.o SmartHero.o -lncurses

OtherActors.o: OtherActors.cpp
	g++ -g -Wall -c OtherActors.cpp
	
SimpleHero.o: SimpleHero.cpp
	g++ -g -Wall -c SimpleHero.cpp
	
SmartEnemy.o: SmartEnemy.cpp
	g++ -g -Wall -c SmartEnemy.cpp
	
SmartPowerup.o: SmartPowerup.cpp
	g++ -g -Wall -c SmartPowerup.cpp

SmartHero.o: SmartHero.cpp
	g++ -g -Wall -c SmartHero.cpp

# Modify the clean target to remove YOUR .o files...
clean:
	rm ssbros OtherActors.o SimpleHero.o SmartEnemy.o SmartPowerup.o SmartHero.o

tarball:
	tar czf jctan93.tar.gz *.cpp *.hpp Makefile README *.o maps/*.txt

