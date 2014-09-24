#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;
/*Northern Territory Statistics*/
#define NT_POP 235182
#define NT_BIRTHS_PER_DAY 11.20
#define NT_DEATHS_PER_DAY 2.84

/*Constants used*/
#define YEARS	5
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


/*Human status*/
#define HEALTHY 0
#define EXPOSED 1

/*Zombies constants*/
#define INFECTION_PROB 0.8
#define KILL_ZOMBIE 0.05
