/*
File: zombiesim_main.hpp
*/

#ifndef __ZOMBIESIM_PARAMETERS_HPP_INCLUDED__
#define __ZOMBIESIM_PARAMETERS_HPP_INCLUDED__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;
/*Northern Territory Statistics 2012*/
#define NT_POP 235182
#define NT_BIRTHS_PER_DAY 11.20
#define NT_DEATHS_PER_DAY 2.84
#define NT_YOUNG 22.6
#define NT_ADULT 61.8
#define NT_ELDER 15.6
#define NT_MALE_PERCENTAGE 52.5
#define NT_POP_DENSITY 0.171

/*Simulation parameters*/
#define YEARS	10
#define STEPS	365
#define SIZE	500
#define MOVE	0.1
#define TRUE	1
#define FALSE	0

/*Types of grid cells*/
#define HUMAN 	0
#define MALE	1
#define FEMALE	2
#define ZOMBIE 	3
#define EMPTY	4

/*Human parameters*/
#define HEALTHY 0
#define EXPOSED 1

/*Human age modifiers*/
#define YOUNG	0
#define ADULT	1
#define ELDER	2

/*Zombies parameters*/
#define INFECTION_PROB 0.8
#define KILL_ZOMBIE 0.05

#endif // __ZOMBIESIM_PARAMETERS_HPP_INCLUDED__