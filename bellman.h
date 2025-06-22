#ifndef BELLMAN_H
#define BELLMAN_H
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
#define MAX 1000

void BF(int edgeList[][3], int numEdges, char start, int BFValue[], int BFPrev[]);


string BF_Path(int edges[][3], int numEdges, char start, char goal);

#endif