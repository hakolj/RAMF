#include "pch.h"
#include "DistanceFromBoundary.h"


using namespace std;

DistanceFromBoundary::DistanceFromBoundary(): _cache(1, 0.0) {};

void DistanceFromBoundary::initialize(const std::string& path, const Config& config) {
	_boundaryID = config.Read<int>("selected Boundary");
	_direction = config.Read<int>("direction");

	_rwPower = config.Read<int>("reward power", 1);
	_totalrwPower = config.Read<int>("total reward power", 1);
	_c2 = config.Read<double>("c2", 0.0);

};


void DistanceFromBoundary::getReward(const SimuManager& simuManager, std::vector<double>& reward) {

	getDistanceValue(simuManager, reward, _rwPower);
	double temp = 0.0;
	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {
		temp = reward[i]; // store the new value.
		reward[i] += _c2 * (reward[i] - _cache[i]); // reward is value + c2* the relative value
		_cache[i] = temp; // update cache
	}
}


std::vector<double> DistanceFromBoundary::getTotalReward(const SimuManager& simuManager) {
	std::vector<double> reward(simuManager.agent->agentnum, 0.0);

	getDistanceValue(simuManager, reward, _totalrwPower);
	return reward;
}
void DistanceFromBoundary::reset(const SimuManager& simuManager) {
	
	_cache = std::vector<double>(simuManager.agent->agentnum, 0.0);
	getDistanceValue(simuManager, _cache, _rwPower);
}


void DistanceFromBoundary::getDistanceValue(const SimuManager& simuManager, std::vector<double>& value, int power) {
	
	double boundaryPos[3];
	simuManager.env->getDomainSize(boundaryPos[0], boundaryPos[1], boundaryPos[2]);

	auto itf = dynamic_pointer_cast<const GetTransRotAble>(simuManager.agent);
	vectors3d pos(itf->getPos());


	for (unsigned i = 0; i < simuManager.agent->agentnum; i++) {
		//reward[i] = pow(pos[i][_boundaryID] / boundaryPos[_boundaryID], _totalrwPower) + pow(1.0 - pos[i][_boundaryID] / boundaryPos[_boundaryID], _totalrwPower);
		if (power == 1)	value[i] = max(pos[i][_boundaryID] / boundaryPos[_boundaryID], 1.0 - pos[i][_boundaryID] / boundaryPos[_boundaryID]);
		else value[i] = pow(pos[i][_boundaryID] / boundaryPos[_boundaryID], power) + pow(1.0 - pos[i][_boundaryID] / boundaryPos[_boundaryID], power);
		value[i] *= _direction;
	}
}
