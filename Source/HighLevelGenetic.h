#include <vector>
#include <queue>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

struct HL_Individual
{
	int fitness;
	vector<int> chromosomoe;
	vector< vector<int> > clusters;

	inline bool operator< (const HL_Individual& ind) const
	{
		return (fitness < ind.fitness);
	}

	inline bool operator== (const HL_Individual& ind) const
	{
		return (fitness == ind.fitness && chromosomoe == ind.chromosomoe && clusters == ind.clusters);
	}
};

class HighLevelGenetic
{
	vector< vector<int> > clusters;
	int dist[MAX_SIZE_GRAPH][MAX_SIZE_GRAPH];

public:

	vector<HL_Individual> population;

	HighLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector< vector<int> > clusters);
	HighLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector< vector<int> > clusters, vector<HL_Individual> population);

	vector<HL_Individual> makePopulation(vector< vector<int> > clusters);

	int calFitness(vector<int> members);

	pair< HL_Individual, HL_Individual > selection();

	pair< HL_Individual, HL_Individual > crossover(HL_Individual parent_1, HL_Individual parent_2);

	HL_Individual firstMutation(HL_Individual individual);

	HL_Individual secondMutation(HL_Individual individual);

	vector<int> makePath(vector< vector<int> > clusters);
};