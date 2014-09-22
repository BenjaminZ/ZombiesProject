#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>

using namespace std;

#define STEPS	1000
#define SIZE	500
#define MOVE	0.1
#define TRUE 1
#define FALSE 0

#define MALE 0
#define FEMALE 1
#define ZOMBIE 2
#define EMPTY 3

class Zombie
{
	private:
		int turningdate;
	public:
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
		int ageModifier;
	public:
		Human(int aux_gender, int aux_birth, int aux_status)
		{
			gender = aux_gender;
			birthdate = aux_birth;
			status = aux_status;
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
		void setStatus(int new_status)
		{
			status = new_status;
			return;
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

void executeMovement(GridCell*** MeshA, GridCell*** MeshB, int i, int j);
int executeBirthControl(GridCell*** MeshA, int i, int j, double pairs);

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
	int male, female, i, j;

	for(i = 1; i <= SIZE; i++)
	{
		for(j = 1; j <= SIZE; j++)
		{
			if(Mesh[i][j]->getStatus() == MALE) male++;
			else if(Mesh[i][j]->getStatus() == FEMALE) female++;
		}
	}
	if(male > female) return (double)female;
	else return (double)male;
}

double getBirthRate(GridCell*** Mesh)
{
	return ((double)getPopulation(Mesh))*(11.2)/(242600.0);
}

void printPopulation(GridCell*** Mesh, int t) 
{
	int populationTotal = 0;
	for (int i = 1; i <= SIZE; i++) 
		for (int j = 1; j <= SIZE; j++)
			if (Mesh[i][j]->isEmpty() == FALSE) populationTotal++;

	cout << t << "\t" << populationTotal << std::endl;
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

	for(int i=0; i<w; i++)
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
		if(Mesh[0][j]->isEmpty() == FALSE)
		{
			if(Mesh[1][j]->isEmpty() == TRUE)
			{
				aux = Mesh[0][j];
				Mesh[0][j] = new GridCell();
				delete Mesh[1][j];
				Mesh[1][j] = aux;
			}
			else
			{
				delete Mesh[0][j];
				Mesh[0][j] = new GridCell();
			}
		}
	}
	/*Bottom boundary*/
	for(j = 1; j < SIZE+1; j++)
	{
		if(Mesh[SIZE+1][j]->isEmpty() == FALSE)
		{
			if(Mesh[SIZE][j]->isEmpty() == TRUE)
			{
				aux = Mesh[SIZE+1][j];
				Mesh[SIZE+1][j] = new GridCell();
				delete Mesh[SIZE][j];
				Mesh[SIZE][j] = aux;
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
		if(Mesh[i][0]->isEmpty() == FALSE)
		{
			if(Mesh[i][1]->isEmpty() == TRUE)
			{
				aux = Mesh[i][0];
				Mesh[i][0] = new GridCell();
				delete Mesh[i][1];
				Mesh[i][1] = aux; 
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
			else
			{
				delete Mesh[i][SIZE+1];
				Mesh[i][SIZE+1] = new GridCell();
			}
		}
	}
	return;
}

int main(int argc, char **argv) {

	int i, j, n;
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

	srand48(8767134);

	for (i = 0; i < SIZE + 2; i++) locks[i] = false;
	
	for (i = 1; i <= SIZE; i++)
	{ 
		for (j = 1; j <= SIZE; j++)
		{
			if (drand48() < 0.171)
			{
				
				if(drand48() < 0.525) MeshA[i][j]->setToHuman(new Human(MALE, 0, 0));
				else MeshA[i][j]->setToHuman(new Human(FEMALE, 0, 0));
			}
		}
	}

	std::cout<<"Time"<<"\t"<<"Population"<<std::endl;

	for (n = 0; n < STEPS; n++) 
	{
		if(n % 100 == 0) printPopulation(MeshA, n);
		double prob_birth = 5.1*getBirthRate(MeshA)/getNumberOfPairs(MeshA);
		int babycounter = 0;
		
		//Parallel pragmas
		#if defined(_OPENMP)
		#pragma omp parallel for default(none) shared(MeshA, MeshB, locks, n, babycounter, prob_birth)
		#endif

		for (i = 1; i <= SIZE; i++) 
		{
			#if defined(_OPENMP)
			lock(i, locks);
			#endif

			for (int j = 1; j <= SIZE; j++) 
			{
				if(MeshA[i][j]->isEmpty() == TRUE && babycounter > 0)
				{
					if(drand48() < 0.525) MeshB[i][j]->setToHuman(new Human(MALE, 0, 0));
					else MeshB[i][j]->setToHuman(new Human(FEMALE, 0, 0));
					babycounter--;
					continue;
				}
				
				//For each grid cell, we do:
				if(MeshA[i][j]->isHuman() == TRUE) 
					if(executeBirthControl(MeshA, i, j, prob_birth) == TRUE)
						babycounter ++;
				
				//executeDeathControl();
				//executeInfection();
				executeMovement(MeshA, MeshB, i, j);
			}
			#if defined(_OPENMP)
			unlock(i, locks);
			#endif
		}
		/*Deal with outliers*/
		proccessBoundaries(MeshB);

		/*Swap pointers*/
		aux = MeshB;
		MeshB = MeshA;
		MeshA = aux;
		
		//char str[100];

		//sprintf(str, "step%d", n);

		//if(n % 10 == 0) printToBitmap(MeshA, str, SIZE, SIZE);
	}
	return 0;
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