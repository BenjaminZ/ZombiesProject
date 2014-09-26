/*
File: Sim_functions.hpp
*/
#include "Sim_functions.hpp"

int executeBirthControl(GridCell*** MeshA, int i, int j, double prob_birth, MTRand* mtwister)
{
	int foundPair;
	int checked;
	double prob;

	prob = mtwister->randExc();

	if(prob > prob_birth) return FALSE;

	else
	{
		if(MeshA[i][j]->getStatus() == MALE)
		{
			if(MeshA[i-1][j]->getStatus() == FEMALE || MeshA[i][j+1]->getStatus() == FEMALE || 
				MeshA[i+1][j]->getStatus() == FEMALE || MeshA[i][j-1]->getStatus() == FEMALE)
				return TRUE;
		}
		else
		{
			if(MeshA[i-1][j]->getStatus() == MALE || MeshA[i][j+1]->getStatus() == MALE || 
				MeshA[i+1][j]->getStatus() == MALE || MeshA[i][j-1]->getStatus() == MALE)
				return TRUE;
		}
	}
	return FALSE;
}

void executeMovement(GridCell*** MeshA, GridCell*** MeshB, int i, int j, MTRand* mtwister)
{
	if(MeshA[i][j]->isEmpty() == FALSE) 
	{ 
		GridCell* aux_cell = MeshA[i][j];

		MeshA[i][j] = new GridCell();

		double move = mtwister->randExc();

		if (move < 1.0*MOVE && MeshA[i-1][j]->isEmpty() == TRUE && MeshB[i-1][j]->isEmpty() == TRUE) 
		{
			delete MeshB[i-1][j];
			MeshB[i-1][j] = aux_cell;
		} 
		else if (move < 2.0*MOVE && MeshA[i+1][j]->isEmpty() == TRUE && MeshB[i+1][j]->isEmpty() == TRUE) 
		{
			delete MeshB[i+1][j];
			MeshB[i+1][j] = aux_cell;
		} 
		else if (move < 3.0*MOVE && MeshA[i][j-1]->isEmpty() == TRUE && MeshB[i][j-1]->isEmpty() == TRUE) 
		{
			delete MeshB[i][j-1];
			MeshB[i][j-1] = aux_cell;
		} 
		else if (move < 4.0*MOVE && MeshA[i][j+1]->isEmpty() == TRUE && MeshB[i][j+1]->isEmpty() == TRUE) 
		{
			delete MeshB[i][j+1];
			MeshB[i][j+1] = aux_cell;
		}
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

void executeDeathControl(GridCell*** Mesh, int i, int j, double prob_death, int n, MTRand* mtwister)
{
	double prob_kill;

	if(Mesh[i][j]->isHuman() == TRUE)
	{
		prob_kill = mtwister->randExc();

		if(prob_kill < prob_death)
		{
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
		}
	}
	else if(Mesh[i][j]->isZombie() == TRUE)
	{
		if((n - Mesh[i][j]->getZombie()->getTurningDate()) > 4*365)
		{
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
		}
	}
	return;
}