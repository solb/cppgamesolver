// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Window for black box puzzle GUI

#ifndef BOX_WINDOW_H_
#define BOX_WINDOW_H_

#include "BoxPuzzle.h"
#include <memory>
#include <QtWidgets>

class BoxWindow : public QMainWindow {
	Q_OBJECT

private:
	// Longest we will stall the application waiting for the GUI loop to update
	static const int LONGEST_EVENT_LOOP_WAIT = 100; // ms

	// Layout for the window contents
	QVBoxLayout *outer;

	// Button to load a puzzle from a file
	QPushButton *load;

	// Button to validate the puzzle for legal moves
	QPushButton *validate;

	// Button to advance the puzzle toward a correct solution
	QPushButton *hint;

	// The current game board
	std::shared_ptr<BoxPuzzle> board;

public:
	// Make and display the window
	explicit BoxWindow(QWidget *parent = nullptr);

	// Light up all the control buttons at the top of the screen
	void enable_all_buttons();

	// Disable the buttons that are used to advance/change the puzzle state
	void disable_advancing_buttons();

private:
	// Called when the load button is pressed
	void loadBoard();

	// Called when the validate button is pressed
	void validateMoves() const;

	// Called when the hint button is pressed
	void requestHint();

	// Disable copy and assign
	BoxWindow(const BoxWindow &) = delete;
	BoxWindow &operator=(const BoxWindow &) = delete;
};

#endif
