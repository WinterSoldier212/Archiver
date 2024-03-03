#pragma once

#include <map>
#include <vector>
#include <fstream>
#include "HuffmanTree.h"

void createHuffmanCode(Node* root, string str, map<unsigned char, string>& huffmanCode)
{
	if (root == nullptr)
		return;

	if (!root->left && !root->right) {
		huffmanCode[root->value] = str;
	}

	createHuffmanCode(root->left, str + "0", huffmanCode);
	createHuffmanCode(root->right, str + "1", huffmanCode);
}

map<unsigned char, string> getHuffmanCode(Node* root)
{
	map<unsigned char, string> huffmanCode;
	createHuffmanCode(root, "", huffmanCode);
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



std::string getBinaryText(std::map<unsigned char, std::string>& haffmanCode, const std::string& pathForFile)
{
	std::ifstream rfile(pathForFile, std::ios::in);
	std::string encodeText = "";

	char ch;
	while (!rfile.eof())
	{
		rfile >> ch;
		encodeText += haffmanCode[ch];
	}
	rfile.close();

	return encodeText;
}

std::string getFileTextInHuffmanCode(
	const std::string& pathForFile,
	std::map<unsigned char, std::string>& haffmanCode
) {
	char ch = 0;
	int counter = 0;
	char countZero = '0';

	std::string str = "";
	std::string binaryFileText = getBinaryText(haffmanCode, pathForFile);

	for (char& i : binaryFileText)
	{
		ch |= (i == '1') ? (1 << counter) : 0;
		counter++;

		if (counter == 8)
		{
			str += ch;
			counter = 0;
			ch = 0;
		}
	}

	countZero = 8 - counter + '0';
	ch <<= 8 - counter;

	return countZero + str;
}



Node* getTreeFromString(string str)
{
	char zeroSymbol = str[0];
	vector<Node*> nodes;

	for (int i = 0; i < str.size(); ++i)
	{
		Node* node = new Node{ str[i] };
		nodes.push_back(node);
	}

	Node* cur = nodes[0];
	auto it = nodes.begin(); it++;
	vector<Node*> zeroNodes;

	while (it != nodes.end())
	{
		Node* left = *it;
		cur->left = left;
		++it;

		Node* right = *it;
		cur->right = right;
		++it;

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


void translateHuffmanTreeIntoText(Node* root, char ch, std::string& huffmanTreeInText)
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

std::string getHuffmanTreeIntoText(Node* root, char ch)
{
	std::string huffmanTreeInText = "";
	translateHuffmanTreeIntoText(root, root->value, huffmanTreeInText);
	return huffmanTreeInText;
}