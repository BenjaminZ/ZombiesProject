
CC=g++-4.9.1

default:

	$(CC) -fopenmp -lm -g Zombie.cpp Human.cpp GridCell.cpp MeshManipulation.cpp SimulationExecution.cpp zombiesim.cpp -o zombiesim
	./zombiesim

