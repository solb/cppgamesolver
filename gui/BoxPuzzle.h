/*
 * Copyright (C) 2014 Sol Boucher
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

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
class QLabel;
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

	// Used to output the number of devices remaining to place
	QLabel *remaining_;

	// Whether we've tried to solve the puzzle since the user's input chaged
	mutable bool tried_to_solve_;

	// Our current backing state for use with the solver
	mutable std::shared_ptr<BoxConfig> config_;

	// The current path to a reachable solution, if any exists
	mutable std::shared_ptr<std::forward_list<std::shared_ptr<Configuration>>> path_;

	// The first reachable solution, if one exists
	mutable std::shared_ptr<BoxConfig> solution_;

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

	// Advance the puzzle toward victory by moving to the next different-looking
	// configuration along the path to the solution provided by the solver
	bool advance_game();

	// Jump the puzzle directly to victory, if the solver is able to find one
	bool fast_track_to_victory();

private:
	BoxPuzzle(unsigned num_devices,
			std::vector<std::vector<char>> &&edge_labels,
			BoxWindow *parent);

	// Called whenever a checkbox is changed
	void board_was_updated(int new_state);

	// Update config_ from checkbox states
	void update_config_from_checks() const;

	// Update checkbox states from config_
	void update_checks_from_config();

	// Disable all unticked checkboxes
	void lock_unselected_locations();

	// Reenable all checkboxes
	void unlock_every_location();

	// Disable copy and assign
	BoxPuzzle(const BoxPuzzle &) = delete;
	BoxPuzzle &operator=(const BoxPuzzle &) = delete;
};

#endif
