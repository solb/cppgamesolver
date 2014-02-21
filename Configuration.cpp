// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#include "Configuration.h"
using std::string;

Configuration::operator const char *() const {
	return (operator const string)().c_str();
}
