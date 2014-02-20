// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include <iostream>
#include "solver.h"
#include "WheelConfig.h"
using std::cout;
using std::endl;
using std::shared_ptr;

int main() {
	cout << *solver(shared_ptr<Configuration>(new WheelConfig(3, {10, 16, 6}))) << endl;
}
