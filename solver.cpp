// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include "Configuration.h"
#include "solver.h"
using std::forward_list;
using std::shared_ptr;
using std::vector;

shared_ptr<Configuration> solver(shared_ptr<Configuration> starting,
		shared_ptr<forward_list<shared_ptr<Configuration>>> log) {
	if(starting->is_goal())
		return starting;

	for(shared_ptr<Configuration> &next : starting->successors()) {
		shared_ptr<Configuration> future = solver(next, log);
		if(future) {
			// No need to log our starting config if it's empty
			if(log && starting->is_nonempty())
				log->push_front(starting);
			return future;
		}
	}

	return nullptr;
}
