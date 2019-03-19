#include "Config.h"
#include "HighLevelGenetic.h"

HighLevelGenetic::HighLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector< vector<int> > clusters)
{
	this->clusters = clusters;

	for (int i = 0; i < MAX_SIZE_GRAPH; i++)
	{
		for (int j = 0; j < MAX_SIZE_GRAPH; j++)
		{
			this->dist[i][j] = dist[i][j];
		}
	}

	this->population = makePopulation(clusters);
}

HighLevelGenetic::HighLevelGenetic(int dist[][MAX_SIZE_GRAPH], vector< vector<int> > clusters, vector<HL_Individual> population)
{
	this->clusters = clusters;

	for (int i = 0; i < MAX_SIZE_GRAPH; i++)
	{
		for (int j = 0; j < MAX_SIZE_GRAPH; j++)
		{
			this->dist[i][j] = dist[i][j];
		}
	}

	this->population = population;
}

vector<HL_Individual> HighLevelGenetic::makePopulation(vector< vector<int> > clusters)
{
	vector<HL_Individual> init;

	for (int i = 0; (int)init.size() < HL_GA_POPULATION_SIZE; i++)
	{
		vector<int> chromosome;

		random_shuffle(clusters.begin(), clusters.end());

		int index = i % (int)clusters.size();

		int rot = rand() % (int)clusters[index].size();

		rotate(clusters[index].begin(), clusters[index].begin() + rot, clusters[index].end());

		chromosome = makePath(clusters);

		HL_Individual individual = { calFitness(chromosome), chromosome, clusters };

		if (find(init.begin(), init.end(), individual) == init.end())
		{
			init.push_back(individual);
		}
	}

	return init;
}

int HighLevelGenetic::calFitness(vector<int> chromosome)
{
	int sum = 0;

	for (int i = 0; i < (int)chromosome.size(); i++)
	{
		sum += dist[chromosome[i]][chromosome[(i + 1) % (int)chromosome.size()]];
	}

	return sum;
}

pair< HL_Individual, HL_Individual > HighLevelGenetic::selection()
{
	srand((unsigned)time(0));

	vector<HL_Individual> candidates;

	sort(population.begin(), population.end());

	for (int i = 0; i < (int)sqrt(population.size()); i++)
	{
		int index = rand() % population.size();

		candidates.push_back(population[index]);
	}

	sort(candidates.begin(), candidates.end());

	return make_pair(candidates[0], candidates[1]);
}

pair< HL_Individual, HL_Individual > HighLevelGenetic::crossover(HL_Individual parent_1, HL_Individual parent_2)
{
	srand((unsigned)time(0));

	int mutationFraction = rand() % 100;

	if (mutationFraction > HL_GA_CF * 100)
	{
		return make_pair(parent_1, parent_2);
	}

	else
	{
		vector< vector<int> > child_1(parent_1.clusters.size());
		vector< vector<int> > child_2(parent_2.clusters.size());

		for (int i = 0; i < (int)parent_1.clusters.size(); i++)
		{
			int random = rand() % 2;

			if (rand() == 0)
			{
				child_1[i] = parent_1.clusters[i];
				child_2[i] = parent_2.clusters[i];
			}
			
			else
			{
				child_1[i] = parent_2.clusters[i];
				child_2[i] = parent_1.clusters[i];
			}
		}

		vector<int> tempChild_1 = makePath(child_1);
		vector<int> tempChild_2 = makePath(child_2);

		HL_Individual new_child_1 = { calFitness(tempChild_1), tempChild_1, child_1 };
		HL_Individual new_child_2 = { calFitness(tempChild_2), tempChild_2, child_2 };

		return make_pair(new_child_1, new_child_2);
	}
}

HL_Individual HighLevelGenetic::firstMutation(HL_Individual individual)
{
	srand((unsigned)time(0));
	vector< vector<int> > clusters = individual.clusters;

	int mutationFraction = rand() % 100;

	if (mutationFraction > HL_GA_MF_11 * 100)
	{
		return individual;
	}

	else
	{
		int index_1 = rand() % clusters.size();
		int index_2 = rand() % clusters.size();

		int startPoint = min(index_1, index_2);
		int endPoint = max(index_1, index_2);

		reverse(clusters.begin() + startPoint, clusters.begin() + endPoint);

		vector<int> temp_clusters = makePath(clusters);

		HL_Individual answer = { calFitness(temp_clusters), temp_clusters, clusters };

		return answer;
	}
}

HL_Individual HighLevelGenetic::secondMutation(HL_Individual individual)
{
	srand((unsigned)time(0));

	vector< vector<int> > clusters = individual.clusters;

	int mutationFraction = rand() % 100;

	if (mutationFraction > HL_GA_MF_21 * 100)
	{
		return individual;
	}

	else
	{
		int index = rand() % (int)clusters.size();
		
		mutationFraction = rand() % 100;

		if (mutationFraction > HL_GA_MF_22 * 100)
		{
			return individual;
		}

		else
		{
			int edge = rand() % (int)clusters[index].size();

			rotate(clusters[index].begin(), clusters[index].begin() + edge, clusters[index].end());

			vector<int> temp_clusters = makePath(clusters);

			HL_Individual answer = { calFitness(temp_clusters), temp_clusters, clusters };

			return answer;
		}

	}
}

vector<int> HighLevelGenetic::makePath(vector< vector<int> > clusters)
{
	vector<int> tempClusters = clusters[0];

	for (int i = 1; i < (int)clusters.size(); i++)
	{
		tempClusters.insert(tempClusters.end(), clusters[i].begin(), clusters[i].end());
	}

	return tempClusters;
}