#pragma once

#include <map>
#include <iostream>
#include "ArchiveHeaders.h"

using namespace std;

class Archiver
{
	ofstream archive;
public:
	Archiver(string& pathForNewArchive)
	{
		string archiveName = getFreeFileNameInDirectory(pathForNewArchive);
		archive.open(archiveName, ios::out | ios::app);
	}

	~Archiver()
	{
		archive.close();
	}

	void addFile(const string& pathForFile)
	{
		if (!fileIsExist(pathForFile)) 
		{
			throw exception("File is not exist!");
		}

		auto&& byteFrequency = getByteFrequencyFromFile(pathForFile);
		auto&& huffmanTree = HuffmanTree().getHuffmanTree(byteFrequency);
		auto&& huffmanCode = HuffmanCode().getHuffmanCode(huffmanTree);

		string
			&& fileName = getFullFileNameFromPath(pathForFile),
			&& huffmanTreeInText = HuffmanTree().convertHuffmanTreeToString(huffmanTree),
			&& binaryText = getBinaryTextFromFileWithHuffmanCode(huffmanCode, pathForFile),
			&& textFromFileModifiedWithHuffmanCode = convertBinarySequenceToSetBytes(binaryText);
		
		HuffmanTree().deleteHuffmanTree(huffmanTree);

		writeTextWithTagToFile(fileName, Tag::FileName);
		writeTextWithTagToFile(huffmanTreeInText, Tag::HuffmanTree);
		writeTextWithTagToFile(textFromFileModifiedWithHuffmanCode, Tag::Text);
	}

private:
	string getFreeFileNameInDirectory(const string& pathForArchive)
	{
		if (!fileIsExist(pathForArchive + ".alzip"))
		{
			return pathForArchive + ".alzip";
		}

		string archiveName;
		for (int i = 1; 1 < 100'000; ++i)
		{
			archiveName = pathForArchive + to_string(i) + ".alzip";
			
			if (!fileIsExist(archiveName))
			{
				return archiveName;
			}
		}
		return "What_are_YOU";
	}

	vector<int>getByteFrequencyFromFile(const string& pathForFile)
	{
		ifstream rfile(pathForFile);
		vector<int> byteFrequency(0x100);

		char ch;
		while (!rfile.eof())
		{
			rfile >> ch;
			++byteFrequency[unsigned char(ch)];
		}
		rfile.close();

		return byteFrequency;
	}

	void writeTextWithTagToFile(string text, char tag)
	{
		archive << '<' << tag << '>';
		archive << text;
		archive << '<' << tag << '>';
		archive << endl;
	}

	string getBinaryTextFromFileWithHuffmanCode(
		map<unsigned char, string>& huffmanCode,
		const string& pathForFile
	) {
		ifstream rfile(pathForFile, ios::in);

		char ch;
		string encodeText = "";
		while (!rfile.eof())
		{
			rfile >> ch;
			encodeText += huffmanCode[ch];
		}
		rfile.close();

		return encodeText;
	}

	string convertBinarySequenceToSetBytes(const string& binaryFileText) {
		char zeroBitCounter = '0';
		string byteInString = "",
			binaryTextInByte = "";

		for (const char& i : binaryFileText)
		{
			byteInString += i;
			if (byteInString.size() == 8)
			{
				binaryTextInByte += convertBinarySequenceInByte(byteInString);
				byteInString = "";
			}
		}

		while (byteInString.size() != 8 && byteInString.size() != 0)
		{
			byteInString += '0';
			++zeroBitCounter;
		}
		binaryTextInByte += convertBinarySequenceInByte(byteInString);

		return zeroBitCounter + binaryTextInByte;
	}
};

void addFileInArchive(Archiver& archive, const string& pathForFile)
{
	try
	{
		cout << "Trying to write a file - " << pathForFile << endl;
		archive.addFile(pathForFile);
		cout << "The file has been successfully archived!" << endl << endl;
	}
	catch (exception& ex)
	{
		cout << "Error! " << ex.what() << endl << endl;
	}
}