// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include "Configuration.h"
using std::ostream;

ostream &operator<<(std::ostream &stream, const Configuration &instance) {
	return stream << instance.str();
}
