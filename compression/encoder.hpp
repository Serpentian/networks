#pragma once

#include <vector>
#include <queue>
#include <list>

#include "icoder.hpp"
#include "haff.hpp"
#include "shan.hpp"

using namespace std;

/** \brief Encoder/decoder for all type of files. */
class Encoder
{

	public:
		/** \brief Encodes file with path to file with pathTo by method. */
		void encode(const string& method, const string& path, const string& pathTo)
		{
			ICoder* c;

			if (method == "haff")
				c = new Huffman(path);
			else if (method == "shan")
				c = new ShannonFano(path);
			else
				throw logic_error("No supported method to encode: " + method);

			c->encode(path, pathTo);
		}

		/** \brief Decodes file with path to file with pathTo by method. */
		void decode(const string& method, const string& path, const string& pathTo)
		{
			ICoder* c;

			if (method == "haff")
				c = new Huffman(path);
			else if (method == "shan")
				c = new ShannonFano(path);
			else
				throw logic_error("No supported method to decode: " + method);

			c->decode(path, pathTo);
		}
};
