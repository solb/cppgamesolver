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
	QVBoxLayout *outer;
	QPushButton *load;
	std::shared_ptr<BoxPuzzle> board;

public:
	explicit BoxWindow(QWidget *parent = nullptr);
	virtual ~BoxWindow();

private:
	void loadBoard();

	// Disable copy and assign
	BoxWindow(const BoxWindow &) = delete;
	BoxWindow &operator=(const BoxWindow &) = delete;
};

#endif
