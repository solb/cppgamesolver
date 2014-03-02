// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#ifndef WHEEL_CONFIG_H_
#define WHEEL_CONFIG_H_

#include "Configuration.h"

class WheelConfig : public Configuration {
	private:
		const unsigned num_triads_;
		const unsigned num_spaces_;
		const unsigned each_sum_;
		const std::vector<unsigned> gap_sums_;
		std::vector<bool> available_spaces_; // coords [0, num_spaces_)
		std::vector<unsigned> config_; // vals (0, num_spaces_]
		unsigned to_place_;
		mutable std::string repr_;

	public:
		WheelConfig(unsigned num_triads, std::vector<unsigned> &&gap_sums, std::vector<unsigned> &&config = std::vector<unsigned>());

		std::vector<std::shared_ptr<Configuration>> successors() const;

		bool is_goal() const;

		operator const std::string &() const;

	private:
		WheelConfig(const WheelConfig &predecessor, unsigned next_placed);
};

#endif
