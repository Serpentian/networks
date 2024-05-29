#pragma once

#include "utils.hpp"
#include "file_reader.hpp"

using namespace std;

/** \brief ShannonFano method encoder/decoder. */
class ShannonFano : public ICoder
{
	public:

		ShannonFano(const string& path)
		{
			map<char, int> m;
			FileReader::readSymbolsMap(path, m);

			for (auto& x : m)
				addNode(x.first, x.second);

			build();
		}

	private:

		void build()
		{
			fano(0, _list.size() - 1);
			for (auto& x : _list)
				_map[x.first.second] = x.second;
		}

		void fano(int l, int r)
		{
			if (l >= r)
				return;

			int m = median(l, r);
			fano(l, m);
			fano(m + 1, r);
		}

		int median(int l, int r)
		{
			int sl = 0;
			for (int i = l; i < r; ++i)
				sl += _list[i].first.first;

			int sr = _list[r].first.first;
			int m = r;
			int d;
			do {
				_list[m--].second += "1";
				d = sl - sr;
				sl -= _list[m].first.first;
				sr += _list[m].first.first;
			} while (abs(sl - sr) <= d);

			for (int i = l; i <= m; ++i)
				_list[i].second += "0";

			return m;
		}

		void addNode(char c, int quantity)
		{
			_list.push_back(pair< pair<int, char>, string >(pair<int, char>(quantity, c), ""));
		}

	private:

		void encode(const string& path, const string& pathTo) override
		{
			vector<char> encodedText;

			vector<char> text;
			FileReader::readAllBytes(path, text);

			// Writing map.
			string ser = to_string(_map.size()) + "\n";
			for (auto& x : _map)
			{
				ser += x.second;
				ser += ":";
				ser += x.first;
				ser += '\n';
			}

			FileReader::writeString(pathTo, ser);

			ofstream ofs(pathTo, ios::binary | ios::ate | ios::app);
			if (!ofs.good())
				throw runtime_error("Can't write to file: " + pathTo);

			// Encoding.
			string buf = "";
			for (int i = 0; i < text.size(); ++i)
			{
				buf += _map[text[i]];
				while (buf.length() >= 8)
				{
					char c = binaryToByte(buf.substr(0, 8));
					ofs.write(&c, 1);
					buf = buf.substr(8);
				}
			}

			if (buf.length() > 0)
			{
				char c = binaryToByte(buf.substr(0, 8));
				ofs.write(&c, 1);
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
				ifs.read(&c, 1);
			}
			int n = atoi(s.c_str());
			for (int j = 0; j < n; ++j)
			{
				string code;
				char co;

				s = "";
				ifs.read(&c, 1);
				while (c != '\n')
				{
					s += c;
					ifs.read(&c, 1);
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
				ifs.read(&c, 1);
				code.append(byteToBinary(c));

				while (code.length() != 0 && k < code.length())
				{
					buf += code[k];
					++k;
					auto it = m.find(buf);
					if (it != m.end())
					{
						ofs.write(&it.operator*().second, 1);
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
		map<char, string> _map;
		vector< pair< pair<int, char>, string> > _list;
};
