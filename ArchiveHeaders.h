#pragma once

#include <fstream>
#include <exception>
#include <string>

#include "HuffmanTree.h"
#include "HuffmanCode.h"
#include "FileFunctions.h"
#include "Convert.h"

ofstream logFile(getCurrentDirectory()+"log.txt");

class Archive
{
protected:
	fstream archive;
	string archiveName;

public:
	Archive() = default;

	void Create(string pathForNewArhcive)
	{
		Close();

		archive.open(pathForNewArhcive, ios::out);
	}
	void Open(string pathForArhcive)
	{
		if (!fileIsExist(pathForArhcive))
		{
			throw ExceptionFileNotExist(pathForArhcive);
		}
		Close();

		archive.open(pathForArhcive, ios::app);
	}
	void Close()
	{
		if (IsOpen())
		{
			archive.close();
		}
	}

	bool IsOpen()
	{
		return archive.is_open();
	}
	string GetName()
	{
		return archiveName;
	}
};

namespace Tag
{
	const char 
		FileName = 'n',
		HuffmanTree = 'h',
		Text = 't';
}

class ExceptionFileNotExist : public exception
{
	string pathForFile;
public:
	ExceptionFileNotExist(string pathForFile_)
		: pathForFile(pathForFile_), exception("File has not been found! ")
	{}

	string GetFileName()
	{
		return pathForFile;
	}
};

class ExceptionArchiveNotOpen : public exception
{
	string archiveName_;
public:
	ExceptionArchiveNotOpen(string archiveName)
		: archiveName_(archiveName)
	{}

	string GetArchiveName()
	{
		return archiveName_;
	}
};

class ExceptionArchiveEof : public exception
{
	string archiveName_;
public:
	ExceptionArchiveEof(string archiveName)
		: archiveName_(archiveName)
	{}

	string GetArchiveName()
	{
		return archiveName_;
	}
};