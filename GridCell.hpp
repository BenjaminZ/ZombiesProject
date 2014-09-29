/*
File: GridCell.hpp
*/

#ifndef __GRIDCELL_HPP_INCLUDED__
#define __GRIDCELL_HPP_INCLUDED__

#include <stdio.h>
#include "Zombie.hpp"
#include "Human.hpp"
#include "zombiesim.hpp"

class GridCell
{
	private:
		Zombie* z;
		Human* h;

	public:
		GridCell();
		void setToHuman(Human*);
		void setToZombie(Zombie*);
		int isEmpty();
		int isHuman();
		int isZombie();
		Human* getHuman();
		Zombie* getZombie();
		int getStatus();
};

#endif // __GRIDCELL_HPP_INCLUDED__