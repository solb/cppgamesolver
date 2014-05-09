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

using std::dynamic_pointer_cast;
using std::ifstream;
using std::make_shared;
using std::move;
using std::shared_ptr;
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
			edge_labels_(edge_labels),
			board_(edge_labels[BoxConfig::LEFT_EDGE].size(),
					vector<bool>(edge_labels[BoxConfig::TOP_EDGE].size())),
			tried_to_solve_(false),
			solution_(nullptr) {
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
			QCheckBox *blackbox = new QCheckBox();
			if(board_[r][c])
				blackbox->setCheckState(Qt::Checked);
			addWidget(blackbox, r + 1, c + 1);
		}
}

bool BoxPuzzle::is_on_the_right_track() const {
	if(!tried_to_solve_)
		solution_ = dynamic_pointer_cast<BoxConfig>(solver(solution_));
	if(!solution_)
		return false;

	// Ensure they haven't done anything the solution doesn't
	// TODO This approach doesn't work for boards with multiple solutions
	for(vector<vector<bool>>::size_type r = 0; r < board_.size(); ++r)
		for(vector<bool>::size_type c = 0; c < board_[r].size(); ++c)
			if(this->board_[r][c] && !solution_->board(r)[c])
				return false;

	return true;
}
