// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#ifndef BOX_PUZZLE_H_
#define BOX_PUZZLE_H_

#include "../BoxConfig.h"
#include <QCheckBox>
#include <QGridLayout>
#include <tuple>
#include <vector>

typedef std::vector<std::vector<QCheckBox *>>::size_type rindex_t;
typedef std::vector<QCheckBox *>::size_type cindex_t;

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

	// Value indicating no distinguishing point was found
	const std::tuple<rindex_t, cindex_t> NOTHING_TO_SEE_HERE_;

public:
	static std::shared_ptr<BoxPuzzle> createFromFile(const char *filename);

	bool has_solution() const;

	bool is_on_the_right_track() const;

	bool advance_game();

private:
	BoxPuzzle(std::vector<std::vector<char>> &&edge_labels, unsigned num_boxes,
			QWidget *parent = nullptr);

	std::tuple<rindex_t, cindex_t> first_distinguishing_coordinate(
			bool (BoxPuzzle::*decider)(rindex_t, cindex_t) const) const;

	bool invalid_placement(rindex_t row, cindex_t col) const;

	bool missing_placement(rindex_t row, cindex_t col) const;
};

#endif
