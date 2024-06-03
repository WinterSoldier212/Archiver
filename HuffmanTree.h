#pragma once

#include <map>
#include <vector>
#include <string>

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
	Node* getHuffmanTree(vector<int> byteWeights, char freeSymbol)
	{
		multimap<int, Node*> tree_;

		char symbol;
		int byteWeight;

		for (int i = 1; i < 0x100; ++i)
		{
			if (byteWeights.at(i) != 0)
			{
				symbol = char(i);
				byteWeight = byteWeights.at(i);
				Node* node = new Node(symbol);

				auto element = make_pair(byteWeight, node);
				tree_.insert(element);
			}
		}

		while (tree_.size() > 1)
		{
			tree_.insert(getNewTreeElement(tree_, freeSymbol));
		}

		return tree_.begin()->second;
	}

	void deleteHuffmanTree(Node* tree)
	{
		if (tree == nullptr)
			return;

		if (!tree->left && !tree->right)
			deleteHuffmanTree(tree->left);
		deleteHuffmanTree(tree->right);

		delete tree;
	}

	Node* convertStringToHuffmanTree(string str)
	{
		char zeroSymbol = str[0];
		vector<Node*> nodes;

		fillTreeNodesWithSymbols(str, nodes);

		Node* cur = nodes[0];
		auto it = nodes.begin(); it++;
		vector<Node*> zeroNodes;

		while (it != nodes.end())
		{
			Node* left = *it;
			cur->left = left;
			++it;

			if (it == nodes.end())
				break;

			Node* right = *it;
			cur->right = right;
			++it;

			if (it == nodes.end())
				break;

			if (left->value == zeroSymbol && right->value == zeroSymbol)
			{
				cur = left;
				zeroNodes.push_back(right);
			}
			else if (left->value == zeroSymbol && right->value != zeroSymbol)
			{
				cur = left;
			}
			else if (left->value != zeroSymbol && right->value == zeroSymbol)
			{
				cur = right;
			}
			else if (left->value != zeroSymbol && right->value != zeroSymbol && !zeroNodes.empty())
			{
				int lastElement = zeroNodes.size() - 1;
				cur = zeroNodes[lastElement];
				zeroNodes.pop_back();
			}
		}
		
		return nodes[0];
	}

	string convertHuffmanTreeToString(Node* tree, char freeSymbol)
	{
		string huffmanTreeInText = string(1, freeSymbol);
		translateHuffmanTreeIntoText(tree, tree->value, huffmanTreeInText);
		return huffmanTreeInText;
	}

private:
	void fillTreeNodesWithSymbols(std::string& str, std::vector<Node*>& nodes)
	{
		for (unsigned long long i = 0; i < str.size(); ++i)
		{
			Node* node = new Node{ str[i] };
			nodes.push_back(node);
		}
	}

	pair<int, Node*> getNewTreeElement(multimap<int, Node*>& tree, char null_symbol)
	{
		int weight_ = 0;

		Node* left = getAndDeleteElement(weight_, tree);
		Node* right = getAndDeleteElement(weight_, tree);
		Node* node_ = new Node(null_symbol, left, right);

		return make_pair(weight_, node_);
	}

	Node* getAndDeleteElement(int& weight_, multimap<int, Node*>& tree)
	{
		weight_ += tree.begin()->first;
		Node* element = tree.begin()->second;
		tree.erase(tree.begin());

		return element;
	}

	void translateHuffmanTreeIntoText(Node* root, char ch, string& huffmanTreeInText)
	{
		if (root == nullptr)
			return;

		if (root->value == ch)
		{
			huffmanTreeInText += root->left->value;
			huffmanTreeInText += root->right->value;

			translateHuffmanTreeIntoText(root->left, ch, huffmanTreeInText);
			translateHuffmanTreeIntoText(root->right, ch, huffmanTreeInText);
		}
	}
};