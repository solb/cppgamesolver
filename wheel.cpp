/*
 * Copyright (C) 2014 Sol Boucher
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include <cstring>
#include <iostream>
#include "solver.h"
#include "WheelConfig.h"
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::forward_list;
using std::make_shared;
using std::shared_ptr;
using std::vector;

int main(int argc, char *argv[]) {
	// Process command-line argument(s)
	bool pathMode = false;
	if(argc > 1) {
		if(argc == 2 && !strcmp(argv[1], "path"))
			pathMode = true;
		else {
			cerr << "Usage: wheel {path}" << endl;
			return 1;
		}
	}

	unsigned triads_count;
	cin >> triads_count;

	vector<unsigned> connector_sums(triads_count);
	for(unsigned &sum : connector_sums)
		cin >> sum;

	// Collect optional input
	vector<unsigned> starting_config;
	do {
		size_t ind;
		unsigned val;
		cin >> ind;
		cin >> val;
		if(cin) {
			if(ind >= starting_config.size())
				starting_config.resize(ind+1);
			starting_config[ind] = val;
		}
	}
	while(!cin.eof());

	// Space for our solution and descent path
	shared_ptr<Configuration> puzzle_state =
			make_shared<WheelConfig>(triads_count,
					move(connector_sums), move(starting_config));
	shared_ptr<forward_list<shared_ptr<Configuration>>> puzzle_hist =
			make_shared<forward_list<shared_ptr<Configuration>>>();

	// Solve the puzzle
	puzzle_state = pathMode ?
			solver(puzzle_state, puzzle_hist) : solver(puzzle_state);

	// Print out solution path if we generated one
	for(shared_ptr<Configuration> snapshot : *puzzle_hist)
		cout << *snapshot << endl;
	// Is that your final answer?
	cout << (puzzle_state ? *puzzle_state : "No solution!") << endl;
}
