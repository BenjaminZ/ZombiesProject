/*
File: zombiesim_main.cpp
*/

#include "zombiesim_parameters.hpp"
#include "Human.hpp"
#include "Zombie.hpp"
#include "GridCell.hpp"
#include "Sim_functions.hpp"
#include "Mesh_functions.hpp"

#if defined(_OPENMP)
void lock(int i, bool *locks) {
	for (bool locked = false; locked == false; /*NOP*/) 
	{
		#pragma omp critical (LockRegion)
		{
			locked = !locks[i-1] && !locks[i] && !locks[i+1];
			if (locked) 
			{
				locks[i-1] = true; locks[i] = true; locks[i+1] = true;
			}
		}
	}
	return;
}

void unlock(int i, bool *locks) {
	#pragma omp critical (LockRegion)
	{
		locks[i-1] = false; 
		locks[i] = false; 
		locks[i+1] = false;
	}
}
#endif



int main(int argc, char **argv) 
{

	int i, j, n, num_zombies;
	double aux_rand;
	bool *locks = new bool[SIZE + 2];
	GridCell ***MeshA, ***MeshB, ***aux;

	srand48(8767134);

	MeshA = (GridCell***)malloc((SIZE+2)*(sizeof(GridCell**)));
	MeshB = (GridCell***)malloc((SIZE+2)*(sizeof(GridCell**)));
	for (i = 0; i < SIZE + 2; i++) locks[i] = false;
	
	initializeMesh(MeshA, MeshB);
	num_zombies = fillMesh(MeshA);

	std::cout<<"Time"<<"\t"<<"Population"<<" Zombies: "<< num_zombies <<std::endl;
	printPopulation(MeshA, 1);
	
	/*
	Main loop
	*/
	for (n = 1; n <= YEARS*STEPS; n++) 
	{
		if(n % 50 == 0) printPopulation(MeshA, n);
		/*
		For each timestep, calculates a new probability of
		birth and death based on current population size.
		Also resets the number of babies.
		*/
		double 	prob_birth 	= getBirthRate(MeshA)/(double)getPairingNumber(MeshA);
		double 	prob_death 	= getDeathRate(MeshA)/(double)getPopulation(MeshA);
		int 	babycounter = 0;
		
		/*
		Parallel for loop. "babycounter" is firstprivate because each thread will count its
		own number of babies and they have to receive an initialized value of this variable.
		*/
		#if defined(_OPENMP)
		#pragma omp parallel for default(none) firstprivate(babycounter) shared(MeshA, MeshB, locks, n, prob_birth, prob_death)
		#endif

		for (i = 1; i <= SIZE; i++) 
		{
			#if defined(_OPENMP)
			lock(i, locks);
			#endif

			for (int j = 1; j <= SIZE; j++) 
			{
				//Place new humans in empty cells
				if(MeshA[i][j]->isEmpty() == TRUE && babycounter > 0)
				{
					if(drand48() < (NT_MALE_PERCENTAGE/100)) 
						MeshB[i][j]->setToHuman(new Human(MALE, n, YOUNG, HEALTHY));
					else 
						MeshB[i][j]->setToHuman(new Human(FEMALE, n, YOUNG, HEALTHY));
					
					babycounter -= 1;
					continue;
				}
				
				//For each grid cell, we do:
				if(MeshA[i][j]->isHuman() == TRUE)
				{
					if(MeshA[i][j]->getHuman()->getStatus() == EXPOSED)
					{
						if((n - MeshA[i][j]->getHuman()->getExposedDate()) > 0)
							MeshA[i][j]->setToZombie(new Zombie(n));
					}
					else
					{
						if(executeBirthControl(MeshA, i, j, prob_birth) == TRUE)
							babycounter ++;
					}
				}

				if(MeshA[i][j]->isZombie() == TRUE) 
					executeInfection(MeshA, i, j, n);	
				
				executeDeathControl(MeshA, i, j, prob_death, n);

				executeMovement(MeshA, MeshB, i, j);
			}
			#if defined(_OPENMP)
			unlock(i, locks);
			#endif
		}
		/*End of parallel.*/
		/*Deal with outliers*/
		proccessBoundaries(MeshB);

		/*Swap pointers*/
		aux = MeshB;
		MeshB = MeshA;
		MeshA = aux;
		
		char str[100];
		sprintf(str, "step%05d", n);
		if(n % 50 == 0) printToBitmap(MeshA, str, SIZE+2, SIZE+2);
	}

	char str[100];
	sprintf(str, "step%05d", n);
	printToBitmap(MeshA, str, SIZE+2, SIZE+2);

	return 0;
}
