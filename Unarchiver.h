#pragma once

#include <regex>
#include <optional>

#include "ArchiveHeaders.h"

using namespace std;

class Unarchiver
{
public:
	Unarchiver(std::string pathForArchive, std::string pathUnzip)
	{
		if (fileIsExist(pathForArchive))
		{
			archive.open(pathForArchive);

			while (!archive.eof())
			{
				string fileName = getTextInTagFromFile(archive, Tag::FileName),
					huffmanTree = getTextInTagFromFile(archive, Tag::HuffmanTree),
					text = getTextInTagFromFile(archive, Tag::Text);

				HuffmanTree tree;
			}
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
			"([A-Za-zР-пр-џ0-9._-]+)"
			"(<" + string(1, tag) + ">)");

		if (regex_search(str, result, regular))
		{
			return optional<string> {result[2]};
		}
		return nullopt;
	}
};