// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#include "BoxConfig.h"
#include <algorithm>
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
		nth_device_(0),
		last_placed_row_(0),
		last_placed_col_(0),
		repr_() {
	// Size the edges to fit the board dimensions
	for(vector<bool> &column : board_)
		column.resize(edge_labels_[TOP_EDGE].size());

	// Place devices in the first num_devices_ locations
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

	// This is a terminal configuration, with no devices left unmoved.
	if(nth_device_ == num_devices_)
		return next;

	// This is not the generic starting configuration
	if(nth_device_) {
		// The device whose position was fixed most recently in order to form
		// this configuration is in an illegal spot, defined as touching the
		// edge of the board in a place where it isn't surrounded by h s or r s
		if((last_placed_row_ == 0 &&
				!valid_edge_for_touching_device(TOP_EDGE, last_placed_col_)) ||
		   (last_placed_row_ == board_.size() - 1 &&
				!valid_edge_for_touching_device(BOTTOM_EDGE, last_placed_col_)) ||
		   (last_placed_col_ == 0 &&
				!valid_edge_for_touching_device(LEFT_EDGE, last_placed_row_)) ||
		   (last_placed_col_ == board_[last_placed_row_].size() - 1 &&
				!valid_edge_for_touching_device(RIGHT_EDGE, last_placed_row_)))
			// Prune this entire branch of the tree
			return next;
	}

	// Start by fixing the current nth_device_ in its present location
	shared_ptr<BoxConfig> most_recent = shared_ptr<BoxConfig>(new BoxConfig(*this));
	next.push_back(most_recent);

	// Also try it at every other possible position
	while(!most_recent->board_.back().back())
			next.push_back(most_recent = shared_ptr<BoxConfig>(
					new BoxConfig(*most_recent, nth_device_)));

	return next;
}

bool BoxConfig::is_nonempty() const {
	// Don't reveal the generic starting config (with no placements) in paths
	return nth_device_;
}

bool BoxConfig::is_goal() const {
	vector<vector<bool>>::size_type r = -1;
	vector<vector<bool>>::size_type c = 0;
	vector<vector<bool>>::size_type dr = 1;
	vector<vector<bool>>::size_type dc = 0;
	for(vector<vector<char>>::size_type edge = 0; edge < edge_labels_.size();
			++edge) {
		vector<vector<bool>>::size_type &change = edge%2 ? r : c;
		for(; change < edge_labels_[edge].size(); ++change)
			if(!trace_from_label(edge_labels_[edge][change], r, c, dr, dc))
				return false;

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
			nth_device_(nth_device + 1),
			last_placed_row_(0),
			last_placed_col_(0),
			repr_() {
	if(nth_device != (unsigned)-1) {
		// Move the *old* nth_device to the position after the last device
		unsigned seen = 0;
		for(vector<vector<bool>>::size_type row = 0; row < board_.size(); ++row)
			for(vector<bool>::size_type col = 0; col < board_[row].size(); ++col) {
				// Here's one!
				if(board_[row][col]) {
					// Send the *old* nth_device to the end to get *this* config
					if(seen++ == nth_device)
						board_[row][col] = false;
					// The current *new* nth_device is now the old one
					else if(seen - 1 == nth_device_) {
						last_placed_row_ = row;
						last_placed_col_ = col;
					}
				}
				// Counted off the last device last time, so this is blank space
				else if(seen == num_devices_) {
					// Move the redacted device to this position
					board_[row][col] = true;
					return;
				}
			}
	}
	else {
		// Just leave the *old* nth_device in place
		nth_device_ = basis.nth_device_ + 1;
		// Find out where the *last* device was placed
		for(vector<vector<bool>>::size_type row = basis.last_placed_row_;
				row < board_.size(); ++row)
			for(vector<bool>::size_type col = basis.last_placed_col_;
					col < board_[row].size(); ++col)
				if(board_[row][col]) {
					last_placed_row_ = row;
					last_placed_col_ = col;
					return;
				}
	}
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
	if(next_r >= board_.size()) {
		if(r >= board_.size())
			return edge_label == REFL_CHAR;
		return edge_label != HIT_CHAR && edge_label != REFL_CHAR &&
				edge_label == edge_labels_[next_r == board_.size() ? BOTTOM_EDGE : TOP_EDGE]
				[next_c];
	}
	else if(next_c >= board_.size()) {
		if(c >= board_.size())
			return edge_label == REFL_CHAR;
		return edge_label != HIT_CHAR && edge_label != REFL_CHAR &&
				edge_label == edge_labels_
				[next_c == board_[next_r].size() ? RIGHT_EDGE : LEFT_EDGE]
				[next_r];
	}

	// Keep following the path until we actually hit an edge or we reflect
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
