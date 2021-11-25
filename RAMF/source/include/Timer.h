#ifndef TIMER_H
#define TIMER_H

#include <time.h>

class Timer {

#if (defined(_WIN32)||defined(_WIN64))

private:
	clock_t _tic, _toc;
public:
	Timer() :_tic(0), _toc(0) {}
	inline clock_t Tic() {
		_tic = clock();
		return _tic;
	}
	inline clock_t Toc() {
		_toc = clock();
		return _toc;
	}

	static inline double elaspe(const clock_t& tic, const clock_t& toc) {
		return  (double(toc) - double(tic)) / double(CLOCKS_PER_SEC);
	}
	inline double elaspe() {
		return  (double(_toc) - double(_tic)) / double(CLOCKS_PER_SEC);
	}

#elif (defined(__linux__))
private:
	timespec _tic, _toc;
public:
	Timer() :_tic(), _toc() {}
	inline timespec Tic() {
		clock_gettime(CLOCK_MONOTONIC, &_tic);
		return _tic;
	}
	inline timespec Toc() {
		clock_gettime(CLOCK_MONOTONIC, &_toc);
		return _toc;
	}
	static inline double elaspe(const timespec& start, const timespec& end) {
			timespec temp;
			if ((end.tv_nsec - start.tv_nsec) < 0) {
				temp.tv_sec = end.tv_sec - start.tv_sec - 1;
				temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
			}
			else {
				temp.tv_sec = end.tv_sec - start.tv_sec;
				temp.tv_nsec = end.tv_nsec - start.tv_nsec;
			}
			return temp.tv_sec + 1e-9 * temp.tv_nsec;
	}
			
	inline double elaspe() {
			timespec temp;
			if ((_toc.tv_nsec - _tic.tv_nsec) < 0) {
				temp.tv_sec = _toc.tv_sec - _tic.tv_sec - 1;
				temp.tv_nsec = 1000000000 + _toc.tv_nsec - _tic.tv_nsec;
			}
			else {
				temp.tv_sec = _toc.tv_sec - _tic.tv_sec;
				temp.tv_nsec = _toc.tv_nsec - _tic.tv_nsec;
			}
			return temp.tv_sec + 1e-9 * temp.tv_nsec;
		
	}
#endif

};


#endif


