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

#include <algorithm>
#include <cassert>
#include "WheelConfig.h"
using std::find;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

WheelConfig::WheelConfig(unsigned num_triads, vector<unsigned>&& gap_sums,
				vector<unsigned> &&config) :
		num_triads_(num_triads),
		num_spaces_(num_triads_*3),
		each_sum_(num_spaces_*(num_spaces_+1)/(2*num_triads_)),
		gap_sums_(move(gap_sums)),
		// Everyone's available unless he changes his profile status
		available_spaces_(num_spaces_, true),
		config_(config),
		// The first zero (or the end of the list) is our first target
		to_place_(find(config.begin(), config.end(), 0)-config.begin()),
		repr_() {
	// Mark off all nonzero numbers
	for(unsigned used : config) {
		if(used)
			available_spaces_[used-1] = false;
	}
}

// Precondition: next_placed in range, set in available_spaces_, not in config_
WheelConfig::WheelConfig(const WheelConfig &predecessor, unsigned next_placed) :
		num_triads_(predecessor.num_triads_),
		num_spaces_(predecessor.num_spaces_),
		each_sum_(predecessor.each_sum_),
		gap_sums_(predecessor.gap_sums_),
		available_spaces_(predecessor.available_spaces_),
		config_(predecessor.config_),
		// Find the first zero once again
		to_place_(find(predecessor.config_.begin()+predecessor.to_place_+1,
				predecessor.config_.end(), 0)-predecessor.config_.begin()),
		repr_() {
	// Make room at the end of the list if there are no zeros
	if(predecessor.to_place_ >= config_.size()) {
		config_.resize(predecessor.to_place_+1);
		to_place_ = config_.size();
	}
	// Place next_placed in the appointed spot
	config_[predecessor.to_place_] = next_placed;
	assert(next_placed > 0);
	assert(next_placed <= num_spaces_);
	assert(available_spaces_[next_placed-1]);
	available_spaces_[next_placed-1] = false;
}

vector<shared_ptr<Configuration>> WheelConfig::successors() const {
	vector<shared_ptr<Configuration>> descendents;

	switch(to_place_%3) {
		case 0: // Placing the first member of a new triad
			if(!to_place_) // First one placed
				for(unsigned possible = 1; possible <= num_spaces_; ++possible) {
					// If anything after this is filled in, it might be taken
					if(available_spaces_[possible-1])
						descendents.push_back(shared_ptr<Configuration>(
								new WheelConfig(*this, possible)));
				}
			else {
				// A specific value must be tied to the previous triad's last
				unsigned needed = gap_sums_[to_place_/3-1]-
						config_[to_place_-1];
				if(needed > 0 && needed <= num_spaces_ &&
						available_spaces_[needed-1])
					descendents.push_back(shared_ptr<Configuration>(
							new WheelConfig(*this, needed)));
			}
			break;
		case 1: { // Placing the middle member of a triad
			// The next two values we select must add up to this
			unsigned sum = each_sum_-config_[to_place_-1];
			// The biggest one we could place is this
			unsigned max = sum > num_spaces_ ? num_spaces_ : sum-1;
			// Knowing that, let's try the smallest one first
			for(unsigned possible = sum-max; possible <= max; ++possible) {
				// But we don't want to place the *same* value twice!
				if(possible*2 == sum)
					continue;
				// Can we fit it, and will it work with whatever already follows
				if(available_spaces_[possible-1] &&
						(available_spaces_[sum-possible-1] ||
								(to_place_+1 < config_.size() &&
										config_[to_place_+1] == sum-possible)))
					descendents.push_back(shared_ptr<Configuration>(
							new WheelConfig(*this, possible)));
			}
			break;
		}
		default: // Placing the last member to complete a triad
			// We already confirmed that this is going to fit
			descendents.push_back(shared_ptr<Configuration>(
					new WheelConfig(*this, each_sum_-
					config_[to_place_-2]-config_[to_place_-1])));
	}

	return descendents;
}

bool WheelConfig::is_nonempty() const {
	return config_.size();
}

bool WheelConfig::is_goal() const {
	// It's gotta be full and contain no unknowns
	return config_.size() == num_spaces_ &&
			!count(config_.begin(), config_.end(), 0) &&
			config_.front()+config_.back() == gap_sums_.back();
}

WheelConfig::operator const string &() const {
	if(!repr_.size()) {
		for(unsigned index = 0; index < num_spaces_; ++index) {
			if(index%3 == 0)
				repr_ += (index ? " - " : "");
			else
				repr_ += '.';

			repr_ += index < config_.size() ? to_string(config_[index]) : "0";
		}
	}

	return repr_;
}
