#include "Config.h"
#include "LowLevelGenetic.h"

LowLevelGenetic::LowLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector<int> cluster)
{
	this->cluster = cluster;

	for (int i = 0; i < MAX_SIZE_GRAPH; i++)
	{
		for (int j = 0; j < MAX_SIZE_GRAPH; j++)
		{
			this->dist[i][j] = dist[i][j];
		}
	}

	this->population = makePopulation(cluster);
}

LowLevelGenetic::LowLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector<int> cluster, vector<LL_Individual> population)
{
	this->cluster = cluster;

	for (int i = 0; i < MAX_SIZE_GRAPH; i++)
	{
		for (int j = 0; j < MAX_SIZE_GRAPH; j++)
		{
			this->dist[i][j] = dist[i][j];
		}
	}

	this->population = population;
}

int LowLevelGenetic::calFitness(vector<int> chromosome)
{
	int sum = 0;

	for (int i = 0; i < (int)chromosome.size(); i++)
	{
		sum += dist[chromosome[i]][chromosome[(i + 1) % (int)chromosome.size()]];
	}

	return sum;
}

vector<LL_Individual> LowLevelGenetic::makePopulation(vector<int> cluster)
{
	vector<LL_Individual> init;

	for (int i = 0; (int)init.size() < (int)cluster.size() * (int)cluster.size(); i++)
	{
		random_shuffle(cluster.begin(), cluster.end());

		LL_Individual individual = { calFitness(cluster), cluster };

		if (find(init.begin(), init.end(), individual) == init.end())
		{
			init.push_back(individual);
		}
	}

	return init;
}

pair<LL_Individual, LL_Individual> LowLevelGenetic::selection()
{
	srand((unsigned)time(0));

	vector<LL_Individual> candidates;

	sort(population.begin(), population.end());

	for (int i = 0; i < (int)sqrt(population.size()); i++)
	{
		int index = rand() % population.size();

		candidates.push_back(population[index]);
	}

	sort(candidates.begin(), candidates.end());

	return make_pair(candidates[0], candidates[1]);
}

pair<LL_Individual, LL_Individual> LowLevelGenetic::crossover(LL_Individual parent_1, LL_Individual parent_2)
{
	srand((unsigned)time(0));

	int mutation_fraction = rand() % 100;

	if (mutation_fraction > LL_GA_CF * 100)
	{
		return make_pair(parent_1, parent_2);
	}

	else
	{
		int rand_1, rand_2, diff;

		do {

			rand_1 = rand() % parent_1.chromosome.size();
			rand_2 = rand() % parent_2.chromosome.size();
			diff = abs(rand_2 - rand_1);

		} while (diff <= 1 || diff >= (int)parent_1.chromosome.size() - 1);

		int cut_point_1 = min(rand_1, rand_2);
		int cut_point_2 = max(rand_1, rand_2);

		vector<int> child_1(parent_1.chromosome.size(), -1);
		vector<int> child_2(parent_2.chromosome.size(), -1);

		queue<int> remain_1;
		queue<int> remain_2;

		for (int i = cut_point_1; i <= cut_point_2; i++)
		{
			child_1[i] = parent_1.chromosome[i];
			child_2[i] = parent_2.chromosome[i];
		}

		for (int i = (cut_point_2 + 1) % (int)child_1.size(), j = 0; j < (int)child_1.size(); i = (i + 1) % (int)child_1.size(), j++)
		{
			if (find(child_1.begin(), child_1.end(), parent_2.chromosome[i]) == child_1.end())
			{
				remain_1.push(parent_2.chromosome[i]);
			}

			if (find(child_2.begin(), child_2.end(), parent_1.chromosome[i]) == child_2.end())
			{
				remain_2.push(parent_1.chromosome[i]);
			}
		}

		for (int i = (cut_point_2 + 1) % child_1.size(); !remain_1.empty(); i = (i + 1) % child_1.size())
		{
			child_1[i] = remain_1.front();
			child_2[i] = remain_2.front();

			remain_1.pop();
			remain_2.pop();
		}

		LL_Individual new_child_1 = { calFitness(child_1), child_1 };
		LL_Individual new_child_2 = { calFitness(child_2), child_2 };

		return make_pair(new_child_1, new_child_2);
	}
}

LL_Individual LowLevelGenetic::mutation(LL_Individual individual)
{
	srand((unsigned)time(0));
	vector<int> chromosome = individual.chromosome;

	int mutationFraction = rand() % 100;
	
	if (mutationFraction > LL_GA_MF * 100)
	{
		return individual;
	}

	else
	{
		int index_1 = rand() % chromosome.size();
		int index_2 = rand() % chromosome.size();

		int startPoint = min(index_1, index_2);
		int endPoint = max(index_1, index_2);

		reverse(chromosome.begin() + startPoint, chromosome.begin() + endPoint);

		LL_Individual answer = { calFitness(chromosome), chromosome };

		return answer;
	}
}
