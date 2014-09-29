/*
File: Sim_functions.hpp
*/

#ifndef __SIMULATION_EXECUTION_HPP_INCLUDED__
#define __SIMULATION_EXECUTION_HPP_INCLUDED__

#include "MersenneTwister.h"
#include "zombiesim.hpp"
#include "Zombie.hpp"
#include "Human.hpp"
#include "GridCell.hpp"

int executeBirthControl(GridCell***, int, int, double, MTRand*);

void executeMovement(GridCell***, GridCell***, int, int, MTRand*);

void executeInfection(GridCell***, int, int, int, MTRand*);

void executeDeathControl(GridCell***, int, int, double*, int, MTRand*);

#endif // __SIMULATION_EXECUTION_HPP_INCLUDED__