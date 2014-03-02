// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include <iostream>
#include "solver.h"
#include "WheelConfig.h"
using std::cin;
using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::vector;

int main() {
	unsigned triads_count;
	cin >> triads_count;

	vector<unsigned> connector_sums(triads_count);
	for(unsigned &sum : connector_sums)
		cin >> sum;

	vector<unsigned> starting_config;
	do {
		size_t ind;
		unsigned val;
		cin >> ind;
		cin >> val;
		if(!cin.eof()) {
			if(ind >= starting_config.size())
				starting_config.resize(ind+1);
			starting_config[ind] = val;
		}
	}
	while(!cin.eof());

	shared_ptr<Configuration> puzzle_state =
			make_shared<WheelConfig>(triads_count,
					move(connector_sums), move(starting_config));
	puzzle_state = solver(puzzle_state);

	cout << (puzzle_state ? *puzzle_state : "No solution!") << endl;
}
