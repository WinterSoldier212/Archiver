#pragma once
#include <bitset>

using namespace std;

char convertBinarySequenceInByte(string binarySequence)
{
	bitset<8> byte(binarySequence);

	return static_cast<char>(byte.to_ulong());
}
string convertByteInBinarySequence(char byte)
{
	bitset<8> binarySequence(byte);
	
	return binarySequence.to_string();
}