#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

class HuffmanTree
{
public:
	map<char, string> getHuffmanCode(const vector<int>& weight)
	{
		if (weight.empty())
		{
			throw exception("---Empty Weight---");
		}

		Node* tree = getRootHuffmanTree(weight);

		string str = (tree->value == '\0' ? "" : "0");

		map<char, string> huffmanCode;
		createHuffmanCode(tree, huffmanCode, str);

		return huffmanCode;
	}

	map<string, char> getReverseHuffmanCode(map<char, string> HuffmanCode)
	{
		map<string, char> reverseMap;

		for (auto& i : HuffmanCode)
		{
			reverseMap.insert(make_pair(i.second, i.first));
		}

		return reverseMap;
	}
private:
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

	Node* getRootHuffmanTree(const vector<int>& weight)
	{
		multimap<int, Node*> tree;

		fillHuffmanTree(tree, weight);
		adjustHuffmanTree(tree);

		Node* root = tree.begin()->second;
		return root;
	}

	void fillHuffmanTree(multimap<int, Node*>& tree, const vector<int>& weight)
	{
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
	}

	void adjustHuffmanTree(multimap<int, Node*>& tree)
	{
		while (tree.size() > 1)
		{
			tree.insert(getNewTreeElement(tree));
		}
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

	void createHuffmanCode(Node* tree, map<char, string>& huffmanCode, string str) {
		if (tree == nullptr)
			return;

		if (!tree->left && !tree->right) {
			huffmanCode[tree->value] = str;
		}

		createHuffmanCode(tree->left, huffmanCode, str + "0");
		createHuffmanCode(tree->right, huffmanCode, str + "1");

		delete tree;
	}
};