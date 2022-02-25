#include "pch.h"
//#include "MachineLearning.h"
#include "QLearning.h"
#include <vector>
#include "Fop.h"

using namespace::std;

//QLearning::QLearning(int numState, int numAction) {
//
//}
QLearning::QLearning(int dimState, int numAction, const Config& config):
	dimState(dimState), numAction(numAction), loss_record(0), qvalue_record(0),
	lr(0),threshold(vector<vector<double>>(dimState))
{
	std::stringstream ss;
	// read thresholds
	for (int i = 0; i < dimState; i++) {
		ss.str("");
		ss << i;
		std::string strthre = config.Read<std::string>("threshold" + ss.str());
		threshold[i] = Fop::loadvec1d<double>(strthre);
	}
	numState = 1;
	for (int i = 0; i < dimState; i++) {
		numState *= threshold[i].size() + 1;
	}
	
	qtable = (vector<vector<double>>(numState, vector<double>(numAction, 0)));
	deltaq = (vector<vector<double>>(numState, vector<double>(numAction, 0)));

}
//QLearning::QLearning(int numState, int numAction, const std::vector<int>& levels):
//	stateNum(numState),actionNum(numAction),levels(levels)
//{
//	if (numState != levels.size()) {
//		cout << "State number is not equal to the size of levels in constructor of QLearning." << endl;
//		exit(0);
//	}
//	totalState = 1;
//	for (int i = 0; i < numState; i++) {
//		totalState *= levels[i];
//	}
//	qtable = (vector<vector<double>>(totalState, vector<double>(numAction, 0)));
//	deltaq = (vector<vector<double>>(totalState, vector<double>(numAction, 0)));
//}

void QLearning::train(Memory* memory) {
	int req_batch_size = 100;
	vector<vector<double>> state0;
	vector<vector<double>> state1;
	vector<double> reward;
	vector<int> action;
	vector<int> isterminated;
	memory->newestBatch(req_batch_size, state0, action, reward, state1, isterminated);

	discretize(state0);
	discretize(state1);

	extv::assign<double>(deltaq, 0);
	for (int i = 0; i < state0.size(); i++) {
		int index0 = toIndex(state0[i]);
		int index1 = toIndex(state1[i]);
		//double maxqa = *(std::max_element(qtable[state0[i][0]].begin(), qtable[state0[i][0]].end()));
		double maxqa = extv::maxval<double>(qtable[index1]);
		double step = (reward[i] + GAMMA * maxqa - qtable[index0][action[i]]);
		deltaq[index0][action[i]] += lr * step / state0.size();  //update
	}

	for (int i = 0; i < qtable.size(); i++) {
		for (int j = 0; j < qtable[i].size(); j++) {
			qtable[i][j] += deltaq[i][j];
		}
	}

}

void QLearning::initialize(const std::string& path, const Config& config) {
	_lr_initial = config.Read<double>("lr", 0.1);
	_sigma = config.Read<int>("lr sigma", 1e8);
	_epsi_initial = config.Read<double>("epsilon ", 0.0);
	_episode_0 = config.Read<int>("no exp episode ", 1);
	if (_sigma <= 0) { cout << "invalid 'sigma' for ML:QLearning" << endl; exit(1); }
	if (_episode_0 <= 0) { cout << "invalid 'no exp episode' for ML:QLearning" << endl; exit(1); }

	cout << "initializing" << endl;
	for (int i = 0; i < numState; i++) {
		for (double& q : qtable[i]) {
			q = 500.0;
		}
	}
}

void QLearning::setNewEp(int ep) {
	_episode = ep;
	lr = _lr_initial * _sigma / (_sigma + _episode);
	_epsi = max(0.0, _epsi_initial * (1.0 - double(_episode) / double(_episode_0)));

}

std::vector<int> QLearning::decideAction(const std::vector<std::vector<double>>& state, bool iexplore) {
	vector<int> action(state.size());
	vector<vector<double>> state_desc(state.size(), vector<double>(dimState));
	discretize(state, state_desc);
	double epsilon = iexplore ? _epsi : 0;

	std::vector<int> maxq(numState);

	for (int i = 0; i < numState; i++) {
		maxq[i] = extv::maxloc(qtable[i]);
	}

	//maxq = std::vector<int>({ 2, 2, 0 });

	for (int i = 0; i < state.size(); i++) {
		int index = toIndex(state_desc[i]);
		int maxa;
		//int maxa = extv::maxloc(qtable[index]);

		if ((index < 0 || index >= numState)) {
			cout << "index out of maximal dimState of Qtable in decideAction" << endl;
			maxa = -1;
		}
		else {
			maxa = maxq[index];
		}

		//epsilon greedy
		double arand = rd::randd();
		action[i] = arand < epsilon ? rd::randi(0, this->numAction) : maxa; 
		
	}
	return action;
}



void QLearning::model_saver(const std::string& path, const std::string& suffix) {
	string qpath = path + "/qtable" + suffix + ".txt";
	Fop::writevec2d<double>(qtable, qpath);
}

void QLearning::model_loader(const std::string& path) {
	string qpath = path + "/qtable.txt";
	qtable = Fop::loadvec2d<double>(qpath);
}

void QLearning::recParam(const char* path) {
	return;
}

//from discretized states to index of all states
int QLearning::toIndex(const std::vector<double>& state) {
	int idx = 0;
	int mult = 1;
	for (int i = 0; i < dimState; i++) {
		idx += state[i] * mult;
		mult *= threshold[i].size() + 1;
	}
	return idx;
}

//from index of all state to discretized states
std::vector<double> QLearning::toState(int index) {
	std::vector<double> state(dimState);
	for (int i = 0; i < dimState; i++) {
		state[i] = index % (threshold[i].size() + 1);
		index = (index - state[i]) / (threshold[i].size() + 1);
	}
	return state;
}

void QLearning::discretize(std::vector<std::vector<double>>& state) {
	
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < dimState; j++) {
			int n = 0;
			while (n < threshold[j].size()) {
				if (state[i][j] < threshold[j][n++]) {
					n--;
					break;
				}

			}
			state[i][j] = n;
			if (n > threshold[j].size()) {
				std::cout << "?" << std::endl;
			}
		}
	}

}
void QLearning::discretize(const std::vector<std::vector<double>>& state, std::vector<std::vector<double>>& state_desc) {

	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < dimState; j++) {
			int n = 0;
			while (n < threshold[j].size()) {
				if (state[i][j] < threshold[j][n++]) {
					n--;
					break;
				}

			}
			state_desc[i][j] = n;
			if (n > threshold[j].size()) {
				std::cout << "?" << std::endl;
			}
		}
	}

}