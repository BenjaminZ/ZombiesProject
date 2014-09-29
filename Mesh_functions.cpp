/*
File: Mesh_functions.cpp
*/

#include "Mesh_functions.hpp"
/*
getPopulation:
	Calculates the total population of humans.
*/
int getPopulation(GridCell*** Mesh)
{
	int populationTotal = 0;
	for (int i = 1; i <= SIZE; i++) 
		for (int j = 1; j <= SIZE; j++)
			if (Mesh[i][j]->isHuman() == TRUE) populationTotal++;
	return populationTotal;
}

/*
Legacy function:
	calculates the amount of possible existent pairs in the grid.
	Problems:
		if the human population size decreases drastically and humans get
		clustered, there will be a "boom" of babies.
*/
double getPairingNumber(GridCell*** Mesh)
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
/*
getBirthRate:
	adjusts the NT birth rate to the current grid population.
*/
double getBirthRate(GridCell*** Mesh)
{
	return ((double)getPopulation(Mesh))*(NT_BIRTHS_PER_DAY)/(NT_POP);
}

/*
getDeathRate:
	adjusts the NT death rate to the current grid population.
*/
double getDeathRate(GridCell*** Mesh)
{
	return ((double)getPopulation(Mesh))*(NT_DEATHS_PER_DAY)/(NT_POP);
}

/*
printPopulation:
	Print current population of humans and zombies.
*/
void printPopulation(GridCell*** Mesh, int t) 
{
	int male = 0, female = 0, zombies = 0;
	for (int i = 1; i <= SIZE; i++)
	{
		for (int j = 1; j <= SIZE; j++)
		{
			if (Mesh[i][j]->isHuman() == TRUE)
			{
				if(Mesh[i][j]->getHuman()->getGender() == MALE) male++;
				else female++;
			}
			else if(Mesh[i][j]->isZombie() == TRUE) zombies++;
		}
	}

	cout << t << "\t" << male << "\t" << female << "\t" << zombies << std::endl;
	return;
}

/*
initializeMesh:
	create all GridCell objects that initially represents empty cells.
*/
void initializeMesh(GridCell*** MeshA, GridCell*** MeshB)
{
	int i, j;

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
	return;
}

/*
fillMesh:
	this function creates all initial humans/zombies in the mesh.
	The parameters can be changed in "zombiesim_parameters.hpp".
*/
int fillMesh(GridCell*** Mesh, MTRand* mtwister)
{
	int i, j, num_zombies, gender, age, agemodifier, birthdate, counter;
	double aux_rand, acc, step;

	for (num_zombies = 0, i = 1; i <= SIZE; i++)
	{ 
		for (j = 1; j <= SIZE; j++)
		{
			aux_rand = mtwister->randExc();
			
			if (aux_rand < NT_POP_DENSITY)
			{	
				/*
				Define gender.
				*/			
				if(mtwister->randExc() < (NT_MALE_PERCENTAGE/100)) gender = MALE;
				else gender = FEMALE;

				/*
				Define age group.
				*/
				aux_rand = mtwister->randExc();
				if(aux_rand < (NT_YOUNG/100)) agemodifier = YOUNG;
				else if(aux_rand < ((NT_YOUNG+NT_ADULT)/100)) agemodifier = ADULT;
				else agemodifier = ELDER;

				/*
				Define age (in steps prior to the simulation)
				*/
				switch(agemodifier)
				{
					case YOUNG:
						step = 1/(double)(NT_YOUNG_FINAL_AGE);
						aux_rand = mtwister->randExc();
						for(counter = 1, acc = step; acc < 1; acc += step, counter++)
						{
							if(aux_rand < acc)
							{
								age = -365*counter;
								break;
							}
						}
						if(acc >= 1) age = -365*NT_YOUNG_FINAL_AGE;
						break;
					case ADULT:
						step = 1/(double)(NT_ADULT_FINAL_AGE-NT_YOUNG_FINAL_AGE);
						aux_rand = mtwister->randExc();
						for(counter = NT_YOUNG_FINAL_AGE+1, acc = step; acc < 1; acc += step, counter++)
						{
							if(aux_rand < acc)
							{
								age = -365*counter;
								break;
							}
						}
						if(acc >= 1) age = -365*NT_ADULT_FINAL_AGE;
						break;
					case ELDER:
						age = -(NT_ADULT_FINAL_AGE+1)*365;
						break;
				}
				/*
				Creates human.
				*/
				Mesh[i][j]->setToHuman(new Human(gender, age, agemodifier, HEALTHY));
			}
			/*
			This establishes a probability of having 2 zombies.
			*/
			else if(aux_rand < NT_POP_DENSITY + (NUM_OF_ZOMBIES/(double)(SIZE*SIZE)))
			{
				Mesh[i][j]->setToZombie(new Zombie(0));
				num_zombies += 1;
			}
		}
	}
	return num_zombies;
}

