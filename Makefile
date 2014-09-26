all:
<<<<<<< HEAD
	g++-4.9.1 -fopenmp -lm -g Mesh.cpp -o mesh
	./mesh
=======
	g++ -fopenmp -lm -g Zombie.cpp Human.cpp GridCell.cpp Mesh_functions.cpp Sim_functions.cpp zombiesim_main.cpp -o zombiesim
	./zombiesim
>>>>>>> 4a62fbc9d34d2fc4f0437dfa548c28e9f224a1c1
