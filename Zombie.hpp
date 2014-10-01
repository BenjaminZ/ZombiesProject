/*
File: Zombie.hpp
*/
#ifndef __ZOMBIE_HPP_INCLUDED__
#define __ZOMBIE_HPP_INCLUDED__

#include "zombiesim.hpp"

class Zombie
{
	private:
		int turningdate;
	public:
		Zombie(int);

		int getTurningDate();
};

#endif // __ZOMBIE_HPP_INCLUDED__