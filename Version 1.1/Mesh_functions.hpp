/*
File: Mesh_functions.hpp
*/

#ifndef __MESH_FUNCTIONS_HPP_INCLUDED__
#define __MESH_FUNCTIONS_HPP_INCLUDED__

#include "zombiesim_parameters.hpp"
#include "Zombie.hpp"
#include "Human.hpp"
#include "GridCell.hpp"

int getPopulation(GridCell***);

double getPairingNumber(GridCell***);

double getBirthRate(GridCell***);

double getDeathRate(GridCell***);

void printPopulation(GridCell***, int);

void proccessBoundaries(GridCell***);

void initializeMesh(GridCell***, GridCell***);

int fillMesh(GridCell***);

void printToBitmap(GridCell***, char*, int, int);

#endif // __MESH_FUNCTIONS_HPP_INCLUDED__