#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>

#include "encoder.hpp"

using namespace std;

const int METHOD_COUNT = 2;
const string METHOD_NAMES[] = { "haff", "shan" };

void print_usage() {
	cout << "Usage: program_name --source <path> --output <path>\n";
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		print_usage();
		return 1;
	}

	string source;
	string output;

	static struct option long_options[] = {
		{"source", required_argument, 0, 's'},
		{"output", required_argument, 0, 'o'},
		{0, 0, 0, 0}
	};

	int opt;
	int option_index = 0;
	while ((opt = getopt_long(argc, argv, "s:o:", long_options, &option_index)) != -1) {
		switch (opt) {
			case 's':
				source = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case '?':
				print_usage();
				return 1;
			default:
				print_usage();
				return 1;
		}
	}

	Encoder e;
	for (int m = 0; m < METHOD_COUNT; ++m) {
		cout << "File: " << source << " method: " << METHOD_NAMES[m] << "\n";
		e.encode(METHOD_NAMES[m], source, output + "." + METHOD_NAMES[m]);
		e.decode(METHOD_NAMES[m], output + "." + METHOD_NAMES[m],
				output + ".un" + METHOD_NAMES[m]);
		cout << "Initial/Compressed: " << FileReader::compressRatio(source,
				output + "." + METHOD_NAMES[m]) << "\n\n";
	}

	return 0;
}
