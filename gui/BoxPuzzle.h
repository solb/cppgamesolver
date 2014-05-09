// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#ifndef BOX_PUZZLE_H_
#define BOX_PUZZLE_H_

#include "../BoxConfig.h"
#include <QCheckBox>
#include <QGridLayout>
#include <vector>

class BoxPuzzle : public QGridLayout {
	Q_OBJECT

private:
	// Our edge labels (analagous to BoxConfig::edge_labels_)
	std::vector<std::vector<char>> edge_labels_;

	// Our board (analagous to BoxConfig::edge_labels_)
	std::vector<std::vector<QCheckBox *>> board_;

	// Whether we've tried to solve the puzzle yet
	mutable bool tried_to_solve_;

	// Our starting configuration, or solution once we've solved it
	mutable std::shared_ptr<BoxConfig> solution_;

public:
	static std::shared_ptr<BoxPuzzle> createFromFile(const char *filename);

	bool is_on_the_right_track() const;

private:
	BoxPuzzle(std::vector<std::vector<char>> &&edge_labels, unsigned num_boxes,
			QWidget *parent = nullptr);
};

#endif
