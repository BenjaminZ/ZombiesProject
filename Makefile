CC=g++

default:
	$(CC) -fopenmp -lm -g Zombie.cpp Human.cpp GridCell.cpp MeshManipulation.cpp SimulationExecution.cpp zombiesim.cpp -o zombiesim
	./zombiesim