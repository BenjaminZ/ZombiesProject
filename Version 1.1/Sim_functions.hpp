/*
File: Sim_functions.hpp
*/

#ifndef __SIM_FUNCTIONS_HPP_INCLUDED__
#define __SIM_FUNCTIONS_HPP_INCLUDED__

#include "zombiesim_parameters.hpp"
#include "Zombie.hpp"
#include "Human.hpp"
#include "GridCell.hpp"

int executeBirthControl(GridCell***, int, int, double);

void executeMovement(GridCell***, GridCell***, int, int);

void executeInfection(GridCell***, int, int, int);

void executeDeathControl(GridCell***, int, int, double, int);

#endif // __SIM_FUNCTIONS_HPP_INCLUDED__