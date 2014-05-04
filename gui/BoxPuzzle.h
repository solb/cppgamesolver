// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Graphical configuration for Black Box boards

#ifndef BOX_PUZZLE_H_
#define BOX_PUZZLE_H_

#include "../BoxConfig.h"
#include <QGridLayout>

class BoxPuzzle : public QGridLayout, public BoxConfig {
	Q_OBJECT

public:
	static std::shared_ptr<BoxPuzzle> createFromFile(const char *filename);

private:
	BoxPuzzle(BoxConfig &&plain, QWidget *parent = nullptr);
};

#endif
