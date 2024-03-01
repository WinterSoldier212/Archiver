#pragma once

#include <fstream>
#include "HuffmanTree.h"
#include "HuffmanCode.h"
#include "FileFunctions.h"
#include <string>

using namespace std;

class Archiver
{
public:
	Archiver(std::string pathForArchive)
	{
		string archiveName;
		if (fileIsExist(pathForArchive + ".alzip"))
		{
			for (int i = 1; 1 < 100'000; ++i)
			{
				archiveName = pathForArchive + to_string(i) + ".alzip";
				if (!fileIsExist(archiveName))
				{
					archive.open(archiveName, ios::out | ios::app);
					break;
				}
			}
		}
		else
		{
			archive.open((pathForArchive + ".alzip"), ios::out | ios::app);
		}
	}

	~Archiver()
	{
		archive.close();
	}

	void addFile(std::string pathForFile)
	{
		if (!fileIsExist(pathForFile)) {;
			throw exception("File is not exist!");
		}

		auto huffmanTree = HuffmanTree().getHuffmanTree(pathForFile);
		auto huffmanCode = getHuffmanCode(huffmanTree);

		std::string fileName = getFileNameFromPath(pathForFile);
		std::string huffmanTreeInText = getHuffmanTreeIntoText(huffmanTree, huffmanTree->value);
		std::string fileTextInHuffmanCode = getFileTextInHuffmanCode(pathForFile, huffmanCode);
		
		deleteTree(huffmanTree);

		archive << "<N>" << fileName << "\n";
		archive << "<T>" << huffmanTreeInText << "\n";
		archive << "<C>" << fileTextInHuffmanCode << "\n";
	}

private:
	ofstream archive;
};