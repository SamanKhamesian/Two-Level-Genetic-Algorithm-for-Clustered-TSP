# Two-Level-Genetic-Algorithm-for-Clustered-TSP

### Abstract

Let `G = (V, E)` be a complete undirected graph with vertex set `V`, edge set `E`, and edge weights `l(e)` satisfying the triangle inequality. The vertex set `V` is partitioned into clusters `V1, V2, ..., Vk`. The clustered traveling salesman problem (CTSP) seeks to compute the shortest Hamiltonian tour that visits all the vertices, in which the vertices of each cluster are visited consecutively. A two-level genetic algorithm (TLGA) was developed for the problem, which favors neither intra-cluster paths nor inter-cluster paths, thus realized integrated evolutionary optimization for both levels of the CTSP. Results show that the algorithm is more effective than known algorithms. A large-scale traveling salesman problem (TSP) can be converted into a CTSP
by clustering so that it can then be solved by the algorithm. Test results demonstrate that the clustering TLGA for large TSPs is more effective and efficient than the classical genetic algorithm.
This project is an implementation of TLGA. The main idea and theoretical approaches with their results are published in [this paper]().

##

#### To use this work on your researches or projects you need:
* GNU (c++14 or higher)

##

#### To run this project:
_First, check if you already have g++ installed or not_.
~~~~
g++ --version
~~~~
_Then, type this command to compile the source files_:
~~~~
g++ Main.cpp HighLevelGenetic.cpp LowLevelGenetic.cpp
~~~~
##
