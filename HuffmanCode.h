#pragma once

#include <fstream>
#include <vector>
#include <map>

std::vector<int>getWeightsSymbolsFromFile(const std::string& pathForFile)
{
	std::ifstream rfile(pathForFile, std::ios::in);
	std::vector<int> weight(0x100);

	char ch;
	while (!rfile.eof())
	{
		rfile >> ch;
		++weight[unsigned char(ch)];
	}
	rfile.close();

	return weight;
}

std::string getEncodedCharacterWeights(std::vector<int> weight)
{
	std::string encodedCharWeights = "";

	for (int i = 0; i < 0x100; ++i)
	{
		if (weight[i] == 0)
			continue;

		encodedCharWeights += '<';
		for (int j = 7; j >= 0; --j)
			if (weight[i] > pow(256, j))
				encodedCharWeights += weight[i] / pow(256, j);

		encodedCharWeights += char(i);
		encodedCharWeights += '>';
	}

	return encodedCharWeights;
}

std::string getBinaryText(std::map<char, std::string>& haffmanCode, const std::string& pathForFile)
{
	std::ifstream rfile(pathForFile, std::ios::in);
	std::string encodeText = "";

	char ch;
	while (!rfile.eof())
	{
		rfile >> ch;
		encodeText += haffmanCode[ch];
	}
	rfile.close();

	return encodeText;
}

std::string getEncodedText(std::map<char, std::string>& haffmanCode, const std::string& pathForFile)
{
	char ch = 0;
	int counter = 0;

	std::string str = "";
	std::string binaryFileText = getBinaryText(haffmanCode, pathForFile);

	for (char& i : binaryFileText)
	{
		ch |= (i == '1') ? (1 << counter) : 0;
		counter++;

		if (counter == 8)
		{
			str += ch;
			counter = 0;
			ch = 0;
		}
	}

	char countZero = '0';
	while (counter != 0)
	{
		countZero++;
		counter++;
		str += '0';

		if (counter == 8)
			counter = 0;
	}

	return countZero + str;
}