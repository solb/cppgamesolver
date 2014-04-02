// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#include "BoxConfig.h"
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

BoxConfig::BoxConfig(unsigned num_devices,
			vector<vector<char>> &&edge_labels) :
		num_devices_(num_devices),
		edge_labels_(move(edge_labels)),
		board_(edge_labels_[RIGHT_EDGE].size()),
		repr_() {
	for(vector<bool> &column : board_)
		column.resize(edge_labels[TOP_EDGE].size());
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
		for(char label : edge_labels_[TOP_EDGE]) {
			repr_ += '\t' + represent_label(label);
		}
	}

	return repr_;
}

string BoxConfig::represent_label(char label) {
	if(label == HIT_CHAR || label == REFL_CHAR)
		return {label};
	else
		return to_string(label);
}
