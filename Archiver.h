#pragma once

#include <map>
#include "ArchiveHeaders.h"
#include <iostream>

using namespace std;

class Archiver : public Archive
{
public:
	Archiver() = default;
	~Archiver()
	{
		Close();
	}

	void Create(string pathForNewArhcive)
	{
		Close();

		archiveName = getFreeFileNameInDirectory(pathForNewArhcive, ".alzip");
		archive.open(archiveName, ios::out);
	}

	virtual void Open(string pathForArhcive) override
	{
		if (!fileIsExist(pathForArhcive))
		{
			throw ExceptionFileNotExist(pathForArhcive);
		}
		Close();

		archiveName = getFreeFileNameInDirectory(pathForArhcive, ".alzip");
		archive.open(archiveName, ios::app);
	}

	void AddFile(const string& pathForFile)
	{
		if (!fileIsExist(pathForFile))
		{
			throw ExceptionFileNotExist(pathForFile);
		}
		if (!IsOpen())
		{
			throw ExceptionArchiveNotOpen(archiveName);
		}

		auto&& byteFrequency = getByteFrequencyFromFile(pathForFile);
		char freeSymbol = getFreeSymbol(byteFrequency);

		auto&& huffmanTree = HuffmanTree().getHuffmanTree(byteFrequency, freeSymbol);
		auto&& huffmanCode = HuffmanCode().getHuffmanCode(huffmanTree);

		string&& fileName = getFullFileNameFromPath(pathForFile);
		string&& huffmanTreeInText = HuffmanTree().convertHuffmanTreeToString(huffmanTree, freeSymbol);
		string&& binaryText = getBinaryTextFromFileWithHuffmanCode(huffmanCode, pathForFile);
		string&& textFromFileModifiedWithHuffmanCode = Convert.binarySequenceToSetBytes(binaryText);

		HuffmanTree().deleteHuffmanTree(huffmanTree);

		writeTextWithTagToFile(fileName, Tag::FileName);
		writeTextWithTagToFile(huffmanTreeInText, Tag::HuffmanTree);
		writeTextWithTagToFile(textFromFileModifiedWithHuffmanCode, Tag::Text);
	}

private:
	char getFreeSymbol(vector<int>& byteWeights)
	{
		for (int i = 1; i < 0x100; ++i)
		{
			if (byteWeights.at(i) == 0)
			{
				return (char)i;
			}
		}
	}

	vector<int>getByteFrequencyFromFile(const string& pathForFile)
	{
		ifstream rfile(pathForFile, ios::binary);
		vector<int> byteFrequency(0x100);

		char ch;
		while (rfile.get(ch))
		{
			++byteFrequency[unsigned char(ch)];
		}
		rfile.close();

		return byteFrequency;
	}

	void writeTextWithTagToFile(string text, char tag)
	{
		archive.put('<').put(tag).put('>');
		archive << text;
		archive.put('<').put(tag).put('>');
		archive.put('\n');
	}

	string getBinaryTextFromFileWithHuffmanCode(
		map<unsigned char, string>& huffmanCode,
		const string& pathForFile
	) {
		ifstream rfile(pathForFile, ios::in);

		char ch;
		string encodeText = "";
		while (rfile.get(ch))
		{
			encodeText += huffmanCode[ch];
		}
		rfile.close();

		return encodeText;
	}
};

void AddFileInArchive(Archiver& archive, const string& pathForFile)
{
	try
	{
		logFile << __TIME__  << " Trying to write a file - " << pathForFile << " in Archive - " << archive.GetName() << endl;
		archive.AddFile(pathForFile);
		logFile << __TIME__  << " The file has been successfully archived!" << endl;
	}
	catch (ExceptionArchiveNotOpen& ex)
	{
		logFile << "Error! " << __TIME__ << endl << ex.what() << ex.GetArchiveName() << endl;
	}
	catch (ExceptionFileNotExist& ex)
	{
		logFile << "Error! " << __TIME__ << endl << ex.what() << ex.GetFileName() << endl;
	}
}

void AddDirectoryInArchive(Archiver& archive, const string& pathForDirectory)
{
	for (const auto& entry : filesystem::recursive_directory_iterator(pathForDirectory))
	{
		if (entry.is_directory())
			AddDirectoryInArchive(archive, entry.path().string());

		AddFileInArchive(archive, entry.path().string());
	}
}