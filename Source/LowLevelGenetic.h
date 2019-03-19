#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <iostream>

using namespace std;

struct LL_Individual
{
	int fitness;
	vector<int> chromosome;

	inline bool operator< (const LL_Individual& ind) const
	{
		return (fitness < ind.fitness);
	}

	inline bool operator== (const LL_Individual& ind) const
	{
		return (fitness == ind.fitness && chromosome == ind.chromosome);
	}
};

class LowLevelGenetic
{
	vector<int> cluster;
	int dist[MAX_SIZE_GRAPH][MAX_SIZE_GRAPH];

public:

	vector<LL_Individual> population;

	LowLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector<int> cluster);
	LowLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector<int> cluster, vector<LL_Individual> population);

	int calFitness(vector<int> chromosoome);

	vector<LL_Individual> makePopulation(vector<int> cluster);

	pair< LL_Individual, LL_Individual > selection();

	pair< LL_Individual, LL_Individual > crossover(LL_Individual parent_1, LL_Individual parent_2);

	LL_Individual mutation(LL_Individual individual);
};