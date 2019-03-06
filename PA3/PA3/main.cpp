#include "CityGraph.h";
#include "CsvParser.h"
#include<iostream>	

void buildMST(CityGraph& graph, vector<vector<string>> dat)
{
	unordered_map<string, int> vertices;

	// organize unique vertices
	for (auto i : dat)
	{
		for (int j = 0; j < 2; j++)
		{
			vertices[i[j]]++;
		}
	}

	// add unique vertices
	for (auto i : vertices)
	{
		string vertex = i.first;
		transform(vertex.begin(), vertex.end(), vertex.begin(), tolower);
		graph.addVertex(vertex);
	}

	// add connections
	for (auto i : dat)
	{
		string connection1 = i[0];
		transform(connection1.begin(), connection1.end(), connection1.begin(), tolower);
		string connection2 = i[1];
		transform(connection2.begin(), connection2.end(), connection2.begin(), tolower);
		graph.connectVertex(connection1, connection2, stod(i[2]), true);
	}
}

vector<string> getDestinations(string filename)
{
	// return val
	vector<string> d{};

	ifstream file{ filename };
	
	if (file.is_open())
	{
		while (file.good())
		{
			string line;
			getline(file, line);
			transform(line.begin(), line.end(), line.begin(), tolower);
			d.push_back(line);
		}
	}
	
	return d;

}

int getTotalTime(vector<Edge> mst)
{
	int i = 0;
	for (auto edge : mst)
	{
		i += edge.weight;
	}
	return i;
}

int main(void)
{
	cout << "***Route Planner***" << endl;
	//Example of how to parse a CSV file for graph building
	CsvStateMachine csm{ "map1.txt" };
	vector<vector<string>> data = csm.processFile();

	CityGraph graph{};
	buildMST(graph, data);

	vector<string> deliveries = getDestinations("deliveries1.txt");

	// compute mst from starting vertex
	vector<Edge> mst = graph.computeMinimumSpanningTree(deliveries[0]);

	cout << "Total transit time: " << getTotalTime(mst) << " minutes" << endl;


	/*
	graph.addVertex("a");
	graph.addVertex("b");
	graph.addVertex("c");
	graph.addVertex("d");
	graph.addVertex("e");
	graph.addVertex("f");
	graph.addVertex("g");
	graph.addVertex("h");
	graph.connectVertex("a", "b", 3, true);
	graph.connectVertex("a", "c", 15, true);
	graph.connectVertex("a", "d", 6, true);
	graph.connectVertex("e", "a", 4, true);
	graph.connectVertex("h", "f", 4, true);
	graph.connectVertex("f", "e", 12, true);
	graph.connectVertex("h", "g", 3, true);
	graph.connectVertex("a", "g", 11, true);
	graph.connectVertex("b", "g", 6, true);
	graph.connectVertex("c", "f", 2, true);
	graph.connectVertex("d", "g", 6, true);
	graph.connectVertex("e", "d", 7, true);
	graph.connectVertex("f", "c", 4, true);
	graph.connectVertex("g", "b", 9, true);
	graph.connectVertex("h", "a", 7, true);
	auto distances = graph.computeShortestPath("a");
	auto mst = graph.computeMinimumSpanningTree("a");
	*/
	return 0;
}