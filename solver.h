// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#ifndef SOLVER_H_
#define SOLVER_H_

#include <forward_list>
#include <memory>
class Configuration;

std::shared_ptr<Configuration> solver(std::shared_ptr<Configuration> starting,
		std::shared_ptr<std::forward_list<std::shared_ptr<Configuration>>> log = nullptr);

#endif
