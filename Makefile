CC=g++

default:
	$(CC) -fopenmp -lm -g Zombie.cpp Human.cpp GridCell.cpp Mesh_functions.cpp Sim_functions.cpp zombiesim_main.cpp -o zombiesim_main
	./zombiesim_main