#include "Config.h"
#include "LowLevelGenetic.h"
#include "HighLevelGenetic.h"

#include <fstream>
#include <cstring>
#include <iomanip>

using namespace std;

ifstream fin(INPUT_FILE_PATH);
ofstream fout(OUTPUT_FILE_PATH);

int n, m, k;
int dp[LIMIT][1 << LIMIT];
int parent[LIMIT][1 << LIMIT];
int clusterNumber[MAX_SIZE_GRAPH];
int dist[MAX_SIZE_GRAPH][MAX_SIZE_GRAPH];

void initialize()
{
	fin >> n >> m;
	k = int(sqrt(n));

	memset(dp, -1, sizeof dp);
	memset(dist, 0, sizeof dist);
	memset(parent, -1, sizeof parent);
	memset(clusterNumber, -1, sizeof clusterNumber);

	for (int i = 0; i < m; i++)
	{
		int u, v, w;
		fin >> u >> v >> w;

		dist[u][v] = dist[v][u] = w;
	}
}

void printAnswer(int pathLength)
{
	int path[LIMIT];
	int curNode = 0;
	int curMask = 1;

	memset(path, -1, sizeof path);

	for (int i = 0; curNode != -1; i++)
	{
		path[i] = curNode;
		curNode = parent[curNode][curMask];
		curMask = curMask | (1 << curNode);
	}

	pathLength += dist[path[n - 1]][path[0]];

	fout << "The Shortest Path Has Length = " << pathLength << endl;

	fout << "The Path is = ";

	for (int i = 0; i < n; i++)
	{
		if (i != 0) fout << " --> ";

		fout << path[i];
	}

	fout << endl;
}

void printAnswer(HL_Individual individual)
{
	int cost = individual.fitness;
	vector<int> path = individual.chromosomoe;

	fout << "The Shortest Path Has Lenght = " << cost << endl;

	fout << "The Path is = ";

	for (int i = 0; i < (int)path.size(); i++)
	{
		if (i != 0) fout << " --> ";

		fout << path[i];
	}

	fout << endl;
}

vector< vector<int> > clusterGraph()
{
	vector< vector<int> > clusters;

	for (int i = 0; i < k * k; i += k)
	{
		vector<int> cluster;
		vector< pair<int, int> > candidates;
		int clusterSize = (n / k) + ((i == 0) ? n%k : 0);

		cluster.push_back(i);
		clusterNumber[i] = (i / k);

		for (int j = 0; j < n; j++)
		{
			if (i == j || ((j % k) == 0 && j < k * k)) continue;

			candidates.push_back(make_pair(dist[i][j], j));
		}

		sort(candidates.begin(), candidates.end());

		for (int j = 0; (j < (int)candidates.size()) && ((int)cluster.size() < clusterSize); j++)
		{
			int index = candidates[j].second;

			if (clusterNumber[index] == -1)
			{
				cluster.push_back(index);
				clusterNumber[index] = (i / k);
			}
		}

		clusters.push_back(cluster);
	}

	return clusters;
}

int runClassicTSPAlgorithm(int pos, int bitmask)
{
	if (bitmask == (1 << (n + 1)) - 1)
	{
		return dist[pos][0];
	}

	else if (dp[pos][bitmask] != -1)
	{
		return dp[pos][bitmask];
	}

	else
	{
		int ans = 2000000000;

		for (int nxt = 0; nxt < n + 1; nxt++)
		{
			if (nxt != pos && !(bitmask & (1 << nxt)))
			{
				int curValue = dist[pos][nxt] + runClassicTSPAlgorithm(nxt, bitmask | (1 << nxt));

				if (ans > curValue)
				{
					ans = curValue;
					parent[pos][bitmask] = nxt;
				}
			}
		}

		return dp[pos][bitmask] = ans;
	}
}

vector<int> runLowLevelGeneticAlgorithm(vector<int> cluster)
{
	LowLevelGenetic genetic(dist, cluster);
	int populationSize = genetic.population.size();

	for (int i = 0; i < LL_GA_GENERATIONS; i++)
	{
		pair<LL_Individual, LL_Individual> parents = genetic.selection();

		pair<LL_Individual, LL_Individual> newParents = genetic.crossover(parents.first, parents.second);

		int fraction = rand() % 2;

		if (!fraction)
			newParents.first = genetic.mutation(newParents.first);

		else
			newParents.second = genetic.mutation(newParents.second);

		sort(genetic.population.begin(), genetic.population.end());

		genetic.population[populationSize - 1] = newParents.first;
		genetic.population[populationSize - 2] = newParents.second;
	}

	sort(genetic.population.begin(), genetic.population.end());

	return genetic.population[0].chromosome;
}

HL_Individual runHighLevelGeneticAlgorithm(vector< vector<int> > clusters)
{
	HighLevelGenetic genetic(dist, clusters);

	int populationSize = genetic.population.size();

	for (int i = 0; i < HL_GA_GENERATIONS; i++)
	{
		pair<HL_Individual, HL_Individual> parents = genetic.selection();

		pair<HL_Individual, HL_Individual> newParents = genetic.crossover(parents.first, parents.second);

		int fraction = rand() % 2;

		if (!fraction)
		{
			newParents.first = genetic.firstMutation(newParents.first);
			newParents.first = genetic.secondMutation(newParents.first);
		}

		else
		{
			newParents.second = genetic.firstMutation(newParents.second);
			newParents.second = genetic.secondMutation(newParents.second);
		}

		sort(genetic.population.begin(), genetic.population.end());

		genetic.population[populationSize - 1] = newParents.first;
		genetic.population[populationSize - 2] = newParents.second;
	}

	sort(genetic.population.begin(), genetic.population.end());

	return genetic.population[0];
}

void process()
{
	initialize();

	vector< vector<int> > subAnswer;

	if (n < LIMIT)
	{
		int answer = runClassicTSPAlgorithm(0, 1);

		printAnswer(answer);
	}

	else
	{
		vector< vector<int> > clusters = clusterGraph();

		for (int i = 0; i < (int)clusters.size(); i++)
		{
			subAnswer.push_back(runLowLevelGeneticAlgorithm(clusters[i]));
		}

		HL_Individual answer = runHighLevelGeneticAlgorithm(subAnswer);

		printAnswer(answer);
	}
}

int main()
{
	process();
}