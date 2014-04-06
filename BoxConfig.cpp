// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#include "BoxConfig.h"
#include <algorithm>
#include <iostream> // TODO remove
using std::shared_ptr;
using std::string;
using std::swap;
using std::to_string;
using std::vector;

BoxConfig::BoxConfig(unsigned num_devices,
			vector<vector<char>> &&edge_labels) :
		num_devices_(num_devices),
		edge_labels_(move(edge_labels)),
		board_(edge_labels_[RIGHT_EDGE].size()),
		repr_() {
	for(vector<bool> &column : board_)
		column.resize(edge_labels_[TOP_EDGE].size());

	unsigned added = 0;
	for(vector<bool> &row : board_)
		for(vector<bool>::size_type col = 0; col < row.size(); ++col) {
			if(added++ >= num_devices_)
				return;
			row[col] = true;
		}
}

vector<shared_ptr<Configuration>> BoxConfig::successors() const {
	vector<shared_ptr<Configuration>> next;
	shared_ptr<BoxConfig> most_recent;

	// TODO: THIS IS WRONG. It never moves the leading elements (see TODO below), but it also doesn't know when to stop yielding successors (i.e. move the same thing all over again). Instead, each level of the successors hierarchy should move a specific and characteristic one of the black box devices (e.g. the top level moves the first one to all possible positions, the second level the second, and so on).

	// Is there room to grow?
	if(!board_.back().back()) {
		do {
			next.push_back(most_recent =
					shared_ptr<BoxConfig>(most_recent ?
							new BoxConfig(*most_recent) : new BoxConfig(*this)))
							;
			std::cout << *most_recent << std::endl;
			std::cin.get();
			std::cout << std::boolalpha << most_recent->board_.back().back() << std::endl;
		}
		while(!most_recent->board_.back().back());
	}

	// TODO move the first pieces, too

	return next;
}

bool BoxConfig::is_nonempty() const {
	// TODO Do I need to implement this?  If not, perhaps provide a default?
	return true;
}

bool BoxConfig::is_goal() const {
	// TODO Write this!
	return is_valid();
}

BoxConfig::operator const string &() const {
	if(!repr_.size()) {
		repr_ += to_string(num_devices_);
		for(char label : edge_labels_[TOP_EDGE])
			repr_ += '\t' + represent_label(label);
		repr_ += '\n';

		for(vector<char>::size_type line = 0;
				line < edge_labels_[RIGHT_EDGE].size(); ++line) {
			repr_ += represent_label(edge_labels_
					[LEFT_EDGE][line]);
			for(bool entry : board_[line]) {
				repr_ += '\t';
				repr_ += entry ? REPR_DEVC : REPR_EMPT;
			}
			repr_ += '\t' + represent_label(edge_labels_[RIGHT_EDGE][line]) +
					'\n';
		}

		for(char label : edge_labels_[BOTTOM_EDGE])
			repr_ += '\t' + represent_label(label);
	}

	return repr_;
}

BoxConfig::BoxConfig(const BoxConfig &basis, unsigned nth_device) :
			Configuration(),
			num_devices_(basis.num_devices_),
			edge_labels_(basis.edge_labels_),
			board_(basis.board_),
			repr_() {
	if(nth_device == (unsigned)-1)
		nth_device = num_devices_ - 1;

	unsigned seen = 0;
	for(vector<bool> &row : board_)
		for(vector<bool>::size_type col = 0; col < row.size(); ++col) {
			// Here's one!
			if(row[col]) {
				// Just now counting the nth_device
				if(seen++ == nth_device)
					row[col] = false;
			}
			// Counted off the last device last time
			else if(seen == num_devices_) {
				row[col] = true;
				return;
			}
		}
}

bool BoxConfig::is_valid() const {
	vector<vector<bool>>::size_type r = -1;
	vector<vector<bool>>::size_type c = 0;
	vector<vector<bool>>::size_type dr = 1;
	vector<vector<bool>>::size_type dc = 0;
	for(vector<vector<char>>::size_type edge = 0; edge < edge_labels_.size();
			++edge) {
		vector<vector<bool>>::size_type &change = edge%2 ? r : c;
		for(; change < edge_labels_[edge].size(); ++change) {
			if(!trace_from_label(edge_labels_[edge][change], r, c, dr, dc))
				return false;
			std::cout << "row=" << r << " col=" << c << std::endl; // TODO remove
		}

		r = 0;
		c = 0;
		change = edge/2 ? -1 : edge_labels_[edge + 1].size();

		if(edge%2 == 0) {
			dr = -dr;
			dc = -dc;
		}
		swap(dr, dc);
	}

	return true;
}

bool BoxConfig::trace_from_label(char edge_label,
		vector<vector<bool>>::size_type r, vector<vector<bool>>::size_type c,
		vector<vector<bool>>::size_type dr,
		vector<vector<bool>>::size_type dc) const {
	vector<vector<bool>>::size_type next_r = r + dr;
	vector<vector<bool>>::size_type next_c = c + dc;

	// We're about to run into one of the devices
	if(next_r < board_.size() && next_c < board_[next_r].size() &&
			board_[next_r][next_c])
		// We expect a hit
		return edge_label == HIT_CHAR;

	// Is there a device diagonally in front of us on either side?
	bool left = false, right = false;
	if(dr) {
		if(next_r < board_.size()) {
			left = next_c + dr < board_.size() && board_[next_r][next_c + dr];
			right = next_c - dr < board_.size() && board_[next_r][next_c - dr];
		}
	}
	else {
		if(next_c < board_.size()) {
			left = next_r - dc < board_.size() && board_[next_r - dc][next_c];
			right = next_r + dc < board_.size() && board_[next_r + dc][next_c];
		}
	}

	// We're about to drive *between* two boxes: turn back now!
	if(left && right)
		// We expect a reflection
		return edge_label == REFL_CHAR;
	// There's a box off to port or starboard: evasive action!
	else if(left || right) {
		rotate_deltas(dr, dc, right);
		next_r = r + dr;
		next_c = c + dc;
	}

	// We're hitting another edge
	if(next_r >= board_.size())
		return edge_label != HIT_CHAR && edge_label != REFL_CHAR &&
				edge_label == edge_labels_[next_r == board_.size() ? BOTTOM_EDGE : TOP_EDGE]
				[next_c];
	else if(next_c >= board_.size())
		return edge_label != HIT_CHAR && edge_label != REFL_CHAR &&
				edge_label == edge_labels_
				[next_c == board_[next_r].size() ? RIGHT_EDGE : LEFT_EDGE]
				[next_r];

	return trace_from_label(edge_label, next_r, next_c, dr, dc);
}

void BoxConfig::rotate_deltas(vector<vector<bool>>::size_type &dr,
		vector<vector<bool>>::size_type &dc, bool ccw) {
	if((!ccw && dr) || (ccw && dc)) {
		dr = -dr;
		dc = -dc;
	}
	swap(dr, dc);
}

string BoxConfig::represent_label(char label) {
	if(label == HIT_CHAR || label == REFL_CHAR)
		return {label};
	else
		return to_string(label);
}
