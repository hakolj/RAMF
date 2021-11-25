#pragma once
#ifndef MEMORY_H
#define MEMORY_H

//#include <vector>
#include "Random.h"


class Memory {
public:
	int size;
	int rec_position; //the present save position
	std::vector<std::vector<double>> pool;
private:
	bool ismemfull;
	int _numState, _numAction;

private:
	Memory() {};
public:
	Memory(int size, int dimState, int dimAction) :size(size), rec_position(0), pool(size, std::vector<double>(dimState * 2 + dimAction + 2, 0)),
		ismemfull(false), _numState(dimState), _numAction(dimAction) {};


	void storeMemory(int num, const std::vector<std::vector<double>>& state, const std::vector<int>& action,
		const std::vector<double>& reward, const std::vector<std::vector<double>>& state_next,
		const std::vector<int>& iterminated);

	void randomBatch(int requestNum, std::vector<std::vector<double>>& state, std::vector<int>& action,
		std::vector<double>& reward, std::vector<std::vector<double>>& state_next,
		std::vector<int>& iterminated) const;

	void newestBatch(int requestNum, std::vector<std::vector<double>>& state, std::vector<int>& action,
		std::vector<double>& reward, std::vector<std::vector<double>>& state_next,
		std::vector<int>& iterminated) const;

	void writeMemory(const std::string& path) const;
	void readMemory(const std::string& path);
};



#endif // !MEMORY_H


