#pragma once

#include <fstream>
#include "HuffmanTree.h"
#include "HuffmanCode.h"
#include "FileFunctions.h"
#include <string>

using namespace std;

class Unarchiver
{
public:
	Unarchiver(std::string pathForArchive)
	{
		if (fileIsExist(pathForArchive))
		{
			//get Files in archive		
		}
		else
		{
			throw exception("Archive not exist!!");
		}
	}

	~Unarchiver()
	{
		archive.close();
	}

private:
	ifstream archive;
};