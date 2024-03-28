#pragma once

#include <fstream>
#include <filesystem>

using namespace std;

bool fileIsEmpty(ifstream& pFile)
{
	return pFile.peek() == ifstream::traits_type::eof();
}

bool fileIsExist(const string& pathForFile)
{
	ifstream file;

	file.open(pathForFile);
	bool isExist = file.is_open();
	file.close();

	return isExist;
}

string getFullFileNameFromPath(string path)
{
	filesystem::path filePath(path);
	return filePath.filename().string();
}

string getFileNameFromPath(string path)
{
	filesystem::path filePath(path);
	return filePath.stem().string();
}

string getFileExtensionFromPath(string path)
{
	filesystem::path filePath(path);
	return filePath.extension().string();
}

string getCurrentDirectory()
{
	return filesystem::current_path().string();
}