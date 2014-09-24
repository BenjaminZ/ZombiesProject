all:
	g++ -fopenmp -lm -g Mesh.cpp -o mesh
	./mesh