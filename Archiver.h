#pragma once

#include <map>
#include <iostream>
#include "ArchiveHeaders.h"

using namespace std;

class Archiver
{
	ofstream archive;
public:
	Archiver(const string& pathForArchive)
	{
		string archiveName = getFreeFileNameInDirectory(pathForArchive);
		archive.open(archiveName, ios::out | ios::app);
	}

	~Archiver()
	{
		archive.close();
	}

	void addFile(const string& pathForFile)
	{
		if (!fileIsExist(pathForFile)) {;
			throw exception("File is not exist!");
		}

		auto byteFrequency = getByteFrequencyFromFile(pathForFile);
		auto huffmanTree = HuffmanTree().getHuffmanTree(byteFrequency);
		auto huffmanCode = HuffmanCode().getHuffmanCode(huffmanTree);

		writeName(pathForFile);
		writeHuffmanTree(huffmanTree);
		writeText(huffmanTree, pathForFile);
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

	void writeName(const string& pathForFile)
	{
		string fileName = getFileNameFromPath(pathForFile);

		writeTextWithTagToFile(fileName, Tag::FileName);
	}

	void writeHuffmanTree(Node* huffmanTree)
	{
		string huffmanTreeInText = HuffmanTree().convertHuffmanTreeToString(huffmanTree);
		HuffmanTree().deleteHuffmanTree(huffmanTree);

		writeTextWithTagToFile(huffmanTreeInText, Tag::HuffmanTree);
	}

	void writeText(Node* huffmanTree, const string& pathForFile)
	{
		auto huffmanCode = HuffmanCode().getHuffmanCode(huffmanTree);
		string binaryText = getBinaryTextFromFileWithHuffmanCode(huffmanCode, pathForFile);
		string textFromFileModifiedWithHuffmanCode = convertBinarySequenceToSetBytes(binaryText);

		writeTextWithTagToFile(textFromFileModifiedWithHuffmanCode, Tag::Text);
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