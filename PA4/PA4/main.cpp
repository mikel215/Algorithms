#include <iostream>
#include <sstream>
#include <fstream>
#include<string>
#include<queue>
#include <unordered_map>
#include <cmath>
#include <string>
#include "CsvParser.h"
#include "TreeNode.h"

using namespace std;

//calculate entropy based on a frequency distribution of all 
//outcome levels
double calculateEntropy(const unordered_map<string, int>& outcome_level)
{
   //determine denominator
   int denominator = 0;
   for (auto i : outcome_level)
   {
      denominator += i.second;
   }

   //calculate entropy
   double entropy = 0.0;
   for (auto item : outcome_level)
   {
      double ratio = (double)item.second / denominator;
      double logged = log2(ratio);
      entropy += -ratio * logged;
   }
   return entropy;
}

//builds frequency distribution based on supplied vector of data
unordered_map<string, int> buildFrequencyDistribution(const vector<string>& data)
{
   unordered_map<string, int> distribution{};
   for (auto item : data)
   {
      distribution[item]++;
   }
   return distribution;
}

//allows us to grab a specific column of data from our 2d matrix
vector<string> getObservations(const vector<vector<string>>& matrix, 
   int column)
{
   vector<string> result{};
   for (int i = 0; i < matrix.size(); i++)
   {
      result.push_back(matrix[i][column]);
   }
   return result;
}

//for the given matrix, reduce matrix such that all results
//match the supplied predictor variable on the supplied column
vector<vector<string>> reduceMatrix(
   const vector<vector<string>>& matrix,
   int column,
   string predictor
)
{
   vector<vector<string>> result{};
   for (int i = 0; i < matrix.size(); i++)
   {
      if (matrix[i][column] == predictor)
      {
         result.push_back(matrix[i]);
      }
   }
   return result;
}

int findMaxGain(
   const vector<vector<string>>& matrix, 
   int outcome_column,
   double entropy)
{
   if (matrix.size() == 0)
   {
      return -1;
   }

   vector<double> information_gain{};

   //caculate information gain for each predictor variable
   for (int column = 0; column < matrix[0].size(); column++)
   {
      //skip outcome column
      if (column == outcome_column)
      {
         information_gain.push_back(-1);
         continue;
      }

      vector<string> observations = getObservations(matrix, column);
      unordered_map<string, int> observation_levels = buildFrequencyDistribution(observations);
      double local_entropy = 0.0;
      for (auto level : observation_levels)
      {
         auto reduced_matrix = reduceMatrix(matrix, column, level.first);
         auto reduced_observations = getObservations(reduced_matrix, outcome_column);
         local_entropy += ((double)level.second / observations.size()) *
            calculateEntropy(buildFrequencyDistribution(reduced_observations));
         
      }
      information_gain.push_back(entropy - local_entropy);
      
   }

   //return column with most gain
   int most_gain = 0;
   for (int i = 1; i < information_gain.size(); i++)
   {
      if (information_gain[i] > information_gain[most_gain])
      {
         most_gain = i;
      }
   }
   return most_gain;
}

TreeNode* buildTree(
   const vector<vector<string>>& matrix,
   const vector<string>& predictors,
   const int& outcome_column
)
{
   vector<string> observations = getObservations(matrix, outcome_column);
   double entropy = calculateEntropy(
      buildFrequencyDistribution(observations));

   //base case: 0 entropy
   if (entropy < 0.01)
   {
      TreeNode* node = new TreeNode{};
      node->value = matrix[0][outcome_column];
      return node;
   }

   int col = findMaxGain(matrix, outcome_column, entropy);

   //create new node
   TreeNode* node = new TreeNode{};
   node->value = predictors[col];

   //attach as children all levels of outcome
   vector<string> selected_observations = getObservations(matrix, col);
   auto selected_levels = buildFrequencyDistribution(selected_observations);
   for (auto level : selected_levels)
   {
      auto reduced_matrix = reduceMatrix(matrix, col, level.first);
      node->children[level.first] = buildTree(reduced_matrix, predictors, outcome_column);
   }
   return node;
}

TreeNode* buildTreeFromFile(string filename, string outcome_variable)
{
   CsvStateMachine parser{ filename };
   vector<vector<string>> data = parser.processFile();
   vector<string> header = data[0];

   //AC NOTE: very slow.  Consider using STL move to move
   //array elements 1...size() to another structure
   data.erase(data.begin());

   TreeNode* root = buildTree(data, header, stoi(outcome_variable));
   return root;
	
}

int heightTree(TreeNode* root)
{
	if (root->children.empty() == true)
	{
		return 0;
	}

	int h = 0;
	for (auto i : root->children)
	{
		h = heightTree(i.second);
	}

	return 1 + h;
}

