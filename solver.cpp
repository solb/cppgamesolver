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
