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
}

vector<shared_ptr<Configuration>> BoxConfig::successors() const {
	// TODO Write this!
	return vector<shared_ptr<Configuration>>();
}

bool BoxConfig::is_nonempty() const {
	// TODO Do I need to implement this?  If not, perhaps provide a default?
	return false;
}

bool BoxConfig::is_goal() const {
	// TODO Write this!
	return false;
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

bool BoxConfig::is_valid() const {
	vector<vector<bool>>::size_type r = -1;
	vector<vector<bool>>::size_type c = 0;
	vector<vector<bool>>::size_type dr = 1;
	vector<vector<bool>>::size_type dc = 0;
	for(vector<vector<char>>::size_type edge = 0; edge < edge_labels_.size();
			++edge) {
		vector<vector<bool>>::size_type &change = edge%2 ? r : c;
		for(; change < edge_labels_[edge].size(); ++change)
			// TODO: actually trace from this "location"
			std::cout << "row=" << r << " col=" << c << std::endl;

		r = 0;
		c = 0;
		change = edge/2 ? -1 : edge_labels_[edge + 1].size();

		if(edge%2) {
			dr = -dr;
			dc = -dc;
		}
		swap(dr, dc);
	}

	// TODO return whether it was actually valid
}

string BoxConfig::represent_label(char label) {
	if(label == HIT_CHAR || label == REFL_CHAR)
		return {label};
	else
		return to_string(label);
}
