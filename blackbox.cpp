// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#include "BoxConfig.h"
#include "solver.h"
#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::forward_list;
using std::istream;
using std::ifstream;
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

// Indicates that the input should be read from stdin instead of a file.
const char *const SYMB_STDIN = "-";

int main(int argc, char *argv[]) {
	// Process command-line argument(s)
	bool pathMode = false;
	if(argc != 2) {
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
			while(isspace(ins->peek())) ins->get();
			if(isdigit(ins->peek())) {
				short label;
				*ins >> label;
				edges[edge].push_back(label);
			}
			else { // It's a character
				char label;
				*ins >> label;
				edges[edge].push_back(label);
			}
		}

	// Space for our solution and descent path
	shared_ptr<Configuration> puzzle_state =
			make_shared<BoxConfig>(num_boxes, move(edges));
	shared_ptr<forward_list<shared_ptr<Configuration>>> puzzle_hist =
			make_shared<forward_list<shared_ptr<Configuration>>>();

	// Solve the puzzle
	puzzle_state = pathMode ?
			solver(puzzle_state, puzzle_hist) : solver(puzzle_state);

	// Print out the solution path if we generated one
	for(shared_ptr<Configuration> snapshot : *puzzle_hist)
		cout << *snapshot << endl << endl;

	cout << (puzzle_state ? *puzzle_state : "No solution!") << endl;
}
