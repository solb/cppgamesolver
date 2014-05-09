// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#ifndef BOX_PUZZLE_H_
#define BOX_PUZZLE_H_

#include "../BoxConfig.h"
#include <QGridLayout>
#include <tuple>
#include <vector>

class BoxWindow;
class QCheckBox;

typedef std::vector<std::vector<QCheckBox *>>::size_type rindex_t;
typedef std::vector<QCheckBox *>::size_type cindex_t;

class BoxPuzzle : public QGridLayout {
	Q_OBJECT

private:
	// Our number of devices (analagous to BoxConfig::num_devices_)
	const unsigned num_devices_;

	// The number of devices we've placed thus far
	unsigned placed_devices_;

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

	// Our containing window
	BoxWindow *parent_;

public:
	static std::shared_ptr<BoxPuzzle> createFromFile(const char *filename,
			BoxWindow *parent);

	bool has_solution() const;

	bool is_on_the_right_track() const;

	bool advance_game();

private:
	BoxPuzzle(unsigned num_devices,
			std::vector<std::vector<char>> &&edge_labels,
			BoxWindow *parent);

	void board_was_updated(int new_state);

	void lock_unselected_locations();

	void unlock_every_location();

	std::tuple<rindex_t, cindex_t> first_distinguishing_coordinate(
			bool (BoxPuzzle::*decider)(rindex_t, cindex_t) const) const;

	bool invalid_placement(rindex_t row, cindex_t col) const;

	bool missing_placement(rindex_t row, cindex_t col) const;
};

#endif
