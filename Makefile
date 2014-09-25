all:
	g++-4.9.1 -fopenmp -lm -g Mesh.cpp -o mesh
	./mesh