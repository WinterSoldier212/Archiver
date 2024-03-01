#pragma once

#include <fstream>
#include <filesystem>

bool fileIsEmpty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

bool fileIsExist(const std::string& pathForFile)
{
	std::ifstream file;

	file.open(pathForFile);
	bool isExist = file.is_open();
	file.close();

	return isExist;
}

std::string getFileNameFromPath(std::string path)
{
	std::filesystem::path filePath(path);
	return filePath.filename().string();
}