#pragma once
#include <bitset>

using namespace std;

class Convert {
public:
	static char binarySequenceToByte(string binarySequence)
	{
		bitset<8> byte(binarySequence);

		return static_cast<char>(byte.to_ulong());
	}
	static string byteToBinarySequence(char byte)
	{
		bitset<8> binarySequence(byte);
		return binarySequence.to_string();
	}
	static string binarySequenceToSetBytes(const string& binaryFileText)
	{
		char zeroBitCounter = '0';
		string byteInString = "",
			binaryTextInByte = "";

		for (const char& i : binaryFileText)
		{
			byteInString += i;
			if (byteInString.size() == 8)
			{
				binaryTextInByte += binarySequenceToByte(byteInString);
				byteInString = "";
			}
		}

		while (byteInString.size() != 0 && byteInString.size() != 8)
		{
			byteInString += '0';
			++zeroBitCounter;
		} 
		if (!byteInString.empty())
			binaryTextInByte += binarySequenceToByte(byteInString);

		return zeroBitCounter + binaryTextInByte;
	}
	static string stringToBinarySequence(string& text)
	{
		string binarySequence = string();
		for (char ch : text)
		{
			binarySequence += byteToBinarySequence(ch);
		}
		return binarySequence;
	}
} Convert;