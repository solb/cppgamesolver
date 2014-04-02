// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#include "BoxConfig.h"
#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
#include <vector>
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::istream;
using std::ifstream;
using std::numeric_limits;
using std::streamsize;
using std::unique_ptr;
using std::vector;

// Indicates that the input should be read from stdin instead of a file.
const char *const SYMB_STDIN = "-";

int main(int argc, char *argv[]) {
	// Process command-line argument(s)
	bool pathMode = false;
	if(argc > 2) {
		if(argc == 3 && !strcmp(argv[1], "path"))
			pathMode = true;
		else {
			cerr << "Usage: blackbox {path} <input>" << endl <<
					"{path} is the (optional) literal string \"path\"" <<
					endl << "<input> is the path to the input file, or `" <<
					SYMB_STDIN << "' to read from standard input" << endl;
			return 1;
		}
	}

	// Handle automatic destruction of the file descriptor we create, if any
	unique_ptr<ifstream> fdesc;

	// Pointer to the input stream we'll actually be using
	istream *ins;
	if(!strcmp(argv[argc - 1], SYMB_STDIN))
		ins = &cin;
	else {
		fdesc.reset(new ifstream(argv[argc - 1]));
		ins = fdesc.get();
		if(!fdesc->is_open()) {
			cerr << "Failed to open input file: " << argv[argc - 1] << endl;
			return 1;
		}
	}

	unsigned num_boxes, side_len;
	*ins >> num_boxes;
	*ins >> side_len;
	if(!num_boxes || !side_len) {
		cerr << "The number of boxes and side length must both be nonzero" <<
				endl;
		return 2;
	}

	// Collect the side labels: top, right, bottom, left
	vector<vector<char>> edges(4);
	for(vector<vector<char>>::size_type edge = 0; edge < 4; ++edge)
		for(unsigned count = 0; count < side_len; ++count) {
			unsigned edge;
			*ins >> edge;
			edges[edge].push_back(edge);
		}

	for(vector<unsigned> &group : edges) {
		for(unsigned element : group)
			cout << element << ' ';
		cout << endl;
	}

	// TODO: we'll need constants for the character codes of 'h' and 'r'
}
