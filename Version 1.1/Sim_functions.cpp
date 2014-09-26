/*
File: Sim_functions.hpp
*/

#include "Sim_functions.hpp"

int executeBirthControl(GridCell*** MeshA, int i, int j, double prob_birth)
{
	int foundPair;
	int checked;
	double prob;
	//Grid cells with distance higher than 0 from boundary
	prob = (double)drand48();
	//Center cells
	if(MeshA[i][j]->getStatus() == MALE)
	{
		if(MeshA[i-1][j]->getStatus() == FEMALE || MeshA[i][j+1]->getStatus() == FEMALE || 
			MeshA[i+1][j]->getStatus() == FEMALE || MeshA[i][j-1]->getStatus() == FEMALE)
		{
			
			if(prob < prob_birth) return TRUE;
		}
	}
	else
	{
		if(MeshA[i-1][j]->getStatus() == MALE || MeshA[i][j+1]->getStatus() == MALE || 
			MeshA[i+1][j]->getStatus() == MALE || MeshA[i][j-1]->getStatus() == MALE)
		{
			if(prob < prob_birth) return TRUE;
		}
	}
	return FALSE;
}

void executeMovement(GridCell*** MeshA, GridCell*** MeshB, int i, int j)
{
	if(MeshA[i][j]->isEmpty() == FALSE) 
	{ 
		GridCell *aux_cell = MeshA[i][j];
		MeshA[i][j] = new GridCell();

		double move = drand48();

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

void executeInfection(GridCell*** MeshA, int i, int j, int n)
{
	double random_num, random_inf, vector_aux, aux;
	vector<Human*> humans;

	/*
	This checks if the zombie will infect someone.
	*/
	random_inf = drand48();

	if(random_inf > INFECTION_PROB) return;

	if(random_inf >= 1 - KILL_ZOMBIE)
	{
		delete MeshA[i][j];
		MeshA[i][j] = new GridCell();
		return;
	}
	/*
	The code below stores all possible humans to be infected.
	*/
	if(MeshA[i-1][j]->isHuman() == TRUE) humans.push_back(MeshA[i-1][j]->getHuman());
	if(MeshA[i][j+1]->isHuman() == TRUE) humans.push_back(MeshA[i][j+1]->getHuman());
	if(MeshA[i+1][j]->isHuman() == TRUE) humans.push_back(MeshA[i+1][j]->getHuman());
	if(MeshA[i][j-1]->isHuman() == TRUE) humans.push_back(MeshA[i][j-1]->getHuman());

	/*
	No humans found nearby to be infected.
	*/
	if(humans.size() == 0) return;
	/*
	Rolls a dice and checks which human gets infected.
	*/
	else
	{
		random_num = drand48();
		//aux possible values: 1, 0.5, 0.33, 0.25
		aux = 1/((double)humans.size());

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

void executeDeathControl(GridCell*** Mesh, int i, int j, double prob_death, int n)
{
	double prob_kill;

	if(Mesh[i][j]->isHuman() == TRUE)
	{
		prob_kill = drand48();

		if(prob_kill < prob_death)
		{
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
		}
	}
	if(Mesh[i][j]->isZombie() == TRUE)
	{
		if((n - Mesh[i][j]->getZombie()->getTurningDate()) > 4*365)
		{
			delete Mesh[i][j];
			Mesh[i][j] = new GridCell();
		}
	}
	return;
}