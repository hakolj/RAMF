#include "pch.h"
#include "NavigationUpward.h"
NavigationUpward::NavigationUpward() :_recorder(1, 0.0), _cache(1, 0.0) {}
void NavigationUpward::initialize(const std::string& path, const Config& config) {}
void NavigationUpward::reset(const SimuManager& simuManager) {
	auto itf = std::dynamic_pointer_cast<const GetTransRotAble>(simuManager.agent);
	_recorder = std::vector<double>(simuManager.agent->agentnum, 0.0);
	_cache = std::vector<double>(simuManager.agent->agentnum, 0.0);
	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {
		double currenty = itf->getPos()[i][1];
		_cache[i] = currenty;
		_recorder[i] = currenty; // record initial y pos
	}
}

std::vector<double> NavigationUpward::getTotalReward(const SimuManager& simuManager) {
	auto itf = std::dynamic_pointer_cast<const GetTransRotAble>(simuManager.agent);
	std::vector<double> ttrw(simuManager.agent->agentnum, 0.0);
	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {
		ttrw[i] = itf->getPos()[i][1] - _recorder[i];
	}
	return ttrw;
}

void NavigationUpward::getReward(const SimuManager& simuManager, std::vector<double>& reward) {
	//auto itf = dynamic_cast<const GetTransRotAble*>(agent);
	auto itf = std::dynamic_pointer_cast<const GetTransRotAble>(simuManager.agent);
	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {
		double currenty = itf->getPos()[i][1];
		reward[i] = currenty - _cache[i];
		//reward[i] /= 0.0005;
		_cache[i] = currenty;
	}
	return;
}