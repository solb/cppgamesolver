// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include "Configuration.h"
using std::shared_ptr;
using std::vector;

// A backtracking solver
shared_ptr<Configuration> solver(shared_ptr<Configuration> starting) {
	if(starting->is_goal())
		return starting;

	for(shared_ptr<Configuration> &next : starting->successors()) {
		shared_ptr<Configuration> future = solver(next);
		if(future) return future;
	}

	return nullptr;
}
