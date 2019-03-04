#include "PA1.h"
#include <fstream>


//PA #1 TOOD: Generates a Huffman character tree from the supplied text
HuffmanTree<char>* PA1::huffmanTreeFromText(vector<string> data)
{

    //Builds a Huffman Tree from the supplied vector of strings.
    //This function implement's Huffman's Algorithm as specified in the
    //book.

    //In order for your tree to be the same as mine, you must take care
    //to do the following:
    //1.	When merging the two smallest subtrees, make sure to place the
    //      smallest tree on the left side!

	//store frequencies in hashtable
    unordered_map<char, int> frequencies{};
    for (auto word : data)
    {
        for (auto ch : word)
        {
            frequencies[ch]++;
        }
    }
	//maintains huffman tree forest
    priority_queue < HuffmanTree<char>*, vector<HuffmanTree<char>*>, TreeComparer> forest{};
    for (auto kvp : frequencies)
    {
        // .first is char, .second is weight/frequency
        forest.push(new HuffmanTree<char>(kvp.first, kvp.second));
    }
    // DO THIS UNTIL DONE MERGING TREES- MAKE ONE HUGE TREE, push back onto
    // priority queue
	while (forest.size() > 1)
	{
		HuffmanTree<char>* smaller = forest.top();
		forest.pop();
		HuffmanTree<char>* larger = forest.top();
		forest.pop();
		// merge them
		forest.push(new HuffmanTree<char>{ smaller,larger });
	}
    //TODO: return finished tree
    return forest.top();
}

void huffmanTreeFromMapHelper(char letter, string map, int position, HuffmanInternalNode<char>* node)
{
	// check if last character
	if (map[position + 1] == NULL)
	{
		if (map[position] == '0')
		{
			node->setLeftChild(new HuffmanLeafNode<char>{ letter, 1 });
			return;
		}
		else
		{
			node->setRightChild(new HuffmanLeafNode<char>{ letter, 1 });
			return;
		}
	}
	if (map[position] == '0')
	{
		//left
		if (node->getLeftChild() == nullptr)
		{
			node->setLeftChild(new HuffmanInternalNode<char>{ nullptr, nullptr });
			return huffmanTreeFromMapHelper(letter, map, position + 1, dynamic_cast<HuffmanInternalNode<char>*>(node->getLeftChild()));
		}
		// child already exists, traverse instead
		else
		{
			return huffmanTreeFromMapHelper(letter, map, position + 1, dynamic_cast<HuffmanInternalNode<char>*>(node->getLeftChild()));
		}
	}
	else
	{
		//right
		if (node->getRightChild() == nullptr)
		{
			node->setRightChild(new HuffmanInternalNode<char>{ nullptr, nullptr });
			return huffmanTreeFromMapHelper(letter, map, position + 1, dynamic_cast<HuffmanInternalNode<char>*>(node->getRightChild()));
		}
		// child already exists, traverse instead
		else
		{
			return huffmanTreeFromMapHelper(letter, map, position + 1, dynamic_cast<HuffmanInternalNode<char>*>(node->getRightChild()));
		}
	}

}
//PA #1 TODO: Generates a Huffman character tree from the supplied encoding map
//NOTE: I used a recursive helper function to solve this!
HuffmanTree<char>* PA1::huffmanTreeFromMap(unordered_map<char, string> huffmanMap)
{
    //Generates a Huffman Tree based on the supplied Huffman Map.Recall that a
    //Huffman Map contains a series of codes(e.g. 'a' = > 001).Each digit(0, 1)
    //in a given code corresponds to a left branch for 0 and right branch for 1.
	
	// create tree
	HuffmanNode<char>* root = new HuffmanInternalNode<char>{ nullptr,nullptr };
	HuffmanInternalNode<char>* current = dynamic_cast<HuffmanInternalNode<char>*>(root);

	for (auto i : huffmanMap)
	{
		huffmanTreeFromMapHelper(i.first, i.second, 0, current );

	}
	HuffmanTree<char>* tree = new HuffmanTree<char>(current);
    return tree;
}

