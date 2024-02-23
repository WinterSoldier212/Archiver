#pragma once

#include <fstream>
#include "HuffmanTree.h"
#include "HuffmanCode.h"
#include "FileFunctions.h"

using namespace std;

class Archive
{
public:
	Archive(std::string pathForArchive)
	{
		if (fileIsExist(pathForArchive) == true)
		{
			archive.open(pathForArchive, ios::app);
		}
		else
		{
			archive.open(pathForArchive, ios::in);
		}
	}

	~Archive()
	{
		archive.close();
	}

	void addFile(std::string pathForFile)
	{
		auto huffmanTree = HuffmanTree().getHuffmanTree(pathForFile);
		auto huffmanCode = getHuffmanCode(huffmanTree);

		std::string fileName = getFileNameFromPath(pathForFile);
		std::string huffmanTreeInText = getHuffmanTreeInText(huffmanTree);
		std::string fileTextInHuffmanCode = getFileTextInHuffmanCode(pathForFile, huffmanCode);

		archive << "<N>" << fileName << "\n";
		archive << "<T>" << huffmanTreeInText << "\n";
		archive << "<C>" << fileTextInHuffmanCode << "\n";
	}

private:
	fstream archive;
};

