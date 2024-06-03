#pragma once
#include <bitset>

using namespace std;

class Convert {
public:
	char binarySequenceToByte(string binarySequence)
	{
		bitset<8> byte(binarySequence);

		return static_cast<char>(byte.to_ulong());
	}

	string byteToBinarySequence(char byte)
	{
		bitset<8> binarySequence(byte);
		return binarySequence.to_string();
	}

	string binarySequenceToSetBytes(const string& binaryFileText)
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

		while (byteInString.size() != 8 && byteInString.size() != 0)
		{
			byteInString += '0';
			++zeroBitCounter;
		}
		binaryTextInByte += binarySequenceToByte(byteInString);

		return zeroBitCounter + binaryTextInByte;
	}
} Convert;