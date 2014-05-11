// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Window for black box puzzle GUI

#include "BoxWindow.h"

BoxWindow::BoxWindow(QWidget *parent) :
			QMainWindow(parent),
			outer(new QVBoxLayout()),
			load(new QPushButton("&Load board")),
			reset(new QPushButton("&Reset game")),
			validate(new QPushButton("&Validate moves")),
			hint(new QPushButton("Get &hint")),
			cheat(new QPushButton("&Cheat and win")),
			rules(new QPushButton("Game r&ules")) {
	setWindowTitle("Sol Boucher's Black Box implementation");
	reset->setEnabled(false);
	validate->setEnabled(false);
	hint->setEnabled(false);
	cheat->setEnabled(false);

	QWidget *central = new QWidget();

	QHBoxLayout *buttons = new QHBoxLayout();
	connect(load, &QPushButton::clicked, this, &BoxWindow::loadBoard);
	connect(reset, &QPushButton::clicked, this, &BoxWindow::resetGame);
	connect(validate, &QPushButton::clicked, this, &BoxWindow::validateMoves);
	connect(hint, &QPushButton::clicked, this, &BoxWindow::requestHint);
	connect(cheat, &QPushButton::clicked, this, &BoxWindow::alternativeVictory);
	connect(rules, &QPushButton::clicked, this, &BoxWindow::displayRules);
	buttons->addWidget(load);
	buttons->addWidget(reset);
	buttons->addWidget(validate);
	buttons->addWidget(hint);
	buttons->addWidget(cheat);
	buttons->addWidget(rules);
	outer->addLayout(buttons);

	central->setLayout(outer);
	setCentralWidget(central);
	show();
}

BoxWindow::~BoxWindow() {
	board.reset();
	delete outer;
	delete load;
	delete reset;
	delete validate;
	delete hint;
	delete cheat;
	delete rules;
}

void BoxWindow::enable_all_buttons(bool include_hint) {
	reset->setEnabled(true);
	validate->setEnabled(true);
	hint->setEnabled(include_hint);
	cheat->setEnabled(include_hint);
}

void BoxWindow::disable_advancing_buttons() {
	validate->setEnabled(false);
	hint->setEnabled(false);
	cheat->setEnabled(false);
}

void BoxWindow::loadBoard() {
	QString filename = QFileDialog::getOpenFileName();
	bool had_board = board.operator bool();
	if(filename.length())
		board = BoxPuzzle::createFromFile(filename.toLocal8Bit().data(), this);
	if(board) {
		if(had_board) {
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents,
					LONGEST_EVENT_LOOP_WAIT);
			resize(0, 0);
		}
		outer->addLayout(board.get());
		enable_all_buttons(false);
	}
}

void BoxWindow::resetGame() {
	board->restart_game();
}

void BoxWindow::validateMoves() const {
	QMessageBox::information(nullptr, "Move validation",
			board->is_on_the_right_track() ?
					"VALID" : "INVALID: You must place all the devices.");
}

void BoxWindow::requestHint() {
	if(!board->advance_game())
		QErrorMessage::qtHandler()->showMessage(
				"The current puzzle has no solution!\nHINT: The solver works "
				"from the upper-left corner, so the only board layout that "
				"doesn't exclude a single possible configuration is to line up"
				"all the devices consecutively starting in the upper-left and "
				"proceeding with a row-major flow (a.k.a. horizontally).  Of "
				"course, this configuration is also the slowest way to start.");
}

void BoxWindow::alternativeVictory() {
	if(!board->fast_track_to_victory())
		QErrorMessage::qtHandler()->showMessage(
				"The current puzzle has no solution!  Try placing all your "
				"pieces horizontally consecutively starting from the "
				"upper-left corner to maximize the solver's search space.");
}

void BoxWindow::displayRules() {
	QMessageBox::information(nullptr, "Help",
			"Black Box is a laser game played on a board bordered by laser "
			"emitters and containing some number of strange \"black box\" "
			"devices at unknown coordinates.  When each laser is turned on, it "
			"fires a laser beam straight outward.  If unhindered, this will be "
			"detected by circuitry in the emitter directly across the board.  "
			"However, when a laser beam interacts with one of the mysterious "
			"devices, its path is modified according to the following rules:\n"
			" - If a laser beam hits a device head-on, it is absorbed and "
			"never reaches the emitters around the perimeter.  This is "
			"considered a HIT.\n"
			" - If a laser beam enters the space diagnally in front and to the "
			"right of a device, it is reflected ninety degrees to the right.\n"
			" - Similarly, if a laser beam enters the space diagnally in front "
			"and to the left of a device, it is reflected ninety degrees to "
			"the left.\n"
			" - If a laser beam enters the space diagnally in front of two "
			"devices, it is reflected back in the direction from which it "
			"came.  Any laser that re-enters the same emitter that produced it "
			"is counted as a REFLECTION.\n"
			" - If a laser beam is reflected at all prior to entering the "
			"field (i.e. before exiting its emitter) is also counted as a "
			"REFLECTION.\n"
			"The goal of the game is to correctly guess the coordinates of "
			"each \"black box\" using the information revealed by the lasers.  "
			"There is no cost associated with firing each laser (or all of "
			"them, for that matter).\n\n"
			"Each emitter indicates the outcome of firing its bean, as "
			"follows:\n"
			" - positive integer: the beam fired from this emitter terminated "
			"at the one with the same numeric identifier\n"
			" - literal 'h': the beam fired from this emitter was absorbed (a "
			"HIT)\n"
			" - literal 'r': the beam fired from this emitter returned (a "
			"REFLECTION)");
}
