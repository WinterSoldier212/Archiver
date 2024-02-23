#pragma once

#include <fstream>
#include <filesystem>

bool fileIsEmpty(const std::string& pathForFile)
{
	std::ifstream pFile(pathForFile);

	if (!fileIsExist(pathForFile))
	{
		throw std::exception("File is not exist!");
	}

	return pFile.peek() == std::ifstream::traits_type::eof();
}

bool fileIsExist(const std::string& pathForFile)
{
	std::ifstream file(pathForFile, std::ios::in);

	bool isExist = file.is_open();
	file.close();

	return isExist;
}

std::string getFileNameFromPath(std::string path)
{
	std::filesystem::path filePath(path);
	return filePath.filename().string();
}

