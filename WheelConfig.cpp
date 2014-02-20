// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include <cassert>
#include <sstream>
#include "WheelConfig.h"
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

WheelConfig::WheelConfig(unsigned num_triads, vector<unsigned>&& gap_sums) :
		num_triads_(num_triads),
		num_spaces_(num_triads_*3),
		each_sum_(num_spaces_*(num_spaces_+1)/(2*num_triads_)),
		gap_sums_(move(gap_sums)),
		available_spaces_(num_spaces_, true),
		config_() {}

// Precondition: next_placed in range, set in available_spaces_, not in config_
WheelConfig::WheelConfig(const WheelConfig &predecessor, unsigned next_placed) :
		num_triads_(predecessor.num_triads_),
		num_spaces_(predecessor.num_spaces_),
		each_sum_(predecessor.each_sum_),
		gap_sums_(predecessor.gap_sums_),
		available_spaces_(predecessor.available_spaces_),
		config_(predecessor.config_) {
	config_.push_back(next_placed);
	assert(next_placed > 0);
	assert(next_placed <= num_spaces_);
	assert(available_spaces_[next_placed-1]);
	available_spaces_[next_placed-1] = false;
}

vector<shared_ptr<Configuration>> WheelConfig::successors() const {
	vector<shared_ptr<Configuration>> descendents;

	switch(config_.size()%3) {
		case 0: // Placing the first member of a new triad
			if(!config_.size()) // First one placed
				for(unsigned possible = 1; possible <= num_spaces_; ++possible)
					// We haven't placed anything, so it must not yet be used
					descendents.push_back(shared_ptr<Configuration>(
							new WheelConfig(*this, possible)));
			else {
				// A specific value must be tied to the previous triad's last
				unsigned needed = gap_sums_[config_.size()/3-1]-config_.back();
				if(needed > 0 && needed <= num_spaces_ &&
						available_spaces_[needed-1])
					descendents.push_back(shared_ptr<Configuration>(
							new WheelConfig(*this, needed)));
			}
			break;
		case 1: // Placing the middle member of a triad
			// The values of the next two are interdependent to make the sum
			for(unsigned possible = each_sum_-num_spaces_-config_.back();
					possible < each_sum_-config_.back()-possible; ++possible) {
				if(available_spaces_[possible-1] &&
						available_spaces_[each_sum_-config_.back()-possible-1])
					descendents.push_back(shared_ptr<Configuration>(
							new WheelConfig(*this, possible)));
			}
			break;
		default: // Placing the last member to complete a triad
			// We already confirmed that this is going to fit
			descendents.push_back(shared_ptr<Configuration>(
					new WheelConfig(*this, each_sum_-
					config_[config_.size()-2]-config_[config_.size()-1])));
	}

	return descendents;
}

bool WheelConfig::is_goal() const {
	return config_.size() == num_spaces_;
}

string WheelConfig::str() const {
	stringstream holding;

	for(unsigned index = 0; index < config_.size(); ++index) {
		if(index%3 == 0)
			holding << (index ? " - " : "");
		else
			holding << '.';

		holding << config_[index];
	}

	return holding.str();
}
