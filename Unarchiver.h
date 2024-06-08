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

	virtual void Open(string pathForArhcive) override
	{
		if (!fileIsExist(pathForArhcive))
		{
			throw ExceptionFileNotExist(pathForArhcive);
		}
		Close();

		archiveName = pathForArhcive;
		archive.open(pathForArhcive, ios::in | ios::binary);
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

		string&& binaryFileText = Convert::stringToBinarySequence(fileText);

		auto huffmanTree = HuffmanTree::convertStringToHuffmanTree(fileHuffmanTree);
		auto reverseHummanCode = HuffmanCode::getReverseHuffmanCode(huffmanTree);

		if (fileIsExist(outputFileDirectory + "\\" + fileName)) {
			string clearFileName = getFileNameFromPath(fileName);
			string clearFileExtension = getFileExtensionFromPath(fileName);

			fileName = getFreeFileNameInDirectory(outputFileDirectory + "\\" + clearFileName, clearFileExtension);
		}
		else {
			fileName = outputFileDirectory + "\\" + fileName;
		}
		ofstream wfile(fileName, ios::out | ios::binary);
		
		int zeroBits = fileText.at(0) - '0';
		size_t clearLenghtBinaryFileText = binaryFileText.length() - zeroBits;
		
		string binarySetForWriteInFile = "";
		for (size_t i = 8; i < clearLenghtBinaryFileText; i++)
		{
			binarySetForWriteInFile += binaryFileText.at(i);
			
			if (reverseHummanCode.count(binarySetForWriteInFile))
			{
				wfile.put(reverseHummanCode.at(binarySetForWriteInFile));
				binarySetForWriteInFile = string();
			}
		}
		wfile.close();
	} 
private:
	string getTextInTagFromFile(fstream& rfile, const char tag)
	{
		optional<string> opt;
		string textWithTagFromFile = string();

		string str;
		while (!opt.has_value())
		{
			getline(rfile, str);
			textWithTagFromFile += str + '\n';
			
			if (str == ("<" + string(1, tag) + ">"))
				opt = ejectContentFromTag(textWithTagFromFile, tag);
		}
		return opt.value();
	}

	optional<string> ejectContentFromTag(string str, const char tag)
	{
		smatch result;
		regex regular(
			"(<" + string(1, tag) + ">)"
			"([\n])"
			"([\\s\\S]*)"
			"([\n])"
			"(<" + string(1, tag) + ">)"
		);

		if (regex_search(str, result, regular))
		{
			return optional<string> {result[3]};
		}
		return nullopt;
	}
};

bool ExtractFileFromArchive(Unarchiver& unarchiver, const string& outputFileDirectory)
{
	try
	{
		logFile << __TIME__ << " Trying to extract file from Archive - " << unarchiver.GetName() << endl;
		unarchiver.ExtractFile(outputFileDirectory);
		logFile << __TIME__ << " The file has been successfully extracted!" << endl;
		return true;
	}
	catch (ExceptionArchiveNotOpen& ex)
	{
		logFile << " Error Open! " << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
		return false;
	}
	catch (ExceptionArchiveEof& ex)
	{
		logFile << "Archive EOF!" << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
		return false;
	}
	catch (exception& ex)
	{
		cout << ex.what();
		return false;
	}
}

void ExtractAllFilesFromArchive(Unarchiver& unarchiver, const string& outputFileDirectory)
{
	bool flag;
	do
	{
		flag = ExtractFileFromArchive(unarchiver, outputFileDirectory);
	}
	while (flag);
}