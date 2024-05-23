#pragma once

#include <regex>
#include <optional>
#include <iostream>

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

	virtual void Open(string pathForArhcive) override
	{
		if (!fileIsExist(pathForArhcive))
		{
			throw ExceptionFileNotExist(pathForArhcive);
		}
		Close();

		archiveName = pathForArhcive;
		archive.open(pathForArhcive, ios::in);
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
		cout << 15 << endl;
		auto reverseHummanCode = HuffmanCode::getReverseHuffmanCode(huffmanTree);
		cout << 15 << endl;

		if (fileIsExist(outputFileDirectory + "\\" + fileName))
		{
			string clearFileName = getFileNameFromPath(fileName);
			string clearFileExtension = getFileExtensionFromPath(fileName);

			fileName = getFreeFileNameInDirectory(outputFileDirectory + "\\" + clearFileName, clearFileExtension);
		}
		cout << 16 << endl;

		ofstream wfile(outputFileDirectory + "\\" + fileName);
		
		cout << fileText << endl << endl;

		string binaryFileText = "";
		for (char ch : fileText)
		{
			binaryFileText += Convert.byteToBinarySequence(ch);
		}

		cout << binaryFileText << endl << endl;

		int zeroBits = fileText[0] - '0';
		int clearLenghtBinaryFileText = binaryFileText.length() - zeroBits;

		string binarySetForWriteInFile = "";
		for (int i = 8; i < clearLenghtBinaryFileText; i++)
		{
			binarySetForWriteInFile += binaryFileText[i];

			if (reverseHummanCode.count(binarySetForWriteInFile))
			{
				wfile << reverseHummanCode.at(binarySetForWriteInFile);
				binarySetForWriteInFile = "";
			}
		}
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
			"([\\s\\S]+)"
			"(<" + string(1, tag) + ">)");

		if (regex_search(str, result, regular))
		{
			return optional<string> {result[2]};
		}
		return nullopt;
	}
};

bool ExtractFileFromArchive(Unarchiver& unarchiver, const string& outputFileDirectory)
{
	try
	{
		logFile << __TIME__  << " Trying to extract file from Archive - " << unarchiver.GetName() << endl;
		unarchiver.ExtractFile(outputFileDirectory);
		logFile << __TIME__ << " The file has been successfully extracted!" << endl;
		return true;
	}
	catch (ExceptionArchiveNotOpen& ex)
	{
		logFile << "Error! " << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
	}
	catch (ExceptionArchiveEof& ex)
	{
		logFile << "Error! " << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
	}
	return false;
}

void ExtractAllFilesFromArchive(Unarchiver& unarchiver, const string& outputFileDirectory)
{
	while (ExtractFileFromArchive(unarchiver, outputFileDirectory));
}