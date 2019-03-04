/*
	Mikel Mayorga
	013290716
	completion time: 25 February 2019, 23:21
*/


#include "CampusGraph.h";
#include "CsvParser.h"
#include <iostream>

// function to add vertices
void buildGraph(CampusGraph& graph, vector<vector<string>> dat)
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
	for (auto i : vertices)
	{
		string vertex = i.first;
		transform(vertex.begin(), vertex.end(), vertex.begin(), tolower);
		graph.addVertex(vertex);
	}
	for (auto i : dat)
	{
		string connection1 = i[0];
		transform(connection1.begin(), connection1.end(), connection1.begin(), tolower);
		string connection2 = i[1];
		transform(connection2.begin(), connection2.end(), connection2.begin(), tolower);
		graph.connectVertex(connection1, connection2, stod(i[2]));
	}

}
string locationToNodeName(string loc)
{
	unordered_map<string, string> names{};
	 
	names["canyon"] = "a";
	names["crekview"] = "b";
	names["jgc"] = "c";
	names["hill"] = "d";
	names["cypress"] = "e";
	names["fwh"] = "f";
	names["nhe"] = "g";
	names["nhw"] = "g";
	names["fh"] = "h";
	names["hah"] = "i";
	names["brh"] = "i";
	names["lapt"] = "i";
	names["uc"] = "j";
	names["uc quad"] = "j";
	names["bookstore"] = "j";
	names["hc"] = "k";
	names["library"] = "l";
	names["vmh"] = "m";
	names["sh"] = "n";
	names["rb"] = "o";
	names["musa"] = "p";
	names["upf"] = "q";
	names["musb"] = "r";
	names["artb"] = "r";
	names["hh"] = "s";
	names["balh"] = "s";
	names["rwc"] = "t";
	names["th"] = "u";
	names["arta"] = "v";
	names["wh"] = "w";
	names["bhh"] = "w";
	names["ceram"] = "x";
	names["sclpt"] = "x";
	names["gh"] = "y";
	names["scia"] = "z";
	names["src"] = "a2";
	names["ka"] = "b2";
	names["fgym"] = "c2";
	names["scib"] = "d2";
	names["scic"] = "d2";
	names["sbs"] = "e2";
	names["delno"] = "f2";
	names["collegecreek"] = "f2";
	names["shast"] = "f2";
	names["cdl"] = "g2";
	names["hgh"] = "g2";
	names["mendo"] = "h2";
	names["trin"] = "h2";
	names["cef"] = "i2";
	names["wdfs"] = "j2";
	names["mwcc"] = "k2";
	names["wgp"] = "l2";
	names["nr"] = "m2";
	names["fm"] = "n2";
	names["boat"] = "o2";
	names["bss"] = "p2";
	names["mcom"] = "q2";
	names["tod"] = "q2";
	names["wwh"] = "q2";
	names["mwh"] = "q2";
	names["bh"] = "q2";
	names["fr"] = "r2";
	names["annex"] = "s2";
	names["serc"] = "t2";

	return names[loc];
}
string nodeNameConversion(string node_name)
{
	unordered_map<string, string> names{};

	names["a"] = "Canyon Complex";
	names["b"] = "Creekview Complex";
	names["c"] = "Jolly Green Giant Commons";
	names["d"] = "The Hill";
	names["e"] = "Cypress Hall";
	names["f"] = "Feuerwerker House";
	names["g"] = "Nelson Hall";
	names["h"] = "Founder's Hall";
	names["i"] = "Little Apartments";
	names["j"] = "UC Quad";
	names["k"] = "Student Health Services";
	names["l"] = "Library";
	names["m"] = "Van Matre Hall";
	names["n"] = "Siemens Hall";
	names["o"] = "Redwood Bowl";
	names["p"] = "Music A";
	names["q"] = "Upper Playing Field";
	names["r"] = "Art Quad";
	names["s"] = "Multicultural Center";
	names["t"] = "Recreation and Wellness Center";
	names["u"] = "Telonicher House";
	names["v"] = "Art A";
	names["w"] = "Warren House/Bret Harte House";
	names["x"] = "Sculpture & Ceramics Lab";
	names["z"] = "Science A";
	names["a2"] = "Student Rcreational Center";
	names["b2"] = "Kinesiology and Atletics/LumberJack Arena";
	names["c2"] = "Forbes Gym";
	names["d2"] = "Science B/Science C";
	names["e2"] = "Student & Business Services";
	names["f2"] = "College Creek Complex North";
	names["g2"] = "Child Development Lab/Harry Griffith Hall";
	names["h2"] = "College Creek Complex East";
	names["i2"] = "Campus Events Field";
	names["j2"] = "Wildlife & Fisheries";
	names["k2"] = "Marine Wildlife Care Center";
	names["l2"] = "Wildlife Game Pens";
	names["m2"] = "Natural Resources";
	names["n2"] = "Facilites Management";
	names["o2"] = "Boat Facility";
	names["p2"] = "Behavioral & Social Sciences";
	names["q2"] = "Toddler Center/Marketing & Communications";
	names["r2"] = "Forestry";
	names["s2"] = "Trinity Complex";
	names["t2"] = "Scatz Energy Research Center";

	return names[node_name];

}

void createDirections(map<string, double> the_map, string start_loc, string end_loc)
{
	cout << "On your way from " << nodeNameConversion(locationToNodeName(start_loc)) <<
		"to" << nodeNameConversion(locationToNodeName(end_loc)) << ", you will pass by:"
		<< endl;
	for (auto i : the_map)
	{
		if (i.second == 0.0)
		{
			continue;
		}
		if (i.first == locationToNodeName(end_loc))
		{
			break;
		}
		cout << nodeNameConversion(i.first) << endl;

	}
}



int main(void)
{
	//Example of how to parse a CSV file for graph building
	CsvStateMachine csm{ "distances.csv" };
	vector<vector<string>> data = csm.processFile();

	// build graph
	CampusGraph graph{};
	buildGraph(graph, data);

	// get starting vertex
	string start = "";
	string end = "";

	cout << "Enter starting point: ";
	getline(cin, start);
	transform(start.begin(), start.end(),start.begin(), tolower);
	cout << "Enter end point: ";
	getline(cin, end);
	transform(end.begin(), end.end(),end.begin(), tolower);

	// vector<pair<string, double>> distances = getShortestDistance(graph, start, end);
	map<string, double> distances = graph.computeShortestPath(locationToNodeName(start),
															  locationToNodeName(end));

	cout << "Total distance: " << distances[locationToNodeName(end)] / 60 << "minutes" 
		<< endl;

	createDirections(distances, start, end);

	

	return 0;
}

