/*
 * Copyright (C) 2014 Sol Boucher
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#ifndef SOLVER_H_
#define SOLVER_H_

#include <forward_list>
#include <memory>
class Configuration;

// A backtracking solver
std::shared_ptr<Configuration> solver(std::shared_ptr<Configuration> starting,
		std::shared_ptr<std::forward_list<std::shared_ptr<Configuration>>> log = nullptr);

#endif
