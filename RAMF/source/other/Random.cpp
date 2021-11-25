#include "pch.h"
#include <random>
#include "Random.h"


namespace rd {
	using namespace std;
	static std::default_random_engine re(time(0));
	static std::uniform_real_distribution<double> default_real_distr(0.0, 1.0);

	int randi(int low, int high) {
		if (low > high - 1) {
			cout << "Error: low > high -1 in rd::randi." << endl;
			exit(0);
		}
		std::uniform_int_distribution<int> intdistr(low, high-1);
		return intdistr(re);
	}
	std::vector<int> randi(int num, int low, int high) {
		if (low > high - 1) {
			cout << "Error: low > high -1 in rd::randi." << endl;
			exit(0);
		}
		std::uniform_int_distribution<int> intdistr(low, high-1);
		std::vector<int> out(num);
		for (int i = 0; i < num; i++) {
			out[i] = intdistr(re);
		}
		return out;
	}

	double randd() {
		return default_real_distr(re);
	}

	std::vector<double> randd(int num, double lo, double hi) {
		std::uniform_real_distribution<double> custom_real_distr(lo, hi);
		std::vector<double> vec(num);

		for (unsigned i = 0; i < vec.size(); i++) {
			vec[i] = custom_real_distr(re);
		}
		return vec;
	}

	std::vector<double> randd(int num, double hi) {
		std::vector<double> vec(num);
		for (unsigned i = 0; i < vec.size(); i++) {
			vec[i] = default_real_distr(re) * hi;
		}
		return vec;

	}


	vector<int> randomChoice(int num, int lo, int hi) {
		int total = hi - lo + 1;
		if (lo > hi - 1) {
			cout << "Error: low > high -1 in rd::randomChoice." << endl;
			exit(0);
		}
		else if (num > total) {
			cout << "required number larger than total number (in rd::randomChoice)" << endl;
			exit(0);
		}
		int remain = total;
		int select = num;
		vector<int> vec(num, 0);
		int idx = 0;

		for (int i = 0; i < total; i++) {
			if (randi(0,remain) < select) {
				vec[idx++] = i + lo;
				select--;
			}
			remain--;
		}
		return vec;
	}
}

