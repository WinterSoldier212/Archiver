#pragma once

#include <regex>
#include <optional>

#include "ArchiveHeaders.h"

using namespace std;

class Unarchiver : public Archive
{
public:
	Unarchiver() = default;
	~Unarchiver()
	{
		Close();
	}

	void ExtractFile(const string& outputFileDirectory)
	{
		if (!IsOpen())
		{
			throw ExceptionArchiveNotOpen(archiveName);
		}
		if (archive.eof())
		{
			throw ExceptionArchiveEof(archiveName);
		}

		string&& fileName = getTextInTagFromFile(archive, Tag::FileName);
		string&& fileHuffmanTree = getTextInTagFromFile(archive, Tag::HuffmanTree);
		string&& fileText = getTextInTagFromFile(archive, Tag::Text);

		auto huffmanTree = HuffmanTree().convertStringToHuffmanTree(fileHuffmanTree);
		auto reversHummanCode = HuffmanCode().getReverseHuffmanCode(huffmanTree);

		if (fileIsExist(outputFileDirectory + "\\" + fileName))
		{
			string clearFileName = getFileNameFromPath(fileName);
			string clearFileExtension = getFileExtensionFromPath(fileName);

			fileName = getFreeFileNameInDirectory(outputFileDirectory + "\\" + clearFileName, clearFileExtension);
		}

		ofstream wfile(outputFileDirectory + "\\" + fileName);

	}

private:
	string getTextInTagFromFile(std::fstream& rfile, char tag)
	{
		optional<string> opt;
		std::string str,
			textWithTagFromFile = "";

		while (!opt.has_value())
		{
			getline(rfile, str);
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

bool ExtractFileFromArchive(Unarchiver& Unarchiver, const string& outputFileDirectory)
{
	try
	{
		logFile << "Trying to extract file from Archive - " << Unarchiver.GetName() << endl;
		Unarchiver.ExtractFile(outputFileDirectory);
		logFile << "The file has been successfully extracted!" << endl;
	}
	catch (ExceptionArchiveNotOpen& ex)
	{
		logFile << "Error! " << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
	}
	catch (ExceptionArchiveEof& ex)
	{
		logFile << "Error! " << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
	}
}

void ExtractAllFilesFromArchive(Unarchiver& Unarchiver, const string& outputFileDirectory)
{

}