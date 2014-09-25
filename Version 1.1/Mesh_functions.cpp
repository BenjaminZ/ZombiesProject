/*
File: Mesh_functions.cpp
*/

#include "Mesh_functions.hpp"

int getPopulation(GridCell*** Mesh)
{
	int populationTotal = 0;
	for (int i = 1; i <= SIZE; i++) 
		for (int j = 1; j <= SIZE; j++)
			if (Mesh[i][j]->isEmpty() == FALSE) populationTotal++;
	return populationTotal;
}

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

int fillMesh(GridCell*** Mesh)
{
	int i, j, num_zombies;
	double aux_rand;
	int gender, age, agemodifier, birthdate;

	for (num_zombies = 0, i = 1; i <= SIZE; i++)
	{ 
		for (j = 1; j <= SIZE; j++)
		{
			aux_rand = drand48();
			
			if (aux_rand < NT_POP_DENSITY)
			{	
				/*
				Define gender.
				*/			
				if(drand48() < (NT_MALE_PERCENTAGE/100)) gender = MALE;
				else gender = FEMALE;

				/*
				Define age group.
				*/
				aux_rand = drand48();
				if(aux_rand < (NT_YOUNG/100)) agemodifier = YOUNG;
				else if(aux_rand < (NT_ADULT/100)) agemodifier = ADULT;
				else agemodifier = ELDER;

				/*
				Define age (in steps)
				*/

				/*
				Creates human.
				*/
				Mesh[i][j]->setToHuman(new Human(gender, 0, agemodifier, HEALTHY));
			}
			else if(aux_rand >= 0.999992)
			{
				Mesh[i][j]->setToZombie(new Zombie(0));
				num_zombies += 1;
			}
		}
	}
	return num_zombies;
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