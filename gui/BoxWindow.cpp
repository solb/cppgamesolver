// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Window for black box puzzle GUI

#include "BoxWindow.h"

BoxWindow::BoxWindow(QWidget *parent) :
			QMainWindow(parent),
			outer(new QVBoxLayout()),
			load(new QPushButton("&Load board")),
			validate(new QPushButton("&Validate moves")),
			hint(new QPushButton("Get &hint")) {
	setWindowTitle("Sol Boucher's Black Box implementation");
	validate->setEnabled(false);
	hint->setEnabled(false);

	QWidget *central = new QWidget();

	QHBoxLayout *buttons = new QHBoxLayout();
	connect(load, &QPushButton::clicked, this, &BoxWindow::loadBoard);
	connect(validate, &QPushButton::clicked, this, &BoxWindow::validateMoves);
	connect(hint, &QPushButton::clicked, this, &BoxWindow::requestHint);
	buttons->addWidget(load);
	buttons->addWidget(validate);
	buttons->addWidget(hint);
	outer->addLayout(buttons);

	central->setLayout(outer);
	setCentralWidget(central);
	show();
}

BoxWindow::~BoxWindow() {
	delete load;
}

void BoxWindow::enable_all_buttons() {
	validate->setEnabled(true);
	hint->setEnabled(true);
}

void BoxWindow::disable_advancing_buttons() {
	validate->setEnabled(false);
	hint->setEnabled(false);
}

void BoxWindow::loadBoard() {
	QString filename = QFileDialog::getOpenFileName();
	if(filename.length())
		board = BoxPuzzle::createFromFile(filename.toLocal8Bit().data(), this);
	if(board) {
		outer->addLayout(board.get());
		enable_all_buttons();
	}
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
