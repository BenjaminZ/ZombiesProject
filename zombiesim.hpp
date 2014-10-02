/*
File: zombiesim.hpp
*/

#ifndef __ZOMBIESIM_HPP_INCLUDED__
#define __ZOMBIESIM_HPP_INCLUDED__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <omp.h>

using namespace std;

/*Output files. Don't forget "/" at the end.*/
#define BITMAP_STEP 50
#define BITMAP_PATH "/Users/Benjamin/Documents/myworkspace/ParallelProject1/bitmaps/"
#define OUTPUT_PATH "/Users/Benjamin/Documents/myworkspace/ParallelProject1/"

/*Northern Territory Statistics 2012*/
#define NT_POP 235182.0
#define NT_BIRTHS_PER_DAY 11.20
#define NT_DEATHS_PER_DAY 2.84
/*0-14*/
#define NT_YOUNG 22.6
#define NT_YOUNG_FINAL_AGE 14
/*15-54*/
#define NT_ADULT 61.8
#define NT_ADULT_FINAL_AGE 54
/*55+*/
#define NT_ELDER 15.6
#define NT_MALE_PERCENTAGE 52.5
#define NT_POP_DENSITY 0.171
/*Death fraction per age group*/
#define NT_YOUNG_DEATH 0.0102
#define NT_ADULT_DEATH 0.0883
#define NT_ELDER_DEATH 0.9014

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
#define YOUNG_MOVE 0.1
#define ADULT_MOVE 0.15
#define ELDER_MOVE 0.05

/*Zombies parameters*/
#define INFECTION_PROB 0.2
#define KILL_ZOMBIE 0.05
#define NUM_OF_ZOMBIES 2.0
#define ZOMBIE_LIFESPAN 4
#define ZOMBIE_MOVE 0.2

#endif // __ZOMBIESIM_HPP_INCLUDED__