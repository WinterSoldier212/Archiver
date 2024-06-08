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
		archive.open(archiveName, ios::out | ios::binary);
		archive << "As the author of this application, which was made as part of the course work, I want to sincerely thank my loved ones and especially Danil! He is a very courageous man who had the strength to tolerate me and listen to my crazy ideas and requests throughout this whole job and my whole life in general! If you hadn't been there, brother, I wouldn't have achieved all this!!! The first line in each of the archives will be dedicated to you, Brother!\n";
		archive << "I also want to express my gratitude to Ksenia Mikhailovna Spiridonova, a wonderful mathematics teacher who became the head of my course work and gave me the opportunity to freely choose a topic! Thanks! Thanks to everyone who was there for me and supported me!!!";
	}

	virtual void Open(string pathForArhcive) override
	{
		if (!fileIsExist(pathForArhcive))
		{
			throw ExceptionFileNotExist(pathForArhcive);
		}
		Close();

		archiveName = getFreeFileNameInDirectory(pathForArhcive, ".alzip");
		archive.open(archiveName, ios::app | ios::binary);
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

		vector<size_t>&& byteFrequency = getByteFrequencyFromFile(pathForFile);

		auto&& huffmanTree = HuffmanTree::getHuffmanTree(byteFrequency);
		auto&& huffmanCode = HuffmanCode::getHuffmanCode(huffmanTree);

		string&& fileName = getFullFileNameFromPath(pathForFile);
		string&& huffmanTreeInText = HuffmanTree::convertHuffmanTreeToString(huffmanTree);
		string&& binaryText = getBinaryTextFromFileWithHuffmanCode(huffmanCode, pathForFile);
		string&& textFromFileModifiedWithHuffmanCode = Convert::binarySequenceToSetBytes(binaryText);

		HuffmanTree::deleteHuffmanTree(huffmanTree);

		writeTextWithTagToFile(fileName, Tag::FileName);
		writeTextWithTagToFile(huffmanTreeInText, Tag::HuffmanTree);
		writeTextWithTagToFile(textFromFileModifiedWithHuffmanCode, Tag::Text);
	}

private:
	vector<size_t> getByteFrequencyFromFile(const string& pathForFile)
	{
		ifstream rfile(pathForFile, ios::in | ios::binary);
		vector<size_t> byteFrequency(0x100);

		char ch;
		while (rfile.get(ch))
		{
			++byteFrequency[unsigned char(ch)];
		}
		rfile.close();

		return byteFrequency;
	}

	void writeTextWithTagToFile(string& text, char tag)
	{
		archive.put('\n').put('<').put(tag).put('>').put('\n');
		archive << text << '\n';
		archive.put('<').put(tag).put('>');
	}

	string getBinaryTextFromFileWithHuffmanCode(
		map<unsigned char, string>& huffmanCode,
		const string& pathForFile
	) {
		ifstream rfile(pathForFile, ios::in | ios::binary);

		char ch;
		string encodeText = string();
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