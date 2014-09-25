/*
File: Zombie.cpp
*/

#include "Zombie.hpp"

Zombie::Zombie(int aux)
{
	turningdate = aux;
}

int Zombie::getTurningDate()
{
	return turningdate;
}