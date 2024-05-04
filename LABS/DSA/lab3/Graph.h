#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#define SIZE1 20
#define SIZE2 100
#define INF 1e9

typedef struct graph {
  int nvertices;
  int **m;     
  int *visited;
} graph;