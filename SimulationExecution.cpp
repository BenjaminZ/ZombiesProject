/*
File: SimulationExecution.cpp
*/
#include "SimulationExecution.hpp"

int executeBirthControl(GridCell*** MeshA, int i, int j, double prob_birth, MTRand* mtwister)
{
	int foundPair;
	int checked;
	double prob;

	prob = mtwister->randExc();

	if(MeshA[i][j]->getStatus() == MALE)
		if(MeshA[i-1][j]->getStatus() == FEMALE || MeshA[i][j+1]->getStatus() == FEMALE || 
			MeshA[i+1][j]->getStatus() == FEMALE || MeshA[i][j-1]->getStatus() == FEMALE)
			if(prob < prob_birth) return TRUE;
	else
		if(MeshA[i-1][j]->getStatus() == MALE || MeshA[i][j+1]->getStatus() == MALE || 
			MeshA[i+1][j]->getStatus() == MALE || MeshA[i][j-1]->getStatus() == MALE)
			if(prob < prob_birth) return TRUE;	

	return FALSE;
}

void executeMovement(GridCell*** MeshA, GridCell*** MeshB, int i, int j, MTRand* mtwister)
{
	/*
	flags[0] - Move up
	flags[1] - Move down
	flags[2] - Move left
	flags[3] - Move right
	*/
	int flags[4] = {FALSE, FALSE, FALSE, FALSE};
	double move, range, offset, step;
	GridCell* aux_cell;
	/*
	Checks if cell is not empty.
	*/
	if(MeshA[i][j]->isEmpty() == FALSE) 
	{ 
		aux_cell = MeshA[i][j];
		MeshA[i][j] = new GridCell();
		/*
		Define the movement probability based on Human(Age)/Zombie
		*/
		if(aux_cell->isHuman())
		{
			switch(aux_cell->getHuman()->getAgeGroup())
			{
				case YOUNG:
					step = YOUNG_MOVE;
					break;
				case ADULT:
					step = ADULT_MOVE;
					break;
				case ELDER:
					step = ELDER_MOVE;
					break;
			}
		}
		else if(aux_cell->isZombie()) step = ZOMBIE_MOVE;

		/*
		Define the direction of movement.
		*/
		move = mtwister->randExc();
		range = 4.0*step;
		offset = (1.0 - range)/2.0;
		if(move >= offset && move < offset + 1.0*step) flags[0] = TRUE;
		else if(move >= offset + 1.0*step && move < offset + 2.0*step) flags[1] = TRUE;
		else if(move >= offset + 2.0*step && move < offset + 3.0*step) flags[2] = TRUE;
		else if(move >= offset + 3.0*step && move < offset + 4.0*step) flags[3] = TRUE;
		/*
		Move up
		*/
		if(flags[0] && MeshA[i-1][j]->isEmpty() == TRUE && MeshB[i-1][j]->isEmpty() == TRUE) 
		{
			delete MeshB[i-1][j];
			MeshB[i-1][j] = aux_cell;
		} 
		/*
		Move down
		*/
		else if(flags[1] && MeshA[i+1][j]->isEmpty() == TRUE && MeshB[i+1][j]->isEmpty() == TRUE) 
		{
			delete MeshB[i+1][j];
			MeshB[i+1][j] = aux_cell;
		} 
		/*
		Move left
		*/
		else if(flags[2] && MeshA[i][j-1]->isEmpty() == TRUE && MeshB[i][j-1]->isEmpty() == TRUE)
		{
			delete MeshB[i][j-1];
			MeshB[i][j-1] = aux_cell;
		} 
		/*
		Move right
		*/
		else if(flags[3] && MeshA[i][j+1]->isEmpty() == TRUE && MeshB[i][j+1]->isEmpty() == TRUE) 
		{
			delete MeshB[i][j+1];
			MeshB[i][j+1] = aux_cell;
		}
		/*
		Does not move
		*/
		else 
		{
			delete MeshB[i][j];
			MeshB[i][j] = aux_cell;
		}
	}
	return;
}

