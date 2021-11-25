#include "pch.h"
#include "Memory.h"
#include "Fop.h"
#include "torch/torch.h"
#include "torch/script.h"
using namespace std;

void Memory::storeMemory(int num, const std::vector<std::vector<double>>& state, const std::vector<int>& action,
	const std::vector<double>& reward, const std::vector<std::vector<double>>& state_next,
	const std::vector<int>& iterminated) {
	unsigned numState = state[0].size();
	for (int i = 0; i < num; i++) {
		for (unsigned j = 0; j < numState; j++) {
			pool[rec_position][j] = state[i][j]; //sn
			pool[rec_position][j + numState + 2] = state_next[i][j]; //sn_next
		}
		pool[rec_position][numState] = action[i];
		pool[rec_position][numState + 1] = reward[i];
		pool[rec_position][numState * 2 + 2] = iterminated[i];
		rec_position++;
		if (!ismemfull) {
			ismemfull = (rec_position == size);
		}
		rec_position %= size;
	}
	return;
}

void Memory::randomBatch(int requestNum, std::vector<std::vector<double>>& state, std::vector<int>& action,
	std::vector<double>& reward, std::vector<std::vector<double>>& state_next,
	std::vector<int>& iterminated) const {
	std::vector<int> selected = ismemfull ? rd::randomChoice(min(requestNum,size), 0, size - 1) : rd::randomChoice(min(requestNum,rec_position), 0, rec_position - 1);
	int num = selected.size();
	state = std::vector<std::vector<double>>(num, std::vector<double>(_numState));
	state_next = std::vector<std::vector<double>>(num, std::vector<double>(_numState));
	action = std::vector<int>(num);
	reward = std::vector<double>(num);
	iterminated = std::vector<int>(num);

	for (int i = 0; i < num; i++) {
		for (unsigned j = 0; j < _numState; j++) {
			state[i][j] = pool[selected[i]][j];//sn
			state_next[i][j] = pool[selected[i]][j + _numState + 2]; //sn_next
		}
		action[i] = pool[selected[i]][_numState];
		reward[i] = pool[selected[i]][_numState + 1];
		iterminated[i] = pool[selected[i]][_numState * 2 + 2];
	}
	return;
}

void Memory::newestBatch(int requestNum, std::vector<std::vector<double>>& state, std::vector<int>& action,
	std::vector<double>& reward, std::vector<std::vector<double>>& state_next,
	std::vector<int>& iterminated) const {
	unsigned int choicestart = 0;
	unsigned int num = 0;
	if (ismemfull) {
		choicestart = (rec_position - requestNum) % size + size;
		num = requestNum;
	}
	else {
		choicestart = max(rec_position - requestNum, 0);
		num = rec_position - choicestart;
	}

	state = std::vector<std::vector<double>>(num, std::vector<double>(_numState));
	state_next = std::vector<std::vector<double>>(num, std::vector<double>(_numState));
	action = std::vector<int>(num);
	reward = std::vector<double>(num);
	iterminated = std::vector<int>(num);

	for (unsigned int i = 0; i < num; i++) {
		unsigned int selected = (choicestart + i) % size;
		for (unsigned j = 0; j < _numState; j++) {
			state[i][j] = pool[selected][j];//sn
			state_next[i][j] = pool[selected][j + _numState + 2]; //sn_next
		}
		action[i] = pool[selected][_numState];
		reward[i] = pool[selected][_numState + 1];
		iterminated[i] = pool[selected][_numState * 2 + 2];
	}



}

void Memory::writeMemory(const std::string& path) const {
	std::ofstream of;
	of.open(path + "/mem.txt",ios::out);
	
	//outfile << std::showpoint << std::left << std::fixed << std::setprecision(8);
	//of.setf(ios::showpoint | ios::left | ios::scientific);
	//of.precision(8);
	of << this->size << " " << this->_numState << " " << this->_numAction
		<< " " << this->ismemfull << " " << this->rec_position << endl;
	Fop::writevec2d<double>(this->pool, of);
	of.close();

	return;
}

void Memory::readMemory(const std::string& path) {
	std::ifstream infile;
	infile.open(path+"/mem.txt",ios::in);
	infile >> this->size >> this->_numState >> this->_numAction >> this->ismemfull >> this->rec_position;
	infile.close();
	this->pool = Fop::loadvec2d<double>(path + "/mem.txt", 1);
	//check
	if (pool[0].size() != _numState * 2 + _numAction + 2) {
		cout << "pool size error";
		exit(0);
	}
	return;

}