void writeTreeToFile(TreeNode* root)
{
	int height = heightTree(root);
	ofstream file{ "tree_file.txt" };
	queue<pair<string, TreeNode*>> nodes{};
	// base case
	file << "NULL" << "," <<root->value << "," << root->children.size() << endl;

	for (auto i : root->children)
	{
		nodes.push(make_pair(i.first, i.second));
	}

	while (nodes.empty() == false)
	{
		pair<string, TreeNode*> current = nodes.front();
		nodes.pop();
		
		// add children nodes to queue
		for (auto i : current.second->children)
		{
			nodes.push(make_pair(i.first, i.second));
		}

		file << current.first << "," << current.second->value << "," <<
			current.second->children.size() << endl;
	}

	file << height;


	file.close();


}

// [0] : edge value
// [1] : node value
// [2] : number of children
vector<string> parse(string in)
{
	stringstream ss(in);
	string i;
	vector<string> s{};
	while (getline(ss, i, ','))
	{
		s.push_back(i);
	}
	return s;
}

int getNumNodes(int height, int k)
{
	int top = (pow(k, (height + 1)) - 1);
	int bottom = k - 1;

	return top / bottom;
}

void balance(vector<string>& tree, vector<string> orig, queue<int> q)
{
	while (q.empty() == false)
	{
		int front = q.front();
		q.pop();

		tree[front] = "NULL";
	}
	
	queue<string> tmp{};
	for (auto i : orig)
	{
		tmp.push(i);
	}

	for (int i = 0; i < tree.size(); i++)
	{
		if (tree[i] == "NULL")
		{
			continue;
		}
		tree[i] = tmp.front();
		tmp.pop();
	}
	return;
}

vector<string> serializeNaryTree(vector<string> tree, int k, int height)
{
	int total_nodes = getNumNodes(height, k);
	vector<string> final_tree(total_nodes);
	queue<int> stuff{};
	for (int i = 0; i < tree.size(); i++)
	{
		int num_children = stoi(parse(tree[i])[2]);
		if (num_children == k)
		{
			continue;
		}
		if (num_children < k)
		{
			for (int j = k; j > num_children - 1 ; j--)
			{
				if (j == num_children) break;
				int new_index = k * i + j;
				stuff.push(new_index);

			}
			int end = stuff.size() + tree.size();
			if (end == total_nodes + 1)
			{
				break;
			}
		}

	}
	balance(final_tree, tree, stuff);

	return final_tree;
}

// recursive function that builds N-ary tree
void buildTreeHelper(TreeNode* node, vector<string> tree, int k_value, int index, int leaf_children)
{
	vector<string> s{};

	// return once we reach leaf node
	if (leaf_children == 0)
	{
		return;
	}

	// i = the nth child node
	unordered_map<string, TreeNode*> children_map{};
	for (int i = 1; i < leaf_children + 1; i++)
	{
		// get index of child node
		int child_index = (k_value * index) + i;

		// get data of child node from tree vector
		vector<string> data = parse(tree[child_index]);

		// make child node
		TreeNode* child = new TreeNode{};
		child->value = data[1];
		// recursively build this nodes child nodes
		buildTreeHelper(child, tree, k_value, child_index, stoi(data[2]));

		// insert node(after building its child nodes) into current nodes child_map
		children_map.insert(make_pair(data[0], child));
	}
	node->children = children_map;
	return;
}


TreeNode* readTreeFromFile()
{
	// read tree file
	ifstream file{ "tree_file.txt" };

	// read file into temp vector
	vector<string> tree_lines{};
	if (file.is_open() == true)
	{
		while (file.good() == true)
		{
			string line = "";
			getline(file, line);
			tree_lines.push_back(line);
		}
	}


	//last element is tree height
	int height = stoi(tree_lines[tree_lines.size() - 1]);

	vector<string> root_data = parse(tree_lines[0]);
	int root_children = stoi(root_data[2]);

	// turn temp vector into completed N-ary tree
	vector<string> tree = serializeNaryTree(tree_lines, root_children, height);



	//build tree from vector, start with root 
	TreeNode* root = new TreeNode{};
	root->value = root_data[1];
	buildTreeHelper(root, tree, root_children, 0, root_children);
	return root;
}


void mainAppPrompt(int& opt)
{
	cout << "Please choose from the following options:" << endl;
	cout << "1. Build decision tree from file" << endl;
	cout << "2. Write decision tree to file" << endl;
	cout << "3. Predict outcome" << endl;
	cout << "4. Read tree from file" << endl;

	string option;
	getline(cin, option);
	opt = stoi(option);

}

int main(void)
{
	// initialize all possible variables
	int decision = 3;
	TreeNode* root = nullptr;
	string outcome = "";

	//mainAppPrompt(decision);

	root = buildTreeFromFile("easy data set.csv", "4");
	string filename = "";
	switch(decision)
	{
	case 1:
		cout << "Enter name of CSV to parse into tree:";
		getline(cin, filename);
		cout << "Enter the column number to use as outcome variable:";
		getline(cin, outcome);

		root = buildTreeFromFile(filename, outcome);
	case 2:
		writeTreeToFile(root);
	case 3:
		root = readTreeFromFile();
		
	}

	return 0;

}