void executeInfection(GridCell*** Mesh, int i, int j, int n, MTRand* mtwister)
{
	double random_num, random_inf, vector_aux, aux;
	
	vector<Human*> humans;

	/*
	This checks if the zombie will infect someone or get himself killed
	*/
	random_inf = mtwister->randExc();
	if(random_inf > INFECTION_PROB && random_inf < 1 - KILL_ZOMBIE) return;
	
	/*
	The code below stores all possible humans to be infected.
	*/
	if(Mesh[i-1][j]->isHuman() == TRUE) humans.push_back(Mesh[i-1][j]->getHuman());
	if(Mesh[i][j+1]->isHuman() == TRUE) humans.push_back(Mesh[i][j+1]->getHuman());
	if(Mesh[i+1][j]->isHuman() == TRUE) humans.push_back(Mesh[i+1][j]->getHuman());
	if(Mesh[i][j-1]->isHuman() == TRUE) humans.push_back(Mesh[i][j-1]->getHuman());

	/*
	No humans found nearby to be infected or to kill the zombie.
	*/
	if(humans.size() == 0) return;
	
	/*
	Rolls a dice and checks which human gets infected or kills the zombie.
	*/
	else
	{
		/*
		Kills a zombie.
		*/
		if(random_inf >= (1 - KILL_ZOMBIE))
		{
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
			return;
		}

		random_num = mtwister->randExc();
		/*
		aux possible values: 1, 0.5, 0.33, 0.25
		*/
		aux = 1/((double)humans.size());

		/*
		Chooses who to infect.
		*/
		for(i = 0, vector_aux = aux; i < humans.size(); i++)
		{
			if(random_num < vector_aux)
			{
				humans[i]->infect(n);
				break;
			}
			else
			{
				vector_aux += aux;
				continue;
			}
		}
	}
	humans.clear();
	return;
}

void executeDeathControl(GridCell*** Mesh, int i, int j, double* prob_death, int n, MTRand* mtwister)
{
	double prob_kill;

	if(Mesh[i][j]->isHuman() == TRUE)
	{
		prob_kill = mtwister->randExc();

		switch(Mesh[i][j]->getHuman()->getAgeGroup())
		{
			case YOUNG:
				if(prob_kill >= 0.5 && prob_kill < 0.5 + NT_YOUNG_DEATH*prob_death)
				{
					delete Mesh[i][j];
					Mesh[i][j] = new GridCell();
				}
				break;
			case ADULT:
				if(prob_kill >= 0.5 && prob_kill < 0.5 + NT_ADULT_DEATH*prob_death)
				{
					delete Mesh[i][j];
					Mesh[i][j] = new GridCell();
				}
				break;
			case ELDER:
				if(prob_kill >= 0.5 && prob_kill < 0.5 + NT_ELDER_DEATH*prob_death)
				{
					delete Mesh[i][j];
					Mesh[i][j] = new GridCell();
				}
				break;
		}

		if (prob_kill >= 0.5 && prob_kill < 0.5 + (2.84 / 235182))
		{
<<<<<<< HEAD:Sim_functions.cpp
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
=======
			case YOUNG:
				if(prob_kill < prob_death[0])
				{
					delete Mesh[i][j];
					Mesh[i][j] = new GridCell();
				}
				break;
			case ADULT:
				if(prob_kill < prob_death[1])
				{
					delete Mesh[i][j];
					Mesh[i][j] = new GridCell();
				}
				break;
			case ELDER:
				if(prob_kill < prob_death[2])
				{
					delete Mesh[i][j];
					Mesh[i][j] = new GridCell();
				}
				break;
>>>>>>> 2b94ce6adac4276ed4dbfa4f0e871eb08d69cb11:SimulationExecution.cpp
		}
	}
	else if(Mesh[i][j]->isZombie() == TRUE)
	{
		if((n - Mesh[i][j]->getZombie()->getTurningDate()) > ZOMBIE_LIFESPAN*365)
		{
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
		}
	}
	return;
}