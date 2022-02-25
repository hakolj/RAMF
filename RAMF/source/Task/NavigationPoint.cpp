#include "pch.h"
#include "Task.h"
#include "NavigationPoint.h"

using namespace std;

NavigationPoint::NavigationPoint() :NavigationPoint(0, 0, 0, 0.1) {};
NavigationPoint::NavigationPoint(double x, double y, double z, double bonusrange, double panelty, double bonus) :
	target(x, y, z), bonusrange(bonusrange), panelty(panelty), bonus(bonus), _recorder(1, 0.0) {
	return;
}

void NavigationPoint::initialize(const std::string& path, const Config& config) {
	//read config
	double x, y, z;

	x = config.Read<double>("target x");
	y = config.Read<double>("target y");
	z = config.Read("target z", 0.0);
	bonusrange = config.Read("bonusRange", 0.1);
	target = vec3d(x, y, z);
	return;
}

void NavigationPoint::getReward(const SimuManager& simuManager, std::vector<double>& reward) {
	//auto itf = dynamic_cast<const GetTransRotAble*>(agent);
	auto itf = dynamic_pointer_cast<const GetTransRotAble>(simuManager.agent);
	
	
	//auto itf = dynamic_pointer_cast<GetTransRotAble>(agent);
	vectors3d pos(itf->getPos());
	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {
		double r = (target - pos[i]).squaredNorm();
		if (r > bonusrange * bonusrange) {
			reward[i] = panelty * r;
		}
		else {
			reward[i] = bonus;
		}
		_recorder[i] += reward[i];
		reward[i] /= 10.0;
	}
	return;
}

std::vector<double> NavigationPoint::getTotalReward(const SimuManager& simuManager) {
	return _recorder;
}

void NavigationPoint::reset(const SimuManager& simuManager) {
	_recorder = std::vector<double>(simuManager.agent->agentnum, 0.0);
	//_reward = std::vector<double>(agent->agentnum,0.0);
}
