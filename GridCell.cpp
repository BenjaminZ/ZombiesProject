/*
File: GridCell.cpp
*/

#include "GridCell.hpp"

GridCell::GridCell()
{
	z = NULL;
	h = NULL;
}
GridCell::~GridCell()
{
	if(z != NULL) delete z;
	if(h != NULL) delete h;
	return;
}
void GridCell::setToHuman(Human *aux)
{
	if(z != NULL)
	{
		delete z;
		z = NULL;
	}
	h = aux;
	return;
}
void GridCell::setToZombie(Zombie *aux)
{
	if(h != NULL)
	{
		delete h;
		h = NULL;
	}
	z = aux;
	return;
}

int GridCell::isEmpty()
{
	if(h != NULL || z != NULL) return FALSE;
	else return TRUE;
}

int GridCell::isHuman()
{
	if(h != NULL) return TRUE;
	else return FALSE;
}

int GridCell::isZombie()
{
	if(z != NULL) return TRUE;
	else return FALSE;
}
Human* GridCell::getHuman()
{
	return h;
}
Zombie* GridCell::getZombie()
{
	return z;
}
int GridCell::getStatus()
{
	if(h != NULL)
	{
		return h->getGender();
	}
	else if(z != NULL)
	{
		return ZOMBIE;
	}
	else return EMPTY;
}