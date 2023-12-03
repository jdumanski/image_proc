#pragma once
#include <vector>
#include <map>

template <typename T>
class Graph {
private:
	std::map<std::pair<int, int>, double> weights; //weight of edge from n1 to n2 - decouple weights from adjacencyList 
	std::map<int, std::vector<int>> adjacencyList; //pair is node, weight
	std::map<int, T> data; //cant use vector because we allow removal of edges
	int numNodes = 0; //keeps track of total number of nodes inserted to ensure all have unique IDs
	bool directed = false;
	bool weighted = false;
public:
	Graph() = delete;
	Graph(int nodes, bool directed = false, bool weighted = false) : directed(directed), weighted(weighted), numNodes(nodes) {}
	//inserts 1 or 2 edges based on if directed or not
	//for directed, inserts edges from n1 to n2
	//assumes n1 and n2 in graph - if not that on YOU, don't be dumb
	bool InsertEdge(int n1, int n2, double weight=1) {
		adjacencyList[n1].push_back(n2);
		if (weighted) {
			weights[std::make_pair(n1, n2)] = weight;
		}
		if (!directed) {
			adjacencyList[n2].push_back(n1);
			if (weighted) {
				weights[std::make_pair(n2, n1)] = weight;
			}
		}
	}
	bool InsertNode(T nodeData) {
		data[numNodes++] = nodeData; 
		//dont need to do anything with adjacencyList bc all vectors in list are initialized to empty list
	}
	//VERY expensive so try not to use
	//why r u removing nodes anyway, just dont put them in in the first place
	bool RemoveNode(int node) {
		data.erase(node);
		//remove all edge weights from node to delete to its neighbours
		for (int neighbour : adjacencyList[node]) {
			weights.erase(std::make_pair(node, neighbour));
		}
		adjacencyList.erase(node);
		//remove all edge weights from nodes to node to delete
		//and remove node to delete from all other adjacency lists
		for (auto &mapPair : adjacencyList) {
			for (auto currNode : mapPair.second) {
				if (currNode == node) {
					weights.erase(std::make_pair(mapPair.first, node));
					mapPair.second.erase(node);
				}
			}
		}
		return true;
	}
	bool RemoveEdge(int n1, int n2) {
		weights.erase(std::make_pair(n1, n2));
		adjacencyList[n1].erase(std::remove(adjacencyList[n1].begin(), adjacencyList[n1].end(), n2), adjacencyList[n1].end());
		if (!directed) {
			weights.erase(std::make_pair(n2, n1));
			adjacencyList[n2].erase(std::remove(adjacencyList[n2].begin(), adjacencyList[n2].end(), n1), adjacencyList[n2].end());
		}
		return true;
	}
	std::vector<int> getNeighbours(int node) const {
		return adjacencyList[node];
	}
	
	//implement A* later if you ever need this
	std::vector<int> ShortestPath();

	int getWeight(int n1, int n2) const {
		if (!weighted) 
			return 1;
		return weights[std::make_pair(n1, n2)];
	}

	bool EdgeExists(int n1, int n2) const {
		if (std::find(adjacencyList[n1].begin(), adjacencyList[n1].end(), n2) != adjacencyList[n1].end())
			return true;
		return false;
	}

	T getData(int node) const {
		return data[node];
	}

	T getData(int node) {
		return data[node];
	}
};