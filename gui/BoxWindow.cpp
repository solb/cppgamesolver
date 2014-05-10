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
			hint(new QPushButton("Get &hint")) {
	setWindowTitle("Sol Boucher's Black Box implementation");
	reset->setEnabled(false);
	validate->setEnabled(false);
	hint->setEnabled(false);

	QWidget *central = new QWidget();

	QHBoxLayout *buttons = new QHBoxLayout();
	connect(load, &QPushButton::clicked, this, &BoxWindow::loadBoard);
	connect(reset, &QPushButton::clicked, this, &BoxWindow::resetGame);
	connect(validate, &QPushButton::clicked, this, &BoxWindow::validateMoves);
	connect(hint, &QPushButton::clicked, this, &BoxWindow::requestHint);
	buttons->addWidget(load);
	buttons->addWidget(reset);
	buttons->addWidget(validate);
	buttons->addWidget(hint);
	outer->addLayout(buttons);

	central->setLayout(outer);
	setCentralWidget(central);
	show();
}

void BoxWindow::enable_all_buttons() {
	reset->setEnabled(true);
	validate->setEnabled(true);
	hint->setEnabled(true);
}

void BoxWindow::disable_advancing_buttons() {
	validate->setEnabled(false);
	hint->setEnabled(false);
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
		enable_all_buttons();
	}
}

void BoxWindow::resetGame() {
	board->restart_game();
}

void BoxWindow::validateMoves() const {
	QMessageBox::information(nullptr, "Move validation",
			board->is_on_the_right_track() ? "VALID" : "INVALID");
}

void BoxWindow::requestHint() {
	if(!board->advance_game())
		QErrorMessage::qtHandler()->showMessage(
				"The current puzzle has no solution!");
}
