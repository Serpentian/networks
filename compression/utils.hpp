#pragma once

#include <string>

using namespace std;

char binaryToByte(const string& bits)
{
	return static_cast<char>(stoi(bits, nullptr, 2));
}

string byteToBinary(unsigned short c)
{
	string r;
	for (int i = 0; i < 8; ++i) {
		r = (c % 2 == 0 ? "0" : "1") + r;
		c /= 2;
	}
	return r;
}
