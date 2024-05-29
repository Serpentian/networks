#pragma once

#include "utils.hpp"
#include "file_reader.hpp"

using namespace std;

/** \brief Huffman method encoder/decoder. */
class Huffman : public ICoder
{
	public:
		Huffman(const string& path)
		{
			map<char, int> m;
			FileReader::readSymbolsMap(path, m);

			for (auto& x : m)
				addNode(x.first, x.second);

			build();
		}

		void encode(const string& path, const string& pathTo) override
		{
			vector<char> encodedText;
			vector<char> text;
			FileReader::readAllBytes(path, text);

			string ser = to_string(_map.size()) + "\n";
			for (auto& x : _map)
			{
				ser += x.second->code;
				ser += ":";
				ser += x.first;
				ser += '\n';
			}

			FileReader::writeString(pathTo, ser);

			ofstream ofs(pathTo, ios::binary | ios::ate | ios::app);
			if (!ofs.good())
				throw runtime_error("Can't write to file: " + pathTo);

			// Codding.
			string buf = "";
			for (int i = 0; i < text.size(); ++i)
			{
				buf += _map[text[i]]->code;
				while (buf.length() >= 8)
				{
					char c = binaryToByte(buf.substr(0, 8));
					ofs.write(&c, sizeof(char));
					buf = buf.substr(8);
				}
			}

			if (buf.length() > 0)
			{
				char c = binaryToByte(buf.substr(0, 8));
				ofs.write(&c, sizeof(char));
			}

			ofs.close();
		}

		void decode(const string& path, const string& pathTo) override
		{
			map<string, char> m;
			vector<char> decodedText;

			ifstream ifs(path, ios::binary);
			if (!ifs.good())
				throw runtime_error("Can't read from file: " + path);

			ofstream ofs(pathTo, ios::binary);
			if (!ofs.good())
				throw runtime_error("Can't write to file: " + pathTo);

			// Deserializing map.
			string s;
			char c;
			ifs.read(&c, 1);
			while (c != '\n')
			{
				s += c;
				ifs.read(&c, sizeof(char));
			}
			int n = atoi(s.c_str());
			for (int j = 0; j < n; ++j)
			{
				string code;
				char co;

				s = "";
				ifs.read(&c, sizeof(char));
				while (c != '\n')
				{
					s += c;
					ifs.read(&c, sizeof(char));
				}

				if (s.size() == 0)
				{
					--j;
					continue;
				}

				if (s[s.length() - 1] == ':' && s[s.length() - 2] != ':')
				{
					co = '\n';
					code = s.substr(0, s.length() - 1);
				}
				else
				{
					co = s[s.length() - 1];
					code = s.substr(0, s.length() - 2);
				}

				m[code] = co;
			}

			// Decode.

			string code = "";
			string buf = "";
			int k = 0;
			while (!ifs.eof())
			{
				ifs.read(&c, sizeof(char));
				code.append(byteToBinary(c));

				while (code.length() != 0 && k < code.length())
				{
					buf += code[k];
					++k;
					auto it = m.find(buf);
					if (it != m.end())
					{
						ofs.write(&it.operator*().second, sizeof(char));
						buf = "";
						code = code.substr(k);
						k = 0;
					}
				}
			}

			ifs.close();
			ofs.close();
		}

	private:

		/** \brief Tree node. */
		struct HuffmanNode
		{
			int quantity;
			string code;

			HuffmanNode* left;
			HuffmanNode* right;

			HuffmanNode(int quantity) : quantity(quantity), left(nullptr), right(nullptr) {}
			HuffmanNode(int quantity, HuffmanNode* left, HuffmanNode* right) : quantity(quantity), left(left), right(right) {}

			~HuffmanNode()
			{
				if (left)
					delete left;
				if (right)
					delete right;
			}
		};

		/** \brief Tree node compare. */
		struct Compare
		{
			bool operator() (const HuffmanNode* l, const HuffmanNode* r)
			{
				return l->quantity > r->quantity;
			}
		};

		/** Traverses through the tree and make codes. */
		void traversal(HuffmanNode* root, string code)
		{
			root->code = code;

			if (root->left || root->right)
			{
				traversal(root->left, code + "0");
				traversal(root->right, code + "1");
			}
		}

		void build()
		{
			// Building a tree.
			while (_queue.size() > 1)
			{
				HuffmanNode* l = _queue.top();
				_queue.pop();

				HuffmanNode* r = _queue.top();
				_queue.pop();

				_queue.push(new HuffmanNode(l->quantity + r->quantity, l, r));
			}

			traversal(_queue.top(), "");
		}

		void addNode(char c, int quantity)
		{
			HuffmanNode* nNode = new HuffmanNode(quantity);
			_map[c] = nNode;
			_queue.push(nNode);
		}

		string getCode(char c)
		{
			return _map[c]->code;
		}

		void getCodeMap(map<char, string>& m) const
		{
			m = map<char, string>();
			for (auto& x : _map)
				m[x.first] = x.second->code;
		}

	private:

		map<char, HuffmanNode*> _map;
		priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> _queue;
};