/*
Function extracted and adapted from:
http://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
Credits to: http://stackoverflow.com/users/318726/deusmacabre
*/
void printToBitmap(GridCell*** Mesh, char* str, int w, int h)
{
	FILE *f;
	unsigned char *img = NULL;
	int r, g, b, red[w][h], green[w][h], blue[w][h];
	int x, y;
	int filesize = 54 + 3*w*h;
	
	img = (unsigned char *)malloc(3*w*h);
	memset(img,0,sizeof(img));

	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j< h; j++)
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

	for(int i = 0; i < w; i++)
	{
	    for(int j = 0; j < h; j++)
		{
		    x = i; 
		    y = (h-1)-j;
		    r = red[i][j]*255;
		    g = green[i][j]*255;
		    b = blue[i][j]*255;
		    if (r > 255) r = 255;
		    if (g > 255) g = 255;
		    if (b > 255) b = 255;
		    img[(x+y*w)*3+2] = (unsigned char)(r);
		    img[(x+y*w)*3+1] = (unsigned char)(g);
		    img[(x+y*w)*3+0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] 		= {0,0,0};

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
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	
	for(int i = 0; i < h; i++)
	{
	    fwrite(img+(w*(h-i-1)*3),3,w,f);
	    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}
	
	fclose(f);

	return;
}

/*
proccessBoundaries:
	Triple No-Flux based boundary. 
	Checks a minimum of 2 and a maximum of 3 cells for empty places and 
	re-inserts the human/zombie back in the mesh.
*/
void proccessBoundaries(GridCell*** Mesh)
{
	MTRand mtwister;
	int i = 0, j;
	GridCell *aux;
	double odds;

	mtwister.seed(i);
	/*
	Top boundary
	*/
	for(j = 1; j < SIZE+1; j++)
	{
		if(Mesh[0][j]->isEmpty() == FALSE)
		{
			aux = Mesh[0][j];
			Mesh[0][j] = new GridCell();
			
			if(Mesh[1][j]->isEmpty() == TRUE)
			{
				delete Mesh[1][j];
				Mesh[1][j] = aux;
			}
			else if(j > 1 && j < SIZE)
			{
				odds = mtwister.randExc();

				if(odds < 0.5)
				{
					if(Mesh[1][j+1]->isEmpty() == TRUE)
					{
						delete Mesh[1][j+1];
						Mesh[1][j+1] = aux;
					}
					else if(Mesh[1][j-1]->isEmpty() == TRUE)
					{
						
						delete Mesh[1][j-1];
						Mesh[1][j-1] = aux;
					}
					else delete aux;
				}
				else
				{
					if(Mesh[1][j-1]->isEmpty() == TRUE)
					{
						
						delete Mesh[1][j-1];
						Mesh[1][j-1] = aux;
					}
					else if(Mesh[1][j+1]->isEmpty() == TRUE)
					{
						delete Mesh[1][j+1];
						Mesh[1][j+1] = aux;
					}
					else delete aux;
				}
				
			}
			else if(j == 1)
			{
				/*Checks the right of original place for empty cell*/
				if(Mesh[1][j+1]->isEmpty() == TRUE)
				{
					delete Mesh[1][j+1];
					Mesh[1][j+1] = aux;
				}
				else delete aux;
			}
			else if(j == SIZE)
			{
				/*Checks the left of original place for empty cell*/
				if(Mesh[1][j-1]->isEmpty() == TRUE)
				{
					delete Mesh[1][j-1];
					Mesh[1][j-1] = aux;
				}
				else delete aux;
			}
		}
	}
	
	/*
	Bottom boundary
	*/
	for(j = 1; j < SIZE+1; j++)
	{
		if(Mesh[SIZE+1][j]->isEmpty() == FALSE)
		{
			aux = Mesh[SIZE+1][j];
			Mesh[SIZE+1][j] = new GridCell();
			
			if(Mesh[SIZE][j]->isEmpty() == TRUE)
			{
				delete Mesh[SIZE][j];
				Mesh[SIZE][j] = aux;
			}
			else if(j > 1 && j < SIZE)
			{
				odds = mtwister.randExc();

				if(odds < 0.5)
				{
					if(Mesh[SIZE][j-1]->isEmpty() == TRUE)
					{
						delete Mesh[SIZE][j-1];
						Mesh[SIZE][j-1] = aux;
					}
					else if(Mesh[SIZE][j+1]->isEmpty() == TRUE)
					{
						delete Mesh[SIZE][j+1];
						Mesh[SIZE][j+1] = aux;
					}
					else delete aux;
				}
				else
				{
					if(Mesh[SIZE][j+1]->isEmpty() == TRUE)
					{
						delete Mesh[SIZE][j+1];
						Mesh[SIZE][j+1] = aux;
					}
					else if(Mesh[SIZE][j-1]->isEmpty() == TRUE)
					{
						delete Mesh[SIZE][j-1];
						Mesh[SIZE][j-1] = aux;
					}
					else delete aux;
				}
				
			}
			else if(j == 1)
			{
				if(Mesh[SIZE][j+1]->isEmpty() == TRUE)
				{
					delete Mesh[SIZE][j+1];
					Mesh[SIZE][j+1] = aux;
				}
				else delete aux;
			}
			else if(j == SIZE)
			{
				if(Mesh[SIZE][j-1]->isEmpty() == TRUE)
				{
					delete Mesh[SIZE][j-1];
					Mesh[SIZE][j-1] = aux;
				}
				else delete aux;
			}
		}
	}
	
	/*
	Left boundary
	*/
	for(i = 1; i < SIZE+1; i++)
	{
		if(Mesh[i][0]->isEmpty() == FALSE)
		{
			aux = Mesh[i][0];
			Mesh[i][0] = new GridCell();
			
			if(Mesh[i][1]->isEmpty() == TRUE)
			{
				delete Mesh[i][1];
				Mesh[i][1] = aux; 
			}
			else if(i > 1 && i < SIZE)
			{
				odds = mtwister.randExc();

				if(odds < 0.5)
				{
					if(Mesh[i-1][1]->isEmpty() == TRUE)
					{
						delete Mesh[i-1][1];
						Mesh[i-1][1] = aux;
					}
					else if(Mesh[i+1][1]->isEmpty() == TRUE)
					{
						delete Mesh[i+1][1];
						Mesh[i+1][1] = aux;
					}
					else delete aux;
				}
				else
				{
					if(Mesh[i+1][1]->isEmpty() == TRUE)
					{
						delete Mesh[i+1][1];
						Mesh[i+1][1] = aux;
					}
					else if(Mesh[i-1][1]->isEmpty() == TRUE)
					{
						delete Mesh[i-1][1];
						Mesh[i-1][1] = aux;
					}
					else delete aux;
				}
				
			}
			else if(i == 1)
			{
				if(Mesh[i-1][1]->isEmpty() == TRUE)
				{
					delete Mesh[i-1][1];
					Mesh[i-1][1] = aux;
				}
				else delete aux;
			}
			else if(i == SIZE)
			{
				if(Mesh[i+1][1]->isEmpty() == TRUE)
				{
					delete Mesh[i+1][1];
					Mesh[i+1][1] = aux;
				}
				else delete aux;
			}
		}
	}
	
	/*
	Right boundary
	*/
	for(i = 1; i < SIZE+1; i++)
	{
		if(Mesh[i][SIZE+1]->isEmpty() == FALSE)
		{
			aux = Mesh[i][SIZE+1];
			Mesh[i][SIZE+1] = new GridCell();
			
			if(Mesh[i][SIZE]->isEmpty() == TRUE)
			{
				delete Mesh[i][SIZE];
				Mesh[i][SIZE] = aux; 
			}
			else if(i > 1 && i < SIZE)
			{
				odds = mtwister.randExc();

				if(odds < 0.5)
				{
					if(Mesh[i-1][SIZE]->isEmpty() == TRUE)
					{
						delete Mesh[i-1][SIZE];
						Mesh[i-1][SIZE] = aux;	
					}
					else if(Mesh[i+1][SIZE]->isEmpty() == TRUE)
					{
						delete Mesh[i+1][SIZE];
						Mesh[i+1][SIZE] = aux;
					}
					else delete aux;
				}
				else
				{
					if(Mesh[i+1][SIZE]->isEmpty() == TRUE)
					{
						delete Mesh[i+1][SIZE];
						Mesh[i+1][SIZE] = aux;
					}
					else if(Mesh[i-1][SIZE]->isEmpty() == TRUE)
					{
						delete Mesh[i-1][SIZE];
						Mesh[i-1][SIZE] = aux;	
					}
					else delete aux;
				}
				
			}
			else if(i == 1)
			{
				if(Mesh[i+1][SIZE]->isEmpty() == TRUE)
				{
					delete Mesh[i+1][SIZE];
					Mesh[i+1][SIZE] = aux;
				}
				else delete aux;
			}
			else if(i == SIZE)
			{
				if(Mesh[i-1][SIZE]->isEmpty() == TRUE)
				{
					delete Mesh[i-1][SIZE];
					Mesh[i-1][SIZE] = aux;	
				}
				else delete aux;
			}
		}
	}
	return;
}