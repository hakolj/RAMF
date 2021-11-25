#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace rd {
	// return random int from [low, hi)
	int randi(int low, int high);

	// return 'num' ints as std::vector, range from [low, hi)
	std::vector<int> randi(int num, int low, int high);

	// return random double from [0, 1)
	double randd();

	// return 'num' doubles as std::vector, range from [low, hi)
	std::vector<double> randd(int num, double lo, double hi);

	// return 'num' doubles as std::vector, range from [0, hi)
	std::vector<double> randd(int num, double hi = 1.0);

	// return random selected numbers from [low, hi) as std::vector. num: length of vector 
	std::vector<int> randomChoice(int num, int low, int high);
}
#endif