//PA #1 TODO: Generates a Huffman encoding map from the supplied Huffman tree
//NOTE: I used a recursive helper function to solve this!
void huffmanEncodingMapFromTreeHelper(
    unordered_map<char, string>&map,
    HuffmanNode<char>* node,
    string encoding)
{
    if (node->isLeaf() == false)
    {
        // not a leaf, make recursive calls
        HuffmanInternalNode<char>* root =
            dynamic_cast<HuffmanInternalNode<char>*>(node);
        huffmanEncodingMapFromTreeHelper(
            map,
            root->getLeftChild(),
            encoding + "0"
        );
        huffmanEncodingMapFromTreeHelper(
            map,
            root->getRightChild(),
            encoding + "1"
        );
        return;
    }
    else
    {
        // this is a leaf. This means that we have a complete mapping for
        // this character.
        // this converst node, which is a HuffmanNode, into a leaf node
        //                             convert      type e want           what we want to convert
        // base case no recursion
        HuffmanLeafNode<char>* root = dynamic_cast<HuffmanLeafNode<char>*> (node);
        map[root->getValue()] = encoding;
        return;
    }
}
unordered_map<char, string> PA1::huffmanEncodingMapFromTree(HuffmanTree<char> *tree)
{
    /*
	HuffmanNode<char> *node = tree->getRoot();
	if (node->isLeaf() == true)
	{
		HuffmanLeafNode<char> *leaf = dynamic_cast<HuffmanLeafNode<char> *>(node);
		leaf = (HuffmanLeafNode<char> *)node;
	}*/


    //Generates a Huffman Map based on the supplied Huffman Tree.  Again, recall
    //that a Huffman Map contains a series of codes(e.g. 'a' = > 001).Each digit(0, 1)
    //in a given code corresponds to a left branch for 0 and right branch for 1.
    //As such, a given code represents a pre-order traversal of that bit of the
    //tree.  I used recursion to solve this problem.
    unordered_map<char, string> result{};
    huffmanEncodingMapFromTreeHelper(result, tree->getRoot(), "");
    return result;
}

//PA #1 TODO: Writes an encoding map to file.  Needed for decompression.
void PA1::writeEncodingMapToFile(unordered_map<char, string> huffmanMap, string file_name)
{
    //Writes the supplied encoding map to a file.  My map file has one
    //association per line (e.g. 'a' and 001 would yield the line "a001")
	ofstream file_out{ file_name };
	for (auto map_element : huffmanMap)
	{
		file_out << map_element.first << map_element.second << endl;
	}
	file_out.close();
}

//PA #1 TODO: Reads an encoding map from a file.  Needed for decompression.
unordered_map<char, string> PA1::readEncodingMapFromFile(string file_name)
{
    //Creates a Huffman Map from the supplied file.Essentially, this is the
    //inverse of writeEncodingMapToFile.
    unordered_map<char, string> result{};
	ifstream map_file{ file_name };

	if (map_file.is_open() == true)
	{
		string in = "";
		while (map_file.good() == true)
		{
			getline(map_file, in);
			if (in == "") continue;
			char character = in.at(0);
			string binary = in.substr(1);
			result[character] = binary;
		}
	}

    return result;
}

//PA #1 TODO: Converts a vector of bits (bool) back into readable text using the supplied Huffman map
string PA1::decodeBits(vector<bool> bits, unordered_map<char, string> huffmanMap)
{
    //Uses the supplied Huffman Map to convert the vector of bools (bits) back into text.
    //To solve this problem, I converted the Huffman Map into a Huffman Tree and used
    //tree traversals to convert the bits back into text.
    ostringstream result{};
	HuffmanTree<char>* tree = huffmanTreeFromMap(huffmanMap);
	HuffmanNode<char>* root = tree->getRoot();
	HuffmanInternalNode<char>* current = dynamic_cast<HuffmanInternalNode<char>*>(root);
	for (int i = 0; i < bits.size(); i++)
	{
		if (bits[i] == 0)
		{
			//HuffmanInternalNode<char>* temp = current;
			//check if current is leaf node
			if (current->getLeftChild()->isLeaf() == true)
			{
				// recast to leaf node
				HuffmanLeafNode<char>* leaf = dynamic_cast<HuffmanLeafNode<char>*>(current->getLeftChild());
				result << leaf->getValue();
				current = dynamic_cast<HuffmanInternalNode<char>*>(root);
				continue;
			}
			else
			{
				// else simply get the next one
				current = dynamic_cast<HuffmanInternalNode<char>*>(current->getLeftChild());

			}
		}
		else
		{
			//check if next is leaf node
			if (current->getRightChild()->isLeaf() == true)
			{
				// recast to leaf node
				HuffmanLeafNode<char>* leaf = dynamic_cast<HuffmanLeafNode<char>*>(current->getRightChild());
				result << leaf->getValue();
				current = dynamic_cast<HuffmanInternalNode<char>*>(root);
				continue;
			}
			else
			{
				current = dynamic_cast<HuffmanInternalNode<char>*>(current->getRightChild());
					
			}
		}
	}
    return result.str();
}

//PA #1 TODO: Using the supplied Huffman map compression, converts the supplied text into a series of bits (boolean values)
vector<bool> PA1::toBinary(vector<string> text, unordered_map<char, string> huffmanMap)
{
    vector<bool> result{};
	for (auto str : text)
	{
		string code;
		for (auto ch : str)
		{
			code = huffmanMap[ch];
			for (auto bit : code)
			{
				if (bit == '0')
				{
					result.push_back(0);
				}
				else
				{
					result.push_back(1);
				}
			}
		}
	}
    return result;
}
