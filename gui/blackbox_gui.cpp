// Sol Boucher
// CSCI-541-01 Strout's F(ant)abulous C++11 Concoction
// 05/11/14 Project 3

// Window for black box puzzle GUI

#include "BoxWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	BoxWindow window;
	return app.exec();
}
