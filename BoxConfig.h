// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#ifndef BOX_CONFIG_H_
#define BOX_CONFIG_H_

#include "Configuration.h"

// A configuration for boxing with lasers using backtracking
class BoxConfig : public Configuration {
	private:
		// The edge label sentinel character indicating a hit
		static const char HIT_CHAR = 'h';

		// The edge label sentinel character indicating a reflection
		static const char REFL_CHAR = 'r';

		// The indices of the edge labels for a given side of the board
		static const std::vector<std::vector<unsigned>>::size_type TOP_EDGE = 0;
		static const std::vector<std::vector<unsigned>>::size_type RIGHT_EDGE =
				1;
		static const std::vector<std::vector<unsigned>>::size_type BOTTOM_EDGE =
				2;
		static const std::vector<std::vector<unsigned>>::size_type LEFT_EDGE =
				3;

		// Number of "black box" devices that need to be placed
		const unsigned num_devices_;

		// Edge labels (for the board border described by the *_EDGE constants)
		const std::vector<std::vector<unsigned>> edge_labels_;

		// Play board, where each true represents a mysterious device
		std::vector<std::vector<bool>> board_;

		// String representation of this instance
		mutable std::string repr_;

	public:
		BoxConfig(unsigned num_devices,
				std::vector<std::vector<unsigned>> &&edge_labels);

		std::vector<std::shared_ptr<Configuration>> successors() const;

		bool is_nonempty() const;

		bool is_goal() const;

		operator const std::string &() const;

	private:
		// Returns a string representation of an edge label, which is a
		// character if the label is a sentinel value, and numeric otherwise
		static std::string represent_label(unsigned label);
};

#endif
