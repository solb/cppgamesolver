// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#ifndef BOX_PUZZLE_H_
#define BOX_PUZZLE_H_

#include "../BoxConfig.h"
#include <forward_list>
#include <QGridLayout>
#include <tuple>
#include <vector>

class BoxWindow;
class QCheckBox;
class QWidget;

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

	// Our current backing state for use with the solver
	std::shared_ptr<BoxConfig> config_;

	// The current path to a reachable solution, if any exists
	mutable std::shared_ptr<std::forward_list<std::shared_ptr<Configuration>>> path_;

	// The first reachable solution, if one exists
	mutable std::shared_ptr<BoxConfig> solution_;

	// Value indicating no distinguishing point was found
	const std::tuple<rindex_t, cindex_t> NOTHING_TO_SEE_HERE_;

	// Our containing window
	BoxWindow *parent_;

	// The widgets we need to clean up when we're done
	std::forward_list<QWidget *> visible_;

public:
	// Read a board in from the specified file and return the result
	// Kicks back a nullptr if the file was invalid!
	static std::shared_ptr<BoxPuzzle> createFromFile(const char *filename,
			BoxWindow *parent);

	// Delete the GUI elements so that they disappear from view
	~BoxPuzzle();

	// Whether the puzzle is solveable
	bool has_solution() const;

	// Begin again with a blank board
	void restart_game();

	// Whether the currently-input configuration can be fed into the backtracker
	bool is_on_the_right_track() const;

	// Advance the puzzle toward victory, first by removing an incorrect
	// placement, and if that doesn't work, by adding an ideal one
	bool advance_game();

private:
	BoxPuzzle(unsigned num_devices,
			std::vector<std::vector<char>> &&edge_labels,
			BoxWindow *parent);

	// Called whenever a checkbox is changed
	void board_was_updated(int new_state);

	// Update checkbox states from config_
	void update_checks_from_config();

	// Disable all unticked checkboxes
	void lock_unselected_locations();

	// Reenable all checkboxes
	void unlock_every_location();

	// Find the first coordinate that meets the given criterion
	// Returns the board sizes (a.k.a. NOTHING_TO_SEE_HERE_) if no match.
	std::tuple<rindex_t, cindex_t> first_distinguishing_coordinate(
			bool (BoxPuzzle::*decider)(rindex_t, cindex_t) const) const;

	// Criterion for a move that contradicts the path to a solution
	bool invalid_placement(rindex_t row, cindex_t col) const;

	// Criterion for a move that must be made to reach a solution
	bool missing_placement(rindex_t row, cindex_t col) const;

	// Disable copy and assign
	BoxPuzzle(const BoxPuzzle &) = delete;
	BoxPuzzle &operator=(const BoxPuzzle &) = delete;
};

#endif
