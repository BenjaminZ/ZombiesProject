/*
File: zombiesim_main.cpp
*/
#include <omp.h>
#include "MersenneTwister.h"
#include "zombiesim_parameters.hpp"
#include "Human.hpp"
#include "Zombie.hpp"
#include "GridCell.hpp"
#include "Sim_functions.hpp"
#include "Mesh_functions.hpp"

#if defined(_OPENMP)
void lock(int i, bool *locks) 
{
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

void unlock(int i, bool *locks) 
{
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
	char str[100];
	int i, j, n, num_zombies;
	double aux_rand;
	bool *locks = new bool[SIZE + 2];
	GridCell ***MeshA, ***MeshB, ***aux;
	MTRand mt_thread[64]; //Max of 64 threads

	/*
	Initializes the MersenneTwister PRNG.
	*/
	for(i = 0; i < 64; i++) mt_thread->seed(i);
	/*
	Creates and initializes MeshA and MeshB.
	Fills MeshA with the correspondent starting density of
	humans and zombies.
	*/
	MeshA = (GridCell***)malloc((SIZE+2)*(sizeof(GridCell**)));
	MeshB = (GridCell***)malloc((SIZE+2)*(sizeof(GridCell**)));
	for (i = 0; i < SIZE + 2; i++) locks[i] = false;
	
	initializeMesh(MeshA, MeshB);
	num_zombies = fillMesh(MeshA, &mt_thread[0]);

	std::cout << "Time" << "\t" << "Male\tFemale\tZombies"<<std::endl;
	printPopulation(MeshA, 1);
	
	sprintf(str, "inf_prob_%.2lf_step%05d.bmp", INFECTION_PROB, 1);
	printToBitmap(MeshA, str, SIZE+2, SIZE+2);
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
		//double 	prob_birth 	= getBirthRate(MeshA)/(double)getPairingNumber(MeshA);
		double	prob_birth	= 1.97/(double)getPopulation(MeshA);
		double 	prob_death 	= getDeathRate(MeshA)/(double)getPopulation(MeshA);
		int 	babycounter = 0;
		
		/*
		Parallel for loop. "babycounter" is firstprivate because each thread 
		will count its own number of babies and they have to receive an 
		initialized value of this variable.
		*/
		#if defined(_OPENMP)
		#pragma omp parallel for default(none) firstprivate(babycounter) shared(mt_thread, MeshA, MeshB, locks, n, prob_birth, prob_death)
		#endif

		for (i = 1; i <= SIZE; i++) 
		{
			#if defined(_OPENMP)
			lock(i, locks);
			#endif
			
			int num_thread = omp_get_thread_num();

			for (int j = 1; j <= SIZE; j++) 
			{
				/*
				Place new humans in empty cells.
				*/
				if(MeshA[i][j]->isEmpty() == TRUE && babycounter > 0)
				{
					if(mt_thread[num_thread].randExc() < (NT_MALE_PERCENTAGE/100)) 
						MeshB[i][j]->setToHuman(new Human(MALE, n, YOUNG, HEALTHY));
					else 
						MeshB[i][j]->setToHuman(new Human(FEMALE, n, YOUNG, HEALTHY));
					
					babycounter -= 1;
					continue;
				}
				
				/*
				Execute human specific actions.
				*/
				if(MeshA[i][j]->isHuman() == TRUE)
				{
					/*
					Change a exposed human for a zombie.
					*/
					if(MeshA[i][j]->getHuman()->getStatus() == EXPOSED)
					{
						if((n - MeshA[i][j]->getHuman()->getExposedDate()) > 0)
						{
							delete MeshA[i][j];
							MeshA[i][j] = new GridCell();
							MeshB[i][j]->setToZombie(new Zombie(n));
							continue;
						}
					}
					/*
					Checks if a human will reproduce.
					*/
					else
					{
						if(executeBirthControl(MeshA, i, j, prob_birth, &mt_thread[num_thread]) == TRUE)
							babycounter ++;
					}
				}
				/*
				A zombie can infect one human per timestep.
				*/
				if(MeshA[i][j]->isZombie() == TRUE) 
					executeInfection(MeshA, i, j, n, &mt_thread[num_thread]);	
				
				/*
				Humans and Zombies live for a limited lifespan.
				*/
				executeDeathControl(MeshA, i, j, prob_death, n, &mt_thread[num_thread]);

				/*
				They can move up, down, left, right or stay in the same place.
				*/
				executeMovement(MeshA, MeshB, i, j, &mt_thread[num_thread]);
			}
			#if defined(_OPENMP)
			unlock(i, locks);
			#endif
		}
		/*
		End of parallel.
		*/

		/*
		Deal with humans/zombies outside the grid.
		*/
		proccessBoundaries(MeshB);

		/*
		Swap pointers.
		*/
		aux = MeshB;
		MeshB = MeshA;
		MeshA = aux;
		
		sprintf(str, "inf_prob_%.2lf_step%05d.bmp", INFECTION_PROB, n);
		if(n % 50 == 0) printToBitmap(MeshA, str, SIZE+2, SIZE+2);
	}
	sprintf(str, "inf_prob_%.2lf_step%05d.bmp", INFECTION_PROB, n);
	printToBitmap(MeshA, str, SIZE+2, SIZE+2);

	return 0;
}
