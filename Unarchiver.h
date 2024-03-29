#pragma once

#include <regex>
#include <optional>

#include "ArchiveHeaders.h"

using namespace std;

class Unarchiver
{
	ifstream archive;
	string pathUnzip = getCurrentDirectory();
public:
	Unarchiver(string pathForArchive)
	{
		if (fileIsExist(pathForArchive))
		{
			archive.open(pathForArchive);
		}
		else
		{
			throw exception("Archive not exist!!");
		}
	}

	void extractFile(const string& outputFileDirectory)
	{
		if (archive.eof())
		{
			throw exception("");
		}

		string 
			&& fileName = getTextInTagFromFile(archive, Tag::FileName),
			&& fileHuffmanTree = getTextInTagFromFile(archive, Tag::HuffmanTree),
			&& fileText = getTextInTagFromFile(archive, Tag::Text);

		auto huffmanTree = HuffmanTree().convertStringToHuffmanTree(fileHuffmanTree);
		auto reversHummanCode = HuffmanCode().getReverseHuffmanCode(huffmanTree);

		if (fileIsExist(outputFileDirectory + "\\" + fileName))
		{
		//	fileName = getNewFileNameFromUser(fileName);
		}

		ofstream wfile(outputFileDirectory + "\\" + fileName);

	}

	~Unarchiver()
	{
		archive.close();
	}

private:
	string getTextInTagFromFile(std::ifstream& rfile, char tag)
	{
		optional<string> opt;
		std::string str,
			textWithTagFromFile = "";

		while (!opt.has_value())
		{
			rfile >> str;
			textWithTagFromFile += str;
			opt = ejectContentFromTag(textWithTagFromFile, tag);
		}

		return opt.value();
	}

	optional<string> ejectContentFromTag(string str, char tag)
	{
		smatch result;
		regex regular(
			"(<" + string(1, tag) + ">)"
			"([A-Za-z�-��-�0-9._-]+)"
			"(<" + string(1, tag) + ">)");

		if (regex_search(str, result, regular))
		{
			return optional<string> {result[2]};
		}
		return nullopt;
	}
};

bool extractFileFromArchive(Unarchiver& Unarchiver, const string& outputFileDirectory)
{
	try
	{
		cout << "Trying to get a file - " << outputFileDirectory << endl;
		Unarchiver.extractFile(outputFileDirectory);
		cout << "The file has been successfully unarchived!" << endl << endl;

		return true;
	}
	catch (exception& ex)
	{
		cout << "The file could not be extracted from the archive." << endl << endl;
		return false;
	}
}