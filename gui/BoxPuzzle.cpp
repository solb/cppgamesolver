// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#include "BoxPuzzle.h"
#include "../solver.h"
#include <fstream>
#include <QChar>
#include <QCheckBox>
#include <QErrorMessage>
#include <QLabel>
#include <string>
#include <utility>
#include <vector>

using Qt::CheckState;
using std::dynamic_pointer_cast;
using std::get;
using std::ifstream;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::tuple;
using std::vector;

shared_ptr<BoxPuzzle> BoxPuzzle::createFromFile(const char *filename) {
	ifstream ins(filename);

	// Ensure we can read the file
	if(!ins.is_open()) {
		QErrorMessage::qtHandler()->showMessage("Failed to open input file: ");
		return nullptr;
	}

	unsigned num_boxes, side_len;
	ins >> num_boxes;
	ins >> side_len;
	if(!num_boxes || !side_len) {
		QErrorMessage::qtHandler()->showMessage(
				"The number of boxes and side length must both be nonzero");
		return nullptr;
	}

	// Collect the side labels: top, right, bottom, left
	vector<vector<char>> edges(4);
	for(vector<vector<char>>::size_type edge = 0; edge < 4; ++edge)
		for(unsigned count = 0; count < side_len; ++count) {
			while(isspace(ins.peek())) ins.get();
			if(isdigit(ins.peek())) {
				short label;
				ins >> label;
				edges[edge].push_back(label);
			}
			else { // It's a character
				char label;
				ins >> label;
				edges[edge].push_back(label);
			}
		}

	return shared_ptr<BoxPuzzle>(new BoxPuzzle(move(edges), num_boxes));
}

BoxPuzzle::BoxPuzzle(vector<vector<char>> &&edge_labels, unsigned num_boxes,
		QWidget *parent) :
			QGridLayout(parent),
			edge_labels_(edge_labels[BoxConfig::LEFT_EDGE].size(),
					vector<char>(edge_labels[BoxConfig::TOP_EDGE].size())),
			board_(edge_labels[BoxConfig::LEFT_EDGE].size(),
					vector<QCheckBox *>
							(edge_labels[BoxConfig::TOP_EDGE].size())),
			tried_to_solve_(false),
			solution_(nullptr),
			NOTHING_TO_SEE_HERE_(board_.size(), board_[0].size()) {
	solution_ = make_shared<BoxConfig>(num_boxes, move(edge_labels));

	// Place the edge labels
	for(vector<char>::size_type edge = 0; edge < edge_labels_.size(); ++edge)
		for(unsigned index = 0; index < edge_labels_[edge].size(); ++index) {
			QLabel *label = new QLabel(QString(
					BoxConfig::represent_label(edge_labels_[edge][index]).
							c_str()));
			if(edge % 2)
				addWidget(label, index + 1,
						edge == BoxConfig::LEFT_EDGE ? 0 : board_[0].size() + 1)
								;
			else
				addWidget(label, edge ==
						BoxConfig::TOP_EDGE ? 0 : board_.size() + 1, index + 1);
		}

	// Place the "black box" devices
	for(vector<bool>::size_type r = 0; r < board_.size(); ++r)
		for(vector<bool>::size_type c = 0; c < board_[r].size(); ++c) {
			board_[r][c] = new QCheckBox();
			addWidget(board_[r][c], r + 1, c + 1);
		}
}

bool BoxPuzzle::has_solution() const {
	if(!tried_to_solve_)
		solution_ = dynamic_pointer_cast<BoxConfig>(solver(solution_));
	return (bool)solution_;
}

bool BoxPuzzle::is_on_the_right_track() const {
	if(!has_solution())
		return false;

	// Ensure they haven't done anything the solution doesn't
	// TODO This approach doesn't work for boards with multiple solutions
	return first_distinguishing_coordinate(&BoxPuzzle::invalid_placement) == NOTHING_TO_SEE_HERE_;

	return true;
}

bool BoxPuzzle::advance_game() {
	if(!has_solution())
		return false;

	tuple<rindex_t, cindex_t> where_to =
			first_distinguishing_coordinate(&BoxPuzzle::invalid_placement);
	CheckState new_state = Qt::Unchecked;

	if(where_to == NOTHING_TO_SEE_HERE_) {
		where_to = first_distinguishing_coordinate(&BoxPuzzle::missing_placement);
		new_state = Qt::Checked;
	}
	board_[get<0>(where_to)][get<1>(where_to)]->setCheckState(new_state);

	return true;
}

tuple<rindex_t, cindex_t> BoxPuzzle::first_distinguishing_coordinate(
		bool (BoxPuzzle::*decider)(rindex_t, cindex_t) const) const {
	for(rindex_t r = 0; r < board_.size(); ++r)
		for(cindex_t c = 0; c < board_[r].size(); ++c)
			if((this->*decider)(r, c))
				return tuple<rindex_t, cindex_t>(r, c);

	return NOTHING_TO_SEE_HERE_;
}

bool BoxPuzzle::invalid_placement(rindex_t row, cindex_t col) const {
	return this->board_[row][col]->isChecked() && !solution_->board(row)[col];
}

bool BoxPuzzle::missing_placement(rindex_t row, cindex_t col) const {
	return solution_->board(row)[col] && !this->board_[row][col]->isChecked();
}
