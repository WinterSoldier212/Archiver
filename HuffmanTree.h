#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

struct Node
{
	char value = '\0';
	bool isNode = true;
	Node* left = nullptr, * right = nullptr;

	Node(char x, bool isNode_)
		: value(x), isNode(isNode_)
	{}
	Node(Node* left, Node* right)
		: left(left), right(right)
	{}
};

class HuffmanTree
{
public:
	HuffmanTree() = delete;

	static Node* getHuffmanTree(vector<size_t> byteWeights)
	{
		multimap<int, Node*> tree_;

		char symbol;
		int byteWeight;

		for (int i = 0; i < 0x100; ++i)
		{
			if (byteWeights.at(i) != 0)
			{
				symbol = char(i);
				byteWeight = byteWeights.at(i);
				Node* node = new Node(symbol, false);

				auto element = make_pair(byteWeight, node);
				tree_.insert(element);
			}
		}

		while (tree_.size() > 1)
		{
			tree_.insert(goTwoNodesIntoOne(tree_));
		}

		return tree_.begin()->second;
	}

	static void deleteHuffmanTree(Node* tree)
	{
		if (tree == nullptr)
			return;

		if (!tree->left && !tree->right)
			deleteHuffmanTree(tree->left);
		deleteHuffmanTree(tree->right);

		delete tree;
	}

	static Node* convertStringToHuffmanTree(string str)
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

			if (left->isNode && right->isNode)
			{
				cur = left;
				zeroNodes.push_back(right);
			}
			else if (left->isNode && !right->isNode)
			{
				cur = left;
			}
			else if (!left->isNode && right->isNode)
			{
				cur = right;
			}
			else if (!left->isNode && !right->isNode && !zeroNodes.empty())
			{
				int lastElement = zeroNodes.size() - 1;
				cur = zeroNodes[lastElement];
				zeroNodes.pop_back();
			}
		}
		
		return nodes[0];
	}

	static string convertHuffmanTreeToString(Node* tree)
	{
		string huffmanTreeInText = string(1, '\0');
		translateHuffmanTreeIntoText(tree, huffmanTreeInText);
		return huffmanTreeInText;
	}

private:
	static void fillTreeNodesWithSymbols(std::string& str, std::vector<Node*>& nodes)
	{
		Node* node;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (str[i] == '\\') 
			{
				node = new Node(str[++i], false);
			} 
			else if (str[i] == '\0')
			{
				node = new Node(str[i], true);
			}
			else
			{
				node = new Node(str[i], false);
			}
			nodes.push_back(node);
		}
	}

	static pair<int, Node*> goTwoNodesIntoOne(multimap<int, Node*>& tree)
	{
		int weight_ = 0;

		Node* left = getAndDeleteElement(weight_, tree);
		Node* right = getAndDeleteElement(weight_, tree);
		Node* node_ = new Node(left, right);

		return make_pair(weight_, node_);
	}

	static Node* getAndDeleteElement(int& weight_, multimap<int, Node*>& tree)
	{
		weight_ += tree.begin()->first;
		Node* element = tree.begin()->second;
		tree.erase(tree.begin());

		return element;
	}

	static void translateHuffmanTreeIntoText(Node* root, string& huffmanTreeInText)
	{
		if (root == nullptr)
			return;

		if (root->isNode)
		{
			Node* left = root->left;
			Node* right = root->right;

			if (!left->isNode && (left->value == '\0' || left->value == '\\'))
				huffmanTreeInText += '\\';
			huffmanTreeInText += left->value;

			if (!right->isNode && (right->value == '\0' || right->value == '\\'))
				huffmanTreeInText += '\\';
			huffmanTreeInText += right->value;

			translateHuffmanTreeIntoText(left, huffmanTreeInText);
			translateHuffmanTreeIntoText(right, huffmanTreeInText);
		}
	}
};