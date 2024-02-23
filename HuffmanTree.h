#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Node
{
	char value;
	Node* left = nullptr, * right = nullptr;

	Node(char x)
		: value(x)
	{}
	Node(char x, Node* left, Node* right)
		: value(x), left(left), right(right)
	{}
};

class HuffmanTree
{
public:
	Node* getHuffmanTree(std::string pathForFile)
	{
		vector<int> weight = getWeightsSymbolsFromFile(pathForFile);
		multimap<int, Node*> tree;

		for (int i = 0; i < 0x100; ++i)
		{
			if (weight.at(i) != 0)
			{
				char symbol = char(i);
				int weightSymbol = weight.at(i);
				Node* node = new Node(symbol);

				auto element = make_pair(weightSymbol, node);
				tree.insert(element);
			}
		}

		while (tree.size() > 1)
		{
			tree.insert(getNewTreeElement(tree));
		}

		return tree.begin()->second;
	}

private:
	std::vector<int>getWeightsSymbolsFromFile(const std::string& pathForFile)
	{
		std::ifstream rfile(pathForFile, std::ios::in);
		std::vector<int> weight(0x100);

		char ch;
		while (!rfile.eof())
		{
			rfile >> ch;
			++weight[unsigned char(ch)];
		}
		rfile.close();

		return weight;
	}

	pair<int, Node*> getNewTreeElement(multimap<int, Node*>& tree)
	{
		int weight_ = 0;
		Node* left = getAndDeleteElement(weight_, tree);
		Node* right = getAndDeleteElement(weight_, tree);

		Node* node_ = new Node('\0', left, right);

		return make_pair(weight_, node_);
	}

	Node* getAndDeleteElement(int& weight_, multimap<int, Node*>& tree)
	{
		weight_ += tree.begin()->first;
		Node* element = tree.begin()->second;
		tree.erase(tree.begin());

		return element;
	}
};