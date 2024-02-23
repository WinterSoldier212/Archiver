#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include "HuffmanTree.h"

std::string getBinaryText(std::map<char, std::string>& haffmanCode, const std::string& pathForFile)
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
	std::map<char, std::string>& haffmanCode
) {
	char ch = 0;
	int counter = 0;

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

	char countZero = '0';
	while (counter != 0)
	{
		countZero++;
		counter++;
		str += '0';

		if (counter == 8)
			counter = 0;
	}

	return countZero + str;
}

//string str = (tree->value == '\0' ? "" : "0");
//
//map<char, string> huffmanCode;
//createHuffmanCode(tree, huffmanCode, str);

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

map<char, string> getHuffmanCode(Node* root)
{
	map<char, string> huffmanCode;
	string str = "";
	stack<Node*> parants;

	bool go = true;
	while (go == true)
	{
		while (root->left != nullptr)
		{
			parants.push(root);
			root = root->left;
		}

		huffmanCode[root->value] = str;
		root = parants.top(); parants.pop();
		delete root->left;

		while (!root->right)
		{

		}
	}

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

// getHuffmanTreeInText