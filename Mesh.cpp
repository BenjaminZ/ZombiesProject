#include "mesh.h"

class Zombie
{
	private:
		int turningdate;
	public:
		Zombie(int aux)
		{
			turningdate = aux;
		}

		int getTurningDate()
		{
			return turningdate;
		}
};

class Human
{
	private:
		int gender;
		int birthdate;
		int status;
		int exposed_date;
		int ageModifier;

	public:
		Human(int aux_gender, int aux_birth, int aux_status)
		{
			gender = aux_gender;
			birthdate = aux_birth;
			status = aux_status;
			exposed_date = 0;
			ageModifier = 0;
			return;
		}
		int getGender()
		{
			return gender;
		}
		int getBirthDate()
		{
			return birthdate;
		}
		int getStatus()
		{
			return status;
		}
		int getAgeModifier()
		{
			return ageModifier;
		}
		void infect(int n)
		{
			status = EXPOSED;
			exposed_date = n;
			return;
		}
		int getExposedDate()
		{
			return exposed_date;
		}
};

class GridCell
{
	private:
		Zombie* z;
		Human* h;

	public:
		GridCell()
		{
			z = NULL;
			h = NULL;
		}

		void setToHuman(Human *aux)
		{
			if(z != NULL)
			{
				delete z;
				z = NULL;
			}
			h = aux;
			return;
		}
		void setToZombie(Zombie *aux)
		{
			if(h != NULL)
			{
				delete h;
				h = NULL;
			}
			z = aux;
			return;
		}

		int isEmpty()
		{
			if(h != NULL || z != NULL) return FALSE;
			else return TRUE;
		}

		int isHuman()
		{
			if(h != NULL) return TRUE;
			else return FALSE;
		}

		int isZombie()
		{
			if(z != NULL) return TRUE;
			else return FALSE;
		}
		Human* getHuman()
		{
			return h;
		}
		Zombie* getZombie()
		{
			return z;
		}
		int getStatus()
		{
			if(h != NULL)
			{
				return h->getGender();
			}
			if(z != NULL)
			{
				return ZOMBIE;
			}
			else return EMPTY;
		}
};

int getPopulation(GridCell*** Mesh)
{
	int populationTotal = 0;
	for (int i = 1; i <= SIZE; i++) 
		for (int j = 1; j <= SIZE; j++)
			if (Mesh[i][j]->isEmpty() == FALSE) populationTotal++;
	return populationTotal;
}

double getNumberOfPairs(GridCell*** Mesh)
{
	int i, j;
	double male, female;
	double no_female, paired_female, allcells;
	double no_male, paired_male;
	double pob_male, pob_female;

	for(i = 1; i <= SIZE; i++)
	{
		for(j = 1; j <= SIZE; j++)
		{
			if(Mesh[i][j]->getStatus() == MALE) male += 1;
			else if(Mesh[i][j]->getStatus() == FEMALE) female += 1;
		}
	}
	allcells = (double)(SIZE*SIZE);
	
	no_female = (allcells - female)/allcells;
	paired_female = 1 - pow(no_female, 4);
	pob_female = female*paired_female;

	no_male = (allcells - male)/allcells;
	paired_male = 1 - pow(no_male, 4);
	pob_male = male*paired_male;

	if(pob_male > pob_female) return pob_female;
	else return pob_male;
}

double getBirthRate(GridCell*** Mesh)
{
	return ((double)getPopulation(Mesh))*(NT_BIRTHS_PER_DAY)/(NT_POP);
}

double getDeathRate(GridCell*** Mesh)
{
	return ((double)getPopulation(Mesh))*(NT_DEATHS_PER_DAY)/(NT_POP);
}

void printPopulation(GridCell*** Mesh, int t) 
{
	int populationTotal = 0;
	for (int i = 1; i <= SIZE; i++) 
		for (int j = 1; j <= SIZE; j++)
			if (Mesh[i][j]->isHuman() == TRUE) populationTotal++;

	cout << t << "\t" << populationTotal << std::endl;
	return;
}

