
CC=g++-4.9.1
CFLAGS= -fopenmp -lm -g
OS=Zombie.o Human.o GridCell.o Mesh_functions.o Sim_functions.o zombiesim_main.o

default: zombiesim_main
	rm -r *.o
	./zombiesim_main

zombiesim_main: $(OS)
	$(CC) $(CFLAGS) -o $@ $(OS) 

%.o: %.cpp
	$(CC) -c $<

