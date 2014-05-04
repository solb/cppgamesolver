// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#include "BoxPuzzle.h"
#include <fstream>
#include <memory>
#include <QChar>
#include <QCheckBox>
#include <QErrorMessage>
#include <QLabel>
#include <string>
#include <utility>
#include <vector>

using std::ifstream;
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

	return shared_ptr<BoxPuzzle>(new BoxPuzzle(BoxConfig(num_boxes, move(edges))));
}

BoxPuzzle::BoxPuzzle(BoxConfig &&plain, QWidget *parent) :
			QGridLayout(parent),
			BoxConfig(move(plain)) {
	// Place the edge labels
	for(vector<char>::size_type edge = 0; edge < edge_labels_.size(); ++edge)
		for(unsigned index = 0; index < edge_labels_[edge].size(); ++index) {
			QLabel *label = new QLabel(QString(
					represent_label(edge_labels_[edge][index]).c_str()));
			if(edge % 2)
				addWidget(label, index + 1,
						edge == LEFT_EDGE ? 0 : board_[0].size() + 1);
			else
				addWidget(label, edge == TOP_EDGE ? 0 : board_.size() + 1,
						index + 1);
		}

	// Place the "black box" devices
	for(vector<bool>::size_type r = 0; r < board_.size(); ++r)
		for(vector<bool>::size_type c = 0; c < board_[r].size(); ++c) {
			QCheckBox *blackbox = new QCheckBox();
			if(is_nonempty() && board_[r][c])
				blackbox->setCheckState(Qt::Checked);
			addWidget(blackbox, r + 1, c + 1);
		}
}
