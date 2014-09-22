#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

using namespace std;

#define STEPS	100
#define SIZE	500
#define MOVE	0.1
#define TRUE 1
#define FALSE 0

#define MALE 0
#define FEMALE 1
#define ZOMBIE 2
#define EMPTY 3