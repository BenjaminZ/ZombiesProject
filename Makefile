all:

	g++-4.9.1 -fopenmp -lm -g Zombie.cpp Human.cpp GridCell.cpp Mesh_functions.cpp Sim_functions.cpp zombiesim_main.cpp -o zombiesim
	./zombiesim