void printToBitmap(GridCell*** Mesh, char* str, int w, int h)
{
	FILE *f;
	unsigned char *img = NULL;
	int r, g, b, red[w][h], green[w][h], blue[w][h];
	int x, y;
	int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
	if( img )
	    free( img );
	img = (unsigned char *)malloc(3*w*h);
	memset(img,0,sizeof(img));

	for(int i= 0; i<w; i++)
	{
		for(int j=0; j<h; j++)
		{
				switch(Mesh[i][j]->getStatus())
				{
					case MALE:
						red[i][j] = 0;
						green[i][j] = 0;
						blue[i][j] = 255;
						break;
					case FEMALE:
						red[i][j] = 0;
						green[i][j] = 255;
						blue[i][j] = 0;
						break;
					case ZOMBIE:
						red[i][j] = 255;
						green[i][j] = 0;
						blue[i][j] = 0;
						break;
					default:
						red[i][j] = 255;
						green[i][j] = 255;
						blue[i][j] = 255;
						break;
				}
		}
	}
	for(int i=0; i<w; i++)
	{
	    for(int j=0; j<h; j++)
		{
		    x=i; 
		    y=(h-1)-j;
		    r = red[i][j]*255;
		    g = green[i][j]*255;
		    b = blue[i][j]*255;
		    if (r > 255) r=255;
		    if (g > 255) g=255;
		    if (b > 255) b=255;
		    img[(x+y*w)*3+2] = (unsigned char)(r);
		    img[(x+y*w)*3+1] = (unsigned char)(g);
		    img[(x+y*w)*3+0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);

	f = fopen(str,"wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	
	for(int i=0; i<h; i++)
	{
	    fwrite(img+(w*(h-i-1)*3),3,w,f);
	    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}
	
	fclose(f);
}

void proccessBoundaries(GridCell*** Mesh)
{
	/*No-Flux based boundary*/
	int i, j;
	GridCell *aux;

	/*Top boundary*/
	for(j = 1; j < SIZE+1; j++)
	{
		/*Found a human/zombie outside the grid*/
		if(Mesh[0][j]->isEmpty() == FALSE)
		{
			/*If the original place is empty, put it back*/
			if(Mesh[1][j]->isEmpty() == TRUE)
			{
				aux = Mesh[0][j];
				Mesh[0][j] = new GridCell();
				delete Mesh[1][j];
				Mesh[1][j] = aux;
			}
			else if(j > 1 && j < SIZE)
			{
				/*Checks the left and right of original place for empty cells*/
				if(Mesh[1][j+1]->isEmpty() == TRUE)
				{
					aux = Mesh[0][j];
					Mesh[0][j] = new GridCell();
					delete Mesh[1][j+1];
					Mesh[1][j+1] = aux;
				}
				else if(Mesh[1][j-1]->isEmpty() == TRUE)
				{
					aux = Mesh[0][j];
					Mesh[0][j] = new GridCell();
					delete Mesh[1][j-1];
					Mesh[1][j-1] = aux;
				}
				else
				{
					/*No empty cells found. Delete it.*/
					delete Mesh[0][j];
					Mesh[0][j] = new GridCell();
				}
			}
			else if(j == 1)
			{
				/*Checks the right of original place for empty cell*/
				if(Mesh[1][j+1]->isEmpty() == TRUE)
				{
					aux = Mesh[0][j];
					Mesh[0][j] = new GridCell();
					delete Mesh[1][j+1];
					Mesh[1][j+1] = aux;
				}
				else
				{
					/*No empty cells found. Delete it.*/
					delete Mesh[0][j];
					Mesh[0][j] = new GridCell();
				}
			}
			else if(j == SIZE)
			{
				/*Checks the left of original place for empty cell*/
				if(Mesh[1][j-1]->isEmpty() == TRUE)
				{
					aux = Mesh[0][j];
					Mesh[0][j] = new GridCell();
					delete Mesh[1][j-1];
					Mesh[1][j-1] = aux;
				}
				else
				{
					/*No empty cells found. Delete it.*/
					delete Mesh[0][j];
					Mesh[0][j] = new GridCell();
				}
			}
			else
			{
				/*No empty cells found. Delete it.*/
				delete Mesh[0][j];
				Mesh[0][j] = new GridCell();
			}
		}
	}
	/*Bottom boundary*/
	for(j = 1; j < SIZE+1; j++)
	{
		/*Found a human/zombie outside the grid*/
		if(Mesh[SIZE+1][j]->isEmpty() == FALSE)
		{
			/*If the original place is empty, put it back*/
			if(Mesh[SIZE][j]->isEmpty() == TRUE)
			{
				aux = Mesh[SIZE+1][j];
				Mesh[SIZE+1][j] = new GridCell();
				delete Mesh[SIZE][j];
				Mesh[SIZE][j] = aux;
			}
			else if(j > 1 && j < SIZE)
			{
				if(Mesh[SIZE][j-1]->isEmpty() == TRUE)
				{
					aux = Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();
					delete Mesh[SIZE][j-1];
					Mesh[SIZE][j-1] = aux;
				}
				else if(Mesh[SIZE][j+1]->isEmpty() == TRUE)
				{
					aux = Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();
					delete Mesh[SIZE][j+1];
					Mesh[SIZE][j+1] = aux;
				}
				else
				{
					delete Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();	
				}
			}
			else if(j == 1)
			{
				if(Mesh[SIZE][j+1]->isEmpty() == TRUE)
				{
					aux = Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();
					delete Mesh[SIZE][j+1];
					Mesh[SIZE][j+1] = aux;
				}
				else
				{
					delete Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();	
				}
			}
			else if(j == SIZE)
			{
				if(Mesh[SIZE][j-1]->isEmpty() == TRUE)
				{
					aux = Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();
					delete Mesh[SIZE][j-1];
					Mesh[SIZE][j-1] = aux;
				}
				else
				{
					delete Mesh[SIZE+1][j];
					Mesh[SIZE+1][j] = new GridCell();	
				}
			}
			else
			{
				delete Mesh[SIZE+1][j];
				Mesh[SIZE+1][j] = new GridCell();	
			}
		}
	}
	/*Left boundary*/
	for(i = 1; i < SIZE+1; i++)
	{
		/*Found a human/zombie outside the grid*/
		if(Mesh[i][0]->isEmpty() == FALSE)
		{
			/*If the original place is empty, put it back*/
			if(Mesh[i][1]->isEmpty() == TRUE)
			{
				aux = Mesh[i][0];
				Mesh[i][0] = new GridCell();
				delete Mesh[i][1];
				Mesh[i][1] = aux; 
			}
			else if(i > 1)
			{
				if(Mesh[i-1][1]->isEmpty() == TRUE)
				{
					aux = Mesh[i][0];
					Mesh[i][0] = new GridCell();
					delete Mesh[i-1][1];
					Mesh[i-1][1] = aux;
				}
				else if(Mesh[i+1][1]->isEmpty() == TRUE)
				{
					aux = Mesh[i][0];
					Mesh[i][0] = new GridCell();
					delete Mesh[i+1][1];
					Mesh[i+1][1] = aux;
				}
				else
				{
					delete Mesh[i][0];
					Mesh[i][0] = new GridCell();
				}
			}
			else if(i == 1)
			{
				if(Mesh[i-1][1]->isEmpty() == TRUE)
				{
					aux = Mesh[i][0];
					Mesh[i][0] = new GridCell();
					delete Mesh[i-1][1];
					Mesh[i-1][1] = aux;
				}
				else
				{
					delete Mesh[i][0];
					Mesh[i][0] = new GridCell();
				}
			}
			else if(i == SIZE)
			{
				if(Mesh[i+1][1]->isEmpty() == TRUE)
				{
					aux = Mesh[i][0];
					Mesh[i][0] = new GridCell();
					delete Mesh[i+1][1];
					Mesh[i+1][1] = aux;
				}
				else
				{
					delete Mesh[i][0];
					Mesh[i][0] = new GridCell();
				}
			}
			else
			{
				delete Mesh[i][0];
				Mesh[i][0] = new GridCell();
			}
		}
	}
	/*Right boundary*/
	for(i = 1; i < SIZE+1; i++)
	{
		if(Mesh[i][SIZE+1]->isEmpty() == FALSE)
		{
			if(Mesh[i][SIZE]->isEmpty() == TRUE)
			{
				aux = Mesh[i][SIZE+1];
				Mesh[i][SIZE+1] = new GridCell();
				delete Mesh[i][SIZE];
				Mesh[i][SIZE] = aux; 
			}
			else if(i > 1 && i < SIZE)
			{
				if(Mesh[i-1][SIZE]->isEmpty() == TRUE)
				{
					aux = Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
					delete Mesh[i-1][SIZE];
					Mesh[i-1][SIZE] = aux;	
				}
				else if(Mesh[i+1][SIZE]->isEmpty() == TRUE)
				{
					aux = Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
					delete Mesh[i+1][SIZE];
					Mesh[i+1][SIZE] = aux;
				}
				else
				{
					delete Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
				}
			}
			else if(i == 1)
			{
				if(Mesh[i+1][SIZE]->isEmpty() == TRUE)
				{
					aux = Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
					delete Mesh[i+1][SIZE];
					Mesh[i+1][SIZE] = aux;
				}
				else
				{
					delete Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
				}
			}
			else if(i == SIZE)
			{
				if(Mesh[i-1][SIZE]->isEmpty() == TRUE)
				{
					aux = Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
					delete Mesh[i-1][SIZE];
					Mesh[i-1][SIZE] = aux;	
				}
				else
				{
					delete Mesh[i][SIZE+1];
					Mesh[i][SIZE+1] = new GridCell();
				}
			}
			else
			{
				delete Mesh[i][SIZE+1];
				Mesh[i][SIZE+1] = new GridCell();
			}
		}
	}
	return;
}

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

	int i, j, n, num_zombies = 0;
	double aux_rand;
	bool *locks = new bool[SIZE + 2];
	GridCell ***MeshA, ***MeshB, ***aux;

	MeshA = (GridCell***)malloc((SIZE+2)*(sizeof(GridCell**)));
	MeshB = (GridCell***)malloc((SIZE+2)*(sizeof(GridCell**)));
	
	for(i = 0; i < SIZE+2; i++)
	{
		MeshA[i] = (GridCell**)malloc((SIZE+2)*(sizeof(GridCell*)));
		MeshB[i] = (GridCell**)malloc((SIZE+2)*(sizeof(GridCell*)));
	}
	
	for(i = 0; i < SIZE+2; i++)
	{
		for(j = 0; j < SIZE+2; j++)
		{
			MeshA[i][j] = new GridCell();
			MeshB[i][j] = new GridCell();
		}
	}
	//omp_get_thread_number();
	srand48(8767135);

	for (i = 0; i < SIZE + 2; i++) locks[i] = false;
	
	for (i = 1; i <= SIZE; i++)
	{ 
		for (j = 1; j <= SIZE; j++)
		{
			aux_rand = drand48();
			if (aux_rand < 0.171)
			{				
				if(drand48() < 0.525) MeshA[i][j]->setToHuman(new Human(MALE, 0, 0));
				else MeshA[i][j]->setToHuman(new Human(FEMALE, 0, 0));
			}
			else if(aux_rand >= 0.999992)
			{
				MeshA[i][j]->setToZombie(new Zombie(0));
				num_zombies += 1;
			}
		}
	}

	std::cout<<"Time"<<"\t"<<"Population"<<" Zombies:"<< num_zombies <<std::endl;
	
	printPopulation(MeshA, 0);
	
	for (n = 0; n < YEARS*STEPS; n++) 
	{
<<<<<<< HEAD
		if(n % 100 == 0) printPopulation(MeshA, n);
		double prob_birth = 1.0*getBirthRate(MeshA)/getNumberOfPairs(MeshA);
=======
		if((n+1) % 30 == 0) printPopulation(MeshA, n+1);
		
		double prob_birth = getBirthRate(MeshA)/getNumberOfPairs(MeshA);
		double prob_death = getDeathRate(MeshA)/getPopulation(MeshA);

>>>>>>> 2051a8eaa02917168225c19c0dcffa1c1131c9b4
		int babycounter = 0;
		
		//Parallel pragmas
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
				//check for human status
				if(MeshA[i][j]->isEmpty() == TRUE && babycounter > 0)
				{
					if(drand48() < 0.525) MeshB[i][j]->setToHuman(new Human(MALE, 0, 0));
					else MeshB[i][j]->setToHuman(new Human(FEMALE, 0, 0));
					
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

				if(MeshA[i][j]->isZombie() == TRUE) executeInfection(MeshA, i, j, n);	
				
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

		if(n % 30 == 0) printToBitmap(MeshA, str, SIZE+2, SIZE+2);
	}
	char str[100];
	sprintf(str, "step%05d", n);
	printToBitmap(MeshA, str, SIZE+2, SIZE+2);

	return 0;
}
