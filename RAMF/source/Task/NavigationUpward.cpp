#include "pch.h"
#include "NavigationUpward.h"
NavigationUpward::NavigationUpward() :_recorder(1, 0.0), _cache(1, 0.0) {}
void NavigationUpward::initialize(const std::string& path, const Config& config) {}
void NavigationUpward::reset(const Agent* agent) {
	auto itf = dynamic_cast<const GetTransRotAble*>(agent);
	_recorder = std::vector<double>(agent->agentnum, 0.0);
	_cache = std::vector<double>(agent->agentnum, 0.0);
	for (unsigned i = 0; i < agent->agentnum; i++) {
		double currenty = itf->getPos()[i][1];
		_cache[i] = currenty;
		_recorder[i] = currenty; // record initial y pos
	}
}

std::vector<double> NavigationUpward::getTotalReward(const Agent* agent) {
	auto itf = dynamic_cast<const GetTransRotAble*>(agent);
	std::vector<double> ttrw(agent->agentnum, 0.0);
	for (unsigned i = 0; i < agent->agentnum; i++) {
		ttrw[i] = itf->getPos()[i][1] - _recorder[i];
	}
	return ttrw;
}

void NavigationUpward::getReward(const Agent* const agent, std::vector<double>& reward) {
	auto itf = dynamic_cast<const GetTransRotAble*>(agent);
	for (unsigned i = 0; i < agent->agentnum; i++) {
		double currenty = itf->getPos()[i][1];
		reward[i] = currenty - _cache[i];
		//reward[i] /= 0.0005;
		_cache[i] = currenty;
	}
	return;
}