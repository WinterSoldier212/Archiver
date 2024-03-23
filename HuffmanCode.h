#pragma once

#include "HuffmanTree.h"

class HuffmanCode
{
public:
	map<unsigned char, string> getHuffmanCode(Node* root)
	{
		map<unsigned char, string> huffmanCode;
		createHuffmanCode(root, "", huffmanCode);
		return huffmanCode;
	}

	map<string, char> getReverseHuffmanCode(Node* root)
	{
		map<unsigned char, string>&& huffmanCode = getHuffmanCode(root);
		map<string, char> reverseMap;

		for (auto& i : huffmanCode)
		{
			reverseMap.insert(make_pair(i.second, i.first));
		}

		return reverseMap;
	}
private:
	void createHuffmanCode(Node* root, std::string str, std::map<unsigned char, string>& huffmanCode)
	{
		if (root == nullptr)
			return;

		if (!root->left && !root->right) {
			huffmanCode[root->value] = str;
		}

		createHuffmanCode(root->left, str + "0", huffmanCode);
		createHuffmanCode(root->right, str + "1", huffmanCode);
	}